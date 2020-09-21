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
bool computeIntersection(const Ellipsoid &ellip, const Triangle &tri, float &collisionTime, glm::vec3 &collisionPoint)
{
	float EPSILON = 0.00001;
	float baseDistToPlane = signedDistanceToPlane(tri, ellip.Position);
	float normDotVel = glm::dot(tri.Normal, ellip.Velocity);

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
		glm::vec3 intersectionPoint = ellip.Position - tri.Normal + (t0 * ellip.Velocity);
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
			double a = glm::dot(ellip.Velocity, ellip.Velocity);
			double b = 2.0f * glm::dot(ellip.Velocity, (ellip.Position - tri.Vertices[i]));
			double c = std::pow(glm::length(tri.Vertices[i] - ellip.Position), 2) - 1.0f;

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
			glm::vec3 posToVertex = tri.Vertices[i] - ellip.Position;
			double edgeLenSquared = glm::length(currEdge) * glm::length(currEdge);
			double velLenSquared = glm::length(ellip.Velocity) * glm::length(ellip.Velocity);
			double posToVertLenSquared = glm::length(posToVertex) * glm::length(posToVertex);
			double edgeDotVel = glm::dot(currEdge, ellip.Velocity);
			double posToVertDotVel = glm::dot(posToVertex, ellip.Velocity);
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
				
				double relativePosOnEdge = ((float)time * glm::dot(currEdge, ellip.Velocity) - glm::dot(currEdge, posToVertex)) / std::pow(glm::length(currEdge), 2);

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
bool computeIntersection(const Ellipsoid &ellip, const glm::vec3 &vert0, const glm::vec3 &vert1, const glm::vec3 &vert2, float &collisionTime, glm::vec3 &collisionPoint)
{
	Triangle tri(vert0, vert1, vert2);
	return computeIntersection(ellip, tri, collisionTime, collisionPoint);
}
