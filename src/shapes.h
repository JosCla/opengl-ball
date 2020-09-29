#ifndef SHAPES_H
#define SHAPES_H
// shapes.h
// Defines numerous shape classes, used for collision handling.

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

// Triangle class. Used for level geometry
class Triangle
{
	public:
		glm::vec3 Vertices[3];
		glm::vec3 Normal;

		Triangle(glm::vec3, glm::vec3, glm::vec3);
		Triangle(glm::vec3, glm::vec3, glm::vec3, glm::vec3);

		void PrintTriangle();
};

// Ellipsoid class. Used for objects which collide with level geometry
// TODO: Currently treated as a sphere. Will change that at some point
class Ellipsoid
{
	public:
		float Radius;
		glm::vec3 Position;
		glm::vec3 Velocity;

		Ellipsoid(float, glm::vec3, glm::vec3);

		void PassFrame(std::vector<Triangle>&);
};

#endif