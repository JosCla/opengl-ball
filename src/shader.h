#ifndef SHADER_H
#define SHADER_H
// shader.h
// Defines the Shader class, used for compiling and using shader code.

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// stdlib stuff
#include <string> // for string
#include <fstream> // for ifstream
#include <sstream> // for stringstream
#include <iostream> // for cin, cout, endl

class Shader
{
public:
  unsigned int ID;

  Shader(const char*, const char*);

  void use();

  void setBool(const std::string&, bool) const;
  void setInt(const std::string&, int) const;
  void setFloat(const std::string&, float) const;
  void setVec3(const std::string&, const glm::vec3&) const;
  void setMat4(const std::string&, const glm::mat4&) const;
};

#endif
