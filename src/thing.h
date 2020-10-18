#ifndef THING_H
#define THING_H
// thing.h
// Defines the Thing class, which is a general container for physical objects.

// stdlib
#include <string> // for string
#include <vector> // for vector

// libraries
#include <glm/gtc/type_ptr.hpp> // for glm types

// our files
#include "model.h" // for Model class
#include "shapes.h" // for Ellipsoid and Triangle class 
#include "camera.h" // for Camera class
#include "shader.h" // for Shader class

// Thing class
class Thing {
	public:
		glm::vec3 Position;
		glm::vec3 Velocity;
		glm::vec3 Scale;
		std::string Name;

		Ellipsoid Hitbox;
		Model ThingModel;
		
		Thing(glm::vec3, glm::vec3, glm::vec3, glm::vec3, std::string, std::string="");

		void PassFrame(std::vector<Triangle>&);
		void RenderThing(Camera&, Shader&, int, int);
		
		void Print() const;
};

#endif
