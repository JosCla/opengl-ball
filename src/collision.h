#ifndef COLLISION_H
#define COLLISION_H

// stdlib
#include <vector>

// libraries
#include <glm/gtc/type_ptr.hpp> // for glm types

// our files
#include "shapes.h"

float signedDistanceToPlane(const Triangle&, const glm::vec3&);
bool pointInsideTriangle(const Triangle&, const glm::vec3&);
bool computeIntersection(const Ellipsoid&, const glm::vec3&, const glm::vec3&, const Triangle&, float&, glm::vec3&);
bool computeIntersection(const Ellipsoid&, const glm::vec3&, const glm::vec3&, const glm::vec3&, const glm::vec3&, const glm::vec3&, float&, glm::vec3&); 
void handleIntersection(const Ellipsoid&, const std::vector<Triangle>&, glm::vec3&, glm::vec3&); 

#endif
