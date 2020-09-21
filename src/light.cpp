// light.cpp

#include <glad/glad.h> // opengl tags
#include <glm/glm.hpp> // gl maths
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// stdlib
#include <string> // for string, to_string

// our files
#include "shader.h" // for Shader class
#include "light.h" // for Light declaration

// Point Light Constructor
PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, bool isOn)
{
	Position = position;
	Ambient = ambient;
	Diffuse = diffuse;
	Specular = specular;
	Constant = constant;
	Linear = linear;
	Quadratic = quadratic;
	IsOn = isOn;
}

// Adds Light to Shader, in an array of Lights
void PointLight::AddLight(Shader &shader, int index_int, std::string uniformName) 
{
	std::string index = std::to_string(index_int);
	shader.use();

	shader.setVec3(uniformName + "[" + index + "].position", Position);
	shader.setVec3(uniformName + "[" + index + "].ambient", Ambient);
	shader.setVec3(uniformName + "[" + index + "].diffuse", Diffuse);
	shader.setVec3(uniformName + "[" + index + "].specular", Specular);
	shader.setFloat(uniformName + "[" + index + "].constant", Constant);
	shader.setFloat(uniformName + "[" + index + "].linear", Linear);
	shader.setFloat(uniformName + "[" + index + "].quadratic", Quadratic);
	shader.setBool(uniformName + "[" + index + "].isOn", IsOn);
}

// Adds Light to Shader, not in an array of Lights
void PointLight::AddLight(Shader &shader, std::string uniformName) 
{
	shader.use();

	shader.setVec3(uniformName + ".position", Position);
	shader.setVec3(uniformName + ".ambient", Ambient);
	shader.setVec3(uniformName + ".diffuse", Diffuse);
	shader.setVec3(uniformName + ".specular", Specular);
	shader.setFloat(uniformName + ".constant", Constant);
	shader.setFloat(uniformName + ".linear", Linear);
	shader.setFloat(uniformName + ".quadratic", Quadratic);
	shader.setBool(uniformName + ".isOn", IsOn);
}

// Directional Light Constructor
DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool isOn)
{
	Direction = direction;
	Ambient = ambient;
	Diffuse = diffuse;
	Specular = specular;
	IsOn = isOn;
}

// Adds Light to Shader, in an array of Lights
void DirectionalLight::AddLight(Shader &shader, int index_int, std::string uniformName) 
{
	std::string index = std::to_string(index_int);
	shader.use();

	shader.setVec3(uniformName + "[" + index + "].direction", Direction);
	shader.setVec3(uniformName + "[" + index + "].ambient", Ambient);
	shader.setVec3(uniformName + "[" + index + "].diffuse", Diffuse);
	shader.setVec3(uniformName + "[" + index + "].specular", Specular);
	shader.setBool(uniformName + "[" + index + "].isOn", IsOn);
}

// Adds Light to Shader, not in an array of Lights
void DirectionalLight::AddLight(Shader &shader, std::string uniformName) 
{
	shader.use();

	shader.setVec3(uniformName + ".direction", Direction);
	shader.setVec3(uniformName + ".ambient", Ambient);
	shader.setVec3(uniformName + ".diffuse", Diffuse);
	shader.setVec3(uniformName + ".specular", Specular);
	shader.setBool(uniformName + ".isOn", IsOn);
}

// SpotLight Constructor
SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, bool isOn) 
{
	Position = position;
	Direction = direction;
	CutOff = cutOff;
	OuterCutOff = outerCutOff;
	Ambient = ambient;
	Diffuse = diffuse;
	Specular = specular;
	Constant = constant;
	Linear = linear;
	Quadratic = quadratic;
	IsOn = isOn;
}

// Adds Light to Shader, in an array of Lights
void SpotLight::AddLight(Shader &shader, int index_int, std::string uniformName) 
{
	std::string index = std::to_string(index_int);
	shader.use();

	shader.setVec3(uniformName + "[" + index + "].position", Position);
	shader.setVec3(uniformName + "[" + index + "].direction", Direction);
	shader.setFloat(uniformName + "[" + index + "].cutOff", CutOff);
	shader.setFloat(uniformName + "[" + index + "].outerCutOff", OuterCutOff);
	shader.setVec3(uniformName + "[" + index + "].ambient", Ambient);
	shader.setVec3(uniformName + "[" + index + "].diffuse", Diffuse);
	shader.setVec3(uniformName + "[" + index + "].specular", Specular);
	shader.setFloat(uniformName + "[" + index + "].constant", Constant);
	shader.setFloat(uniformName + "[" + index + "].linear", Linear);
	shader.setFloat(uniformName + "[" + index + "].quadratic", Quadratic);
	shader.setBool(uniformName + "[" + index + "].isOn", IsOn);
}

// Adds Light to Shader, not in an array of Lights

void SpotLight::AddLight(Shader &shader, std::string uniformName) 
{
	shader.use();

	shader.setVec3(uniformName + ".position", Position);
	shader.setVec3(uniformName + ".direction", Direction);
	shader.setFloat(uniformName + ".cutOff", CutOff);
	shader.setFloat(uniformName + ".outerCutOff", OuterCutOff);
	shader.setVec3(uniformName + ".ambient", Ambient);
	shader.setVec3(uniformName + ".diffuse", Diffuse);
	shader.setVec3(uniformName + ".specular", Specular);
	shader.setFloat(uniformName + ".constant", Constant);
	shader.setFloat(uniformName + ".linear", Linear);
	shader.setFloat(uniformName + ".quadratic", Quadratic);
	shader.setBool(uniformName + ".isOn", IsOn);
}
