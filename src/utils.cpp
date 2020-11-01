// Utility Functions

// libraries
#include <glad/glad.h> // for opengl flags
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image.h> // for image processing
#include <stb/stb_image_write.h> // for screenshotting
#include <glm/glm.hpp> // gl maths
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// stdlib
#include <iostream> // for cout
#include <cstring> // for strlen
#include <cmath> // for pow

// generates an image given a width, a height, and a pointer to the image to store it in
void genImg(int width, int height, GLubyte *img) {
  int i, j, currPos;

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      currPos = ((i * width) + j) * 4;
      int color = ((i <= 1 || i >= height-2) || (j <= 1 || j >= width-2)) ? 75 : 150;
      img[currPos] = color;
      img[currPos+1] = color;
      img[currPos+2] = color;
      img[currPos+3] = 255;
    }
  }
}

// loads a texture at a given path, and returns an ID for referencing it later
unsigned int loadTexture(const char *path, const std::string& directory = ".")
{
	stbi_set_flip_vertically_on_load(true);

	std::string filename = std::string(path);
	filename = directory + '/' + filename;

  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrChannels;
  unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

  if (data)
  {
    GLenum format;
    if (nrChannels == 1)
      format = GL_RED;
    if (nrChannels == 3)
      format = GL_RGB;
    if (nrChannels == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else
  {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}

// code based on https://vallentin.dev/2013/09/02/opengl-screenshot
int saveScreenshot(const char *filename, const bool flipOnWrite = true)
{
  stbi_flip_vertically_on_write(flipOnWrite);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	int x0 = viewport[0];
	int y0 = viewport[1];
	int width = viewport[2];
	int height = viewport[3];

	char *data = (char*) malloc((size_t) (width * height * 3)); // (* 3 for rgb)

	if (!data)
		return 0;

	glPixelStorei(GL_PACK_ALIGNMENT, 1); // specifies that the start of a pixel row should align to the start of a byte
	glReadPixels(x0, y0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	int saved = stbi_write_png(filename, width, height, 3, data, 0);

	free(data); // freeing up memory

  return saved; // returns 0 if failure, 1 if success
}	

// code based on https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool RayIntersectsTriangle(glm::vec3 rayOrigin, glm::vec3 rayVector, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2, float &t0)
{
  const float EPSILON = 0.0000001;
	glm::vec3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = vert1 - vert0;
	edge2 = vert2 - vert0;

	h = glm::cross(rayVector, edge2);
	a = glm::dot(edge1, h); 
	if (a > -EPSILON && a < EPSILON)
		return false; // ray is parallel to the triangle
	
	f = 1.0 / a;
	s = rayOrigin - vert0;
	u = f * glm::dot(s, h);
	if (u < 0.0 || u > 1.0)
		return false; // too far towards/away from vert1
	
	q = glm::cross(s, edge1);
	v = f * glm::dot(rayVector, q);
	if (v < 0.0 || (u + v) > 1.0)
		return false; // too far towards/away from vert2

	// finally we compute the amount by which to scale rayVector to get intersection point
	float t = f * glm::dot(edge2, q);
	if (t >= 0)
	{
		t0 = t;
		return true;
	}
	else
	{
		return false; // intersection is behind ray (line intersection)
	}
}

// code based on https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
void LineIntersectsPlane(glm::vec3 rayOrigin, glm::vec3 rayVector, glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2, float &t0, float& u, float& v)
{
  const float EPSILON = 0.0000001;
	glm::vec3 edge1, edge2, h, s, q;
	float a, f;
	edge1 = vert1 - vert0;
	edge2 = vert2 - vert0;

	h = glm::cross(rayVector, edge2);
	a = glm::dot(edge1, h); 
	if (a > -EPSILON && a < EPSILON)
		return; // ray is parallel to the triangle
	
	f = 1.0 / a;
	s = rayOrigin - vert0;
	u = f * glm::dot(s, h);
	
	q = glm::cross(s, edge1);
	v = f * glm::dot(rayVector, q);

	// finally we compute the amount by which to scale rayVector to get intersection point
	float t = f * glm::dot(edge2, q);
	t0 = t;
}

// solves quadratic using quadratic formula. returns false if solution is complex.
template <typename Num>
bool solveQuadrat(Num a, Num b, Num c, Num& r0, Num& r1)
{
	Num discriminant = std::pow(b, 2) - (4 * a * c);
	if (discriminant < 0)
	{
		return false;
	}

	Num term1 = -b / (2 * a);
	Num term2 = std::sqrt(discriminant) / (2 * a);
	r0 = term1 + term2;
	r1 = term1 - term2;
	return true;
}
// instantiating the versions of solveQuadrat we'll need
template bool solveQuadrat<float>(float, float, float, float&, float&);
template bool solveQuadrat<double>(double, double, double, double&, double&);
