#ifndef LIGHT_H
#define LIGHT_H
// light.h
// Defines the Light class, used for easily creating and positioning lights.

#include <glm/glm.hpp> // gl maths
#include <glm/gtc/type_ptr.hpp>

// stdlib
#include <string> // for string, to_string

// our files
#include "shader.h" // for shader class

const glm::vec3 POSITION = glm::vec3(0.0f);
const glm::vec3 LIGHTDIRECTION = glm::vec3(0.0f, -1.0f, 0.0f);

const float CUTOFF = glm::cos(glm::radians(18.0f));
const float OUTERCUTOFF = glm::cos(glm::radians(25.0f));

const glm::vec3 AMBIENT = glm::vec3(1.0f);
const glm::vec3 DIFFUSE = glm::vec3(1.0f);
const glm::vec3 SPECULAR = glm::vec3(1.0f);

const float CONSTANT = 1.0f;
const float LINEAR = 0.09f;
const float QUADRATIC = 0.032f;

const bool ISON = true;

class PointLight
{
	public:
		glm::vec3 Position;

		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;

		float Constant;
		float Linear;
		float Quadratic;

		bool IsOn;

		PointLight(glm::vec3 = POSITION, glm::vec3 = AMBIENT, glm::vec3 = DIFFUSE, glm::vec3 = SPECULAR, float = CONSTANT, float = LINEAR, float = QUADRATIC, bool = ISON);

    void AddLight(Shader&, int, std::string); 
    void AddLight(Shader&, std::string); 
};

class DirectionalLight
{
  public:
    glm::vec3 Direction;

    glm::vec3 Ambient; 
    glm::vec3 Diffuse;
    glm::vec3 Specular;

		bool IsOn;

		DirectionalLight(glm::vec3 = LIGHTDIRECTION, glm::vec3 = AMBIENT, glm::vec3 = DIFFUSE, glm::vec3 = SPECULAR, bool = ISON);
		
    void AddLight(Shader&, int, std::string); 
    void AddLight(Shader&, std::string);
};

class SpotLight
{
  public:
    glm::vec3 Position;
    glm::vec3 Direction;

		float CutOff;
		float OuterCutOff;

    glm::vec3 Ambient; 
    glm::vec3 Diffuse;
    glm::vec3 Specular;

		float Constant;
		float Linear;
		float Quadratic;

		bool IsOn;

		SpotLight(glm::vec3 = POSITION, glm::vec3 = LIGHTDIRECTION, float = CUTOFF, float = OUTERCUTOFF, glm::vec3 = AMBIENT, glm::vec3 = DIFFUSE, glm::vec3 = SPECULAR, float = CONSTANT, float = LINEAR, float = QUADRATIC, bool = ISON);
	 
    void AddLight(Shader&, int, std::string);
    void AddLight(Shader&, std::string); 
};

#endif
