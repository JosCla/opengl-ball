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
void Triangle::Print()
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

void Ellipsoid::Print() 
{
	std::cout << "Radius: " << Radius << std::endl;
}
