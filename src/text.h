#ifndef TEXT_H
#define TEXT_H
// text.h
// Defines the Text class, used for displaying text to the screen.

// libraries
#include <glad/glad.h> // for opengl flags
#include <glm/glm.hpp> // for gl maths
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// stdlib
#include <iostream> // for cout
#include <string> // for string
#include <vector> // for vector

// our files
#include "utils.h"
#include "shader.h"

class Text
{
  public:
		Text(std::string, unsigned int, unsigned int, unsigned int = 30, unsigned int = 30, unsigned int = 400, unsigned int = 20, unsigned int = 5);
		~Text();

		void DrawText(Shader shader);
		void SetText(std::string str);
		void SetPosition(unsigned int, unsigned int);
		void SetWidth(unsigned int);
		void SetScreenSize(unsigned int, unsigned int);
		void SetCharSize(unsigned int);
		void SetPadding(unsigned int);
				
	private:
		unsigned int VAO, VBO, EBO;
		std::string Str;
		std::string::size_type NumChars;
		unsigned int ScrWidth, ScrHeight;
		unsigned int XMin, YMin, MaxWidth;
		unsigned int CharSize, Padding;
		unsigned int TextMaterial;

		std::string::size_type SetupText();
		void makeLetter(float (&)[16], float, float, float, float, int);
		std::vector<std::string> splitToLine(const std::string&, unsigned int);
		std::vector<std::string> split(const std::string&, char);
		template <typename T> void zipArr(T*, int, T*, int);
};

#endif
