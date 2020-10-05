// thing.cpp

// stdlib
#include <iostream> // for cout, etc.
#include <string> // for string
#include <vector> // for vector
using namespace std;

// libraries
#include <glm/gtc/type_ptr.hpp> // for glm types

// our files
#include "thing.h" // for Thing declaration
#include "model.h" // for Model class
#include "shapes.h" // for Ellipsoid class
#include "collision.h" // for collision utilities

// Thing Class 
// TODO: add another argument to constructor as the path to the model
// TODO: add some way of initializing Hitbox
Thing::Thing(glm::vec3 position, glm::vec3 velocity, string name) : Hitbox(1.0, glm::vec3(0.0), glm::vec3(0.0)) {
	Position = position;
	Velocity = velocity;
	Name = name;
	//Hitbox = Ellipsoid(glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0));
}

// Computes the movement of the Thing over a single frame
void Thing::PassFrame(vector<Triangle>& tris) {
	// Handling intersections with terrain
	handleIntersection(Hitbox, tris, Position, Velocity);

	// Adding gravity and friction after handling intersections
	Velocity += glm::vec3(0.0, -0.001, 0.0);
	Velocity *= 0.99;
}

void Thing::RenderThing() {
	cout << "hey!!! not finished!!! please finish!!!" << endl;
}
