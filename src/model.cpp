// model.cpp

#include <glad/glad.h> // opengl flags
#include <assimp/Importer.hpp> // assimp asset importer
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp> // gl maths
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// stdlib
#include <iostream> // for cin, cout, endl
#include <string> // for string
#include <vector> // for vector

// our headers
#include "shader.h" // for Shader class
#include "mesh.h" // for Mesh class
#include "shapes.h" // for Triangle class
#include "utils.h" // for utility functions
#include "model.h" // for Model declaration

// Model Constructor
Model::Model(const char *path)
{
	loadModel(path);
}

// Draws the Model (by calling the Draw function of child Meshes).
void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw(shader);
	}
}

// Converts the entire Model into Triangle shapes (for collision purposes).
std::vector<Triangle> Model::ToTriangles()
{
	std::vector<Triangle> ret;
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		for (unsigned int j = 0; j < meshes[i].indices.size(); j += 3)
		{
			glm::vec3 vertices[3];
			for (unsigned int k = 0; k < 3; ++k)
			{
				vertices[k] = meshes[i].vertices[meshes[i].indices[j+k]].Position;
			}
			ret.push_back(Triangle(vertices[0], vertices[1], vertices[2]));
		}
	}
	return ret;
}

// Loads a Model given a path.
void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

// Processes a node of a Model's scene.
void Model::processNode(aiNode *node, const aiScene *scene)
{
	// process node's meshes if there are any
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// then do so for child nodes
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}
}

// Processes a Mesh of a Model's node.
Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		// process vertex positions, normals, textures
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) // only do if mesh has texture coords
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// indices
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// textures
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

// Loads a material's textures.
std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); ++j)
		{
			if (textures_loaded[j].path == str.C_Str())
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture;
			texture.id = loadTexture(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}
