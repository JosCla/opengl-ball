// collision.cpp
// functions for detecting collisions between objects

// libraries
#include <glm/glm.hpp> // for gl maths
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// stdlib
#include <iostream> // for cout
#include <cmath> // for abs
#include <algorithm> // for min, max
#include <vector> // for vector

// our files
#include "shapes.h" // shape classes
#include "utils.h" // utility functions

// finds signed distance from a point to a plane defined by a triangle
float signedDistanceToPlane(const Triangle &tri, const glm::vec3 &point)
{
	return glm::dot(tri.Normal, point) - glm::dot(tri.Normal, tri.Vertices[0]);
}

// finds whether a point, when projected onto a triangle's plane, is inside the triangle
bool pointInsideTriangle(const Triangle &tri, const glm::vec3 &point)
{
	for (unsigned int i = 0; i < 3; ++i)
	{
		if (glm::dot(glm::cross(tri.Vertices[(i + 1) % 3] - tri.Vertices[i], point - tri.Vertices[i]), tri.Normal) < 0)
			return false;
	}
	return true;
}

// finds the time and point at which ellipsoid intersects with a triangle
// returns whether or not an intersection happens
// based on http://www.peroxide.dk/papers/collision/collision.pdf
bool computeIntersection(const Ellipsoid &ellip, const glm::vec3& pos, const glm::vec3& vel, const Triangle &tri, float &collisionTime, glm::vec3 &collisionPoint)
{
	float EPSILON = 0.00001;
	float baseDistToPlane = signedDistanceToPlane(tri, pos);
	float normDotVel = glm::dot(tri.Normal, vel);

	// back face culling
	if (normDotVel > 0)
		return false;

	// t0 is time when sphere enters plane, t1 is time when it exits
	float t0, t1;
	bool embedded = false;

	if (std::abs(normDotVel) > EPSILON)
	{
		// if it's not moving parallel to the plane, we find times of entering and exiting by
		// asking when points and the sphere closest and farthest from the plane intersect it.
		t0 = (1.0f - baseDistToPlane) / (normDotVel);
		t1 = (-1.0f - baseDistToPlane) / (normDotVel);

		// checking for t0 and t1 being too big for collision to happen
		if (t0 > 1.0f || t1 < 0.0f)
		{
			return false;
		}

		// clamping t0 and t1 to [0, 1]
		if (t0 < 0.0f)
			t0 = 0.0f;
		if (t1 < 0.0f)
			t1 = 0.0f;
		if (t0 > 1.0f)
			t0 = 1.0f;
		if (t1 > 1.0f)
			t1 = 1.0f;
	}
	else
	{
		// if it's moving parallel to the plane, then it's either embedded, in which case it 
		// intersects the whole time, or will never touch the triangle's plane, in which case 
		// we can return early.
		if (std::abs(baseDistToPlane) <= 1)
		{
			t0 = 0;
			t1 = 1;
			embedded = true;
		}
		else
		{
			return false;
		}
	}

	// defining some variables to set as collisions are detected
	bool collision = false;
	collisionTime = 1.0f;
	collisionPoint = glm::vec3(0.0f);

	// proceeding with face intersection test:
	if (!embedded)
	{
		// checking if intersection point is in triangle
		glm::vec3 intersectionPoint = pos - tri.Normal + (t0 * vel);
		if (t0 >= 0 && t0 < 1 && pointInsideTriangle(tri, intersectionPoint))
		{
			collision = true;
			collisionTime = t0;
			collisionPoint = intersectionPoint;
		}
	}

	// if a face collision happens, no other collision can theoretically happen
	if (!collision)
	{
		// note: converting certain values to doubles instead of floats for calculations
		//   reduces error by about 2 to 3 orders of magnitude on average

		// proceeding with vertex intersection test:
		for (unsigned int i = 0; i < 3; ++i)
		{
			// solving for when the squared distance between the center of the sphere and
			// the vertex equals one yields the following quadratic equation
			double a = glm::dot(vel, vel);
			double b = 2.0f * glm::dot(vel, (pos - tri.Vertices[i]));
			double c = std::pow(glm::length(tri.Vertices[i] - pos), 2) - 1.0f;

			double r0, r1;
			if (solveQuadrat(a, b, c, r0, r1))
			{
				double time = std::min(r0, r1);
				if (time < 0 && time >= -0.01)
					time = 0; // helps mitigate false negatives from calculation imprecision 

				if (time >= 0 && time < collisionTime)
				{
					collision = true;
					collisionTime = time;
					collisionPoint = tri.Vertices[i];
				}
			}
		}

		// proceeding with edge intersection test:
		for (unsigned int i = 0; i < 3; ++i)
		{
			glm::vec3 currEdge = tri.Vertices[(i+1)%3] - tri.Vertices[i];
			glm::vec3 posToVertex = tri.Vertices[i] - pos;
			double edgeLenSquared = glm::length(currEdge) * glm::length(currEdge);
			double velLenSquared = glm::length(vel) * glm::length(vel);
			double posToVertLenSquared = glm::length(posToVertex) * glm::length(posToVertex);
			double edgeDotVel = glm::dot(currEdge, vel);
			double posToVertDotVel = glm::dot(posToVertex, vel);
			double edgeDotPosToVert = glm::dot(currEdge, posToVertex);

			// process for determining when distance between sphere's center and the edge
			// line boils down to another quadratic equation
			double a = edgeLenSquared * -velLenSquared + edgeDotVel * edgeDotVel;
			double b = edgeLenSquared * (2.0f * posToVertDotVel) - 2.0f * edgeDotVel * edgeDotPosToVert;
			double c = edgeLenSquared * (1.0f - posToVertLenSquared) + edgeDotPosToVert * edgeDotPosToVert;

			double r0, r1;
			if (solveQuadrat(a, b, c, r0, r1))
			{
				double time = std::min(r0, r1);
				if (time < 0 && time >= -0.01)
					time = 0; // helps mitigate false negatives from calculation imprecision 
				
				double relativePosOnEdge = ((float)time * glm::dot(currEdge, vel) - glm::dot(currEdge, posToVertex)) / std::pow(glm::length(currEdge), 2);

				if (relativePosOnEdge > 0 && relativePosOnEdge < 1)
				{
					glm::vec3 posOnEdge = tri.Vertices[i] + ((float)relativePosOnEdge * currEdge);

					if (time >= 0 && time < collisionTime)
					{
						collision = true;
						collisionTime = time;
						collisionPoint = posOnEdge;
					}
				}
			}
		}
	}

	return collision;
}

// function overload for inputting vertices instead of a triangle
bool computeIntersection(const Ellipsoid &ellip, const glm::vec3 &pos, const glm::vec3 &vel, const glm::vec3 &vert0, const glm::vec3 &vert1, const glm::vec3 &vert2, float &collisionTime, glm::vec3 &collisionPoint)
{
	Triangle tri(vert0, vert1, vert2);
	return computeIntersection(ellip, pos, vel, tri, collisionTime, collisionPoint);
}

void handleIntersection(const Ellipsoid &ellip, const std::vector<Triangle> &tris, glm::vec3 &pos, glm::vec3 &vel) 
{
	float EPSILON = 0.00001f;
	float totalTimePassed = 0.0f;
	unsigned int passes = 0;

	while (totalTimePassed < 1.0f)
	{
		bool collision;
		float collisionTime = 1.0f;
		glm::vec3 collisionPoint;

		// TODO: make an octree or something. anything to make this more efficient
		for (unsigned int i = 0; i < tris.size(); ++i)
		{
			float currCollisionTime;
			glm::vec3 currCollisionPoint;
			if (computeIntersection(ellip, pos, vel, tris[i], currCollisionTime, currCollisionPoint))
			{
				// the second part of the following if-statement makes sure that the ellipsoid
				// will not get stuck repeatedly colliding with something at time = 0
				if (currCollisionTime < collisionTime && std::abs(glm::dot(vel, pos + (vel * currCollisionTime) - currCollisionPoint)) > EPSILON)
				{
					collision = true;
					collisionTime = currCollisionTime;
					collisionPoint = currCollisionPoint;
				}
			}
		}

		// if a collision happened, then we compute its effect, and keep going until
		// we've passed the entire frame
		if (collision && collisionTime < (1.0f - totalTimePassed))
		{
			pos += (vel * collisionTime);

			glm::vec3 slidingPlaneNormal = pos - collisionPoint;

			glm::vec3 projection = glm::dot(vel, glm::normalize(slidingPlaneNormal)) * glm::normalize(slidingPlaneNormal);
			vel -= projection;
		}
		else
		{
			collisionTime = 1.0f;

			pos += (vel * (1.0f - totalTimePassed));
		}

		totalTimePassed += collisionTime;

		if (++passes >= 3)
			break; // prevents hard-locking
	}

	// adding gravity and friction to velocity post-calculations
	vel += glm::vec3(0.0f, -0.001f, 0.0f);
	vel *= 0.99f;
}
