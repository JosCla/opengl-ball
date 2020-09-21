#ifndef MESH_H
#define MESH_H
// mesh.h
// Defines the Mesh class, which is a smaller portion of a model to render.

// stdlib
#include <string> // for string
#include <vector> // for vector

// our headers
#include "shader.h" // for Shader class

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		Mesh(std::vector<Vertex>&, std::vector<unsigned int>&, std::vector<Texture>&);
		Mesh(const Mesh&);
		~Mesh();

		void Draw(Shader&);
		
	private:
		unsigned int VAO, VBO, EBO;

		void SetupMesh();
};

#endif
