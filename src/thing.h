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

// Thing class
class Thing {
	public:
		glm::vec3 Position;
		glm::vec3 Velocity;
		std::string Name;
		
		Ellipsoid Hitbox;
		//Model ThingModel;

		Thing(glm::vec3, glm::vec3, std::string="");

		void PassFrame(std::vector<Triangle>&);
		void RenderThing();
};

#endif
