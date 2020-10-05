// player.cpp

// libraries
#include <glm/gtc/type_ptr.hpp> // type pointer

// stdlib
#include <iostream> // for cout

// our files
#include <camera.h> // for Camera class
#include <shapes.h> // for Ellipsoid class

class Player
{
	// constructor
	// get/set for all attributes
	// pass frame function
	// render player?
	public:
		Player(glm::vec3 position, glm::vec3 direction) {
			this.Position = position;
			this.Direction = direction;
		}

		void PassFrame() {
			std::cout << "bruh moment" << endl;
		}

	// position, direction, velocity
	// camera
	// hitbox
	// model?
	private:
		glm::vec3 Position;
		glm::vec3 Direction;
		glm::vec3 Velocity;

		Camera PlayerCamera;
		Ellipsoid PlayerHitbox;
};
