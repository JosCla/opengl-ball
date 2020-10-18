// thing.cpp

// stdlib
#include <iostream> // for cout, etc.
#include <string> // for string
#include <vector> // for vector
using namespace std;

// libraries
#include <glm/glm.hpp> // gl maths
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// our files
#include "thing.h" // for Thing declaration
#include "model.h" // for Model class
#include "shapes.h" // for Ellipsoid class
#include "collision.h" // for collision utilities
#include "camera.h" // for Camera class
#include "shader.h" // for Shader class

// Thing Class 
Thing::Thing(glm::vec3 position, glm::vec3 velocity, glm::vec3 scale, glm::vec3 radii, string modelFilepath, string name) : Hitbox(radii, glm::vec3(0.0f), glm::vec3(0.0f)), ThingModel(modelFilepath.c_str()) {
	Position = position;
	Velocity = velocity;
	Scale = scale;

	Name = name;
}

// Computes the movement of the Thing over a single frame
void Thing::PassFrame(vector<Triangle>& tris) {
	// Handling intersections with terrain
	Hitbox.Position = Position;
	Hitbox.Velocity = Velocity;
	handleIntersection(Hitbox, tris);
	Position = Hitbox.Position;
	Velocity = Hitbox.Velocity;

	// Adding gravity and friction after handling intersections
	Velocity += glm::vec3(0.0f, -0.001f, 0.0f);
	Velocity *= 0.99f;
}

// Renders the Thing
void Thing::RenderThing(Camera &camera, Shader &shader, int SCR_WIDTH, int SCR_HEIGHT) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Position);
	model = glm::scale(model, Scale);

	glm::mat4 view = camera.GetViewMatrix();

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	shader.use();
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	ThingModel.Draw(shader);
}

// Prints the thing
void Thing::Print() const {
	cout << "Thing " << Name << ": Position (" << Position.x << ", " << Position.y << ", " << Position.z << ")" << endl;
}
