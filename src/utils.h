#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>

void genImg(int, int, GLubyte*);
unsigned int loadTexture(const char*, const std::string& = ".");
int saveScreenshot(const char*, const bool = true);
bool RayIntersectsTriangle(glm::vec3, glm::vec3, glm::vec3, glm::vec3, glm::vec3, float&);
void LineIntersectsPlane(glm::vec3, glm::vec3, glm::vec3, glm::vec3, glm::vec3, float&, float&, float&);
template <typename Num>
bool solveQuadrat(Num, Num, Num, Num&, Num&);

#endif
