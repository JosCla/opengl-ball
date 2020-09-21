#ifndef MODEL_H
#define MODEL_H
// model.h
// Defines the model class, which is used for loading scenes from external files.

#include <assimp/Importer.hpp> // assimp asset importer
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp> // gl maths
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// stdlib
#include <vector> // for vector

// our headers
#include "shader.h" // for Shader class
#include "mesh.h" // for Mesh class
#include "shapes.h" // for Triangle class

class Model
{
	public:
		Model(const char*);
		
		void Draw(Shader&);
		std::vector<Triangle> ToTriangles();

	private:
		std::vector<Mesh> meshes;
		std::string directory;
		std::vector<Texture> textures_loaded;

		void loadModel(std::string);
		void processNode(aiNode*, const aiScene*);
		Mesh processMesh(aiMesh*, const aiScene*);
		std::vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, std::string);
};

#endif
