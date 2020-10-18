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
void Triangle::Print() const
{
	for (unsigned int i = 0; i < 3; ++i)
	{
		std::cout << "(" << Vertices[i].x << ", " << Vertices[i].y << ", " << Vertices[i].z << ")";
		if (i < 2)
		{
			std::cout << ", ";
		}
	}
}

// Ellipsoid Class
///////////////////////////////////////////////////////////////////////////////////////////
Ellipsoid::Ellipsoid(glm::vec3 radii, glm::vec3 position, glm::vec3 velocity)
{
	Radii = radii;
	Position = position;
	Velocity = velocity;
}

// Prints the Ellipsoid to std::cout
void Ellipsoid::Print() const 
{
	std::cout << "Radii: " << Radii[0] << ", " << Radii[1] << ", " << Radii[2] << " | ";
	std::cout << "Position: (" << Position[0] << ", " << Position[1] << ", " << Position[2] << ") | ";
	std::cout << "Velocity: (" << Velocity[0] << ", " << Velocity[1] << ", " << Velocity[2] << ")";
}

// Converts a 3D point into Ellipsoid space - a space squished the proper amount in each dimension to make the ellipsoid into a unit sphere
glm::vec3 Ellipsoid::toEllipSpace(const glm::vec3 &point) const {
	return point / Radii;
}

// Converts a 3D point back from Ellipsoid space
glm::vec3 Ellipsoid::fromEllipSpace(const glm::vec3 &point) const {
	return point * Radii;
}

// Converts a Triangle into Ellipsoid space
Triangle Ellipsoid::toEllipSpace(const Triangle &tri) const {
	return Triangle(toEllipSpace(tri.Vertices[0]), toEllipSpace(tri.Vertices[1]), toEllipSpace(tri.Vertices[2]));
}

// Converts a Triangle from Ellipsoid space
Triangle Ellipsoid::fromEllipSpace(const Triangle &tri) const {
	return Triangle(fromEllipSpace(tri.Vertices[0]), fromEllipSpace(tri.Vertices[1]), fromEllipSpace(tri.Vertices[2]));
}
