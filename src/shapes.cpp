// shapes.cpp

// libraries
#include <glad/glad.h> // for opengl flags
#include <glm/glm.hpp> // for gl maths
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// stdlib
#include <iostream> // for cout
#include <cmath> // for abs
#include <algorithm> // for min, max
#include <vector> // for vector

// our files
#include "collision.h" // for collision detection

// Triangle Class
///////////////////////////////////////////////////////////////////////////////////////////
// Triangle Constructor
Triangle::Triangle(glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2)
{
	Vertices[0] = vert0;
	Vertices[1] = vert1;
	Vertices[2] = vert2;
	Normal = glm::normalize(glm::cross(vert1 - vert0, vert2 - vert0));
}
Triangle::Triangle(glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2, glm::vec3 normal)
{
	Vertices[0] = vert0;
	Vertices[1] = vert1;
	Vertices[2] = vert2;
	Normal = glm::normalize(normal);
}

// Prints the Triangle's vertices to std::cout
void Triangle::PrintTriangle()
{
	for (unsigned int i = 0; i < 3; ++i)
	{
		std::cout << "(" << Vertices[i].x << ", " << Vertices[i].y << ", " << Vertices[i].z << ")";
		if (i < 2)
		{
			std::cout << ", ";
		}
		else
		{
			std::cout << std::endl;
		}
	}
}

// Ellipsoid Class
///////////////////////////////////////////////////////////////////////////////////////////
Ellipsoid::Ellipsoid(float radius, glm::vec3 position, glm::vec3 velocity)
{
	Radius = radius;
	Position = position;
	Velocity = velocity;
}

// Moves the Ellipsoid by one frame
void Ellipsoid::PassFrame(std::vector<Triangle> &tris)
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
			if (computeIntersection(*this, tris[i], currCollisionTime, currCollisionPoint))
			{
				// the second part of the following if-statement makes sure that the ellipsoid
				// will not get stuck repeatedly colliding with something at time = 0
				if (currCollisionTime < collisionTime && std::abs(glm::dot(Velocity, Position + (Velocity * currCollisionTime) - currCollisionPoint)) > EPSILON)
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
			Position += (Velocity * collisionTime);

			glm::vec3 slidingPlaneNormal = Position - collisionPoint;

			glm::vec3 projection = glm::dot(Velocity, glm::normalize(slidingPlaneNormal)) * glm::normalize(slidingPlaneNormal);
			Velocity -= projection;
		}
		else
		{
			collisionTime = 1.0f;

			Position += (Velocity * (1.0f - totalTimePassed));
		}

		totalTimePassed += collisionTime;

		if (++passes >= 3)
			break; // prevents hard-locking
	}

	// adding gravity and friction to velocity post-calculations
	Velocity += glm::vec3(0.0f, -0.001f, 0.0f);
	Velocity *= 0.99f;
}
