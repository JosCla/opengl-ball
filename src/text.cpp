// text.cpp

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
#include "utils.h" // for utility functions
#include "shader.h" // for Shader class
#include "text.h" // for Text declaration

// Text Constructor
Text::Text(std::string str, unsigned int scrWidth, unsigned int scrHeight, unsigned int xMin, unsigned int yMin, unsigned int maxWidth, unsigned int charSize, unsigned int padding)
{
	Str = str;
	ScrWidth = scrWidth;
	ScrHeight = scrHeight;
	XMin = xMin;
	YMin = yMin;
	MaxWidth = maxWidth;
	CharSize = charSize;
	Padding = padding;

	NumChars = SetupText();

	TextMaterial = loadTexture("textures/text.png");
}

// Text Destructor
Text::~Text()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

// Draws the Text to the screen.
void Text::DrawText(Shader shader)
{
	shader.use();
	shader.setInt("textMaterial", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextMaterial);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (NumChars + 1) * 6, GL_UNSIGNED_INT, 0);
}

// Sets the Text to having a new string.
void Text::SetText(std::string str)
{
	Str = str;
	NumChars = SetupText();
}

// Sets the position of the Text.
void Text::SetPosition(unsigned int xMin, unsigned int yMin)
{
	XMin = xMin;
	YMin = yMin;
	NumChars = SetupText();
}

// Sets the width of the Text.
void Text::SetWidth(unsigned int maxWidth)
{
	MaxWidth = maxWidth;
	NumChars = SetupText();
}

// Sets the screen size.
void Text::SetScreenSize(unsigned int scrWidth, unsigned int scrHeight)
{
	ScrWidth = scrWidth;
	ScrHeight = scrHeight;
	NumChars = SetupText();
}

// Sets the character size of the Text.
void Text::SetCharSize(unsigned int charSize)
{
	CharSize = charSize;
	NumChars = SetupText();
}

// Sets the padding of the Text.
void Text::SetPadding(unsigned int padding)
{
	Padding = padding;
	NumChars = SetupText();
}

// Sets up the text's buffers
std::string::size_type Text::SetupText()
{
	const unsigned int charsPerRow = MaxWidth / CharSize;
	const float charWidth = 2.0f / (ScrWidth / CharSize);
	const float charHeight = charWidth * (float)ScrWidth / (float)ScrHeight; 
	const float xLeft = ((float)XMin / (float)ScrWidth) * 2.0f - 1.0f;
	const float yTop = ((float)YMin / (float)ScrHeight) * -2.0f + 1.0f;
	const float paddingX = 2.0f / (ScrWidth / Padding);
	const float paddingY = 2.0f / (ScrHeight / Padding);

	const std::vector<std::string> lines = splitToLine(Str, charsPerRow);
	std::string::size_type numChars = 0;
	for (unsigned int i = 0; i < lines.size(); ++i)
	{
		numChars += lines[i].length();
	}

	float vertices[(numChars+1) * 4 * 4]; // 4 vertices per char * 4 floats per vertex (+ 1 for bg) 
	unsigned int indices[(numChars+1) * 6]; // 6 indices per char (+ 1 for bg) 
	
	int currChar = 0; // for tracking position in vertices/indices

	for (unsigned int i = 0; i < lines.size(); ++i)
	{
		for (unsigned int j = 0; j < lines[i].length(); ++j)
		{
			int charCode = 0; // space/empty character by default
			if (int(lines[i][j]) >= int(' ') && int(lines[i][j]) <= int('~'))
			{
				// char
				charCode = int(lines[i][j]) - int(' ');
			}

			int offsetVertices = currChar * 4 * 4; // 16 floats per char

	  	// inserting vertices
			float verticesToAdd[16];

			makeLetter(verticesToAdd, (float)j * charWidth + xLeft, -1.0f * (i * charHeight) + yTop, charWidth, charHeight, charCode);
			zipArr(verticesToAdd, 16, vertices, offsetVertices);

			currChar++;
		}
	}

	// also getting the background in there
	float verticesToAdd[16];
	makeLetter(verticesToAdd, xLeft - paddingX, yTop + paddingY, charsPerRow * charWidth + (2 * paddingX), lines.size() * charHeight + (2 * paddingY), 96);
	zipArr(verticesToAdd, 16, vertices, numChars * 4 * 4);

	// inserting indices
	for (unsigned int i = 0; i < (numChars+1); i++)
	{
	  unsigned int indicesToAdd[6] = {0, 1, 2, 1, 2, 3};
		for (int j = 0; j < 6; ++j)
		{
			indicesToAdd[j] += (i * 4);
		}
		zipArr(indicesToAdd, 6, indices, i * 6);
	}

	// putting the stuff into the arrays/buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// unbinding the arrays/buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// returning numChars
	return numChars;
}

// Makes the proper VBO for a single letter.
void Text::makeLetter(float (&ret)[16], float x, float y, float width, float height, int charCode)
{
  for (int j = 0; j < 2; ++j)
	{
	  for (int k = 0; k < 2; ++k)
	  {
	  	// making the vertex coordinates and texture coordinates for each vertex
	  	ret[4 * (2 * j + k)] = x + (float)(k * width); // posX
			ret[4 * (2 * j + k) + 1] = y - (float)(j * height); // posY
			ret[4 * (2 * j + k) + 2] = 0.01f * (float)(charCode + k); // texX
			ret[4 * (2 * j + k) + 3] = 1.0f - (float)j; // texY
	  }
  }
}

// Splits a string into lines.
std::vector<std::string> Text::splitToLine(const std::string& str, unsigned int charsPerLine)
{
	std::vector<std::string> ret;

	std::vector<std::string> split_linebreak = split(str, '\n');
	
	unsigned int currLine = 0;
	for (unsigned int n = 0; n < split_linebreak.size(); ++n)
	{
		std::vector<std::string> words = split(split_linebreak[n], ' ');

		unsigned int tempIndex = 0; // for words so long that they take up entire lines
		ret.push_back("");
		for (unsigned int i = 0; i < words.size(); ++i)
		{
  	  if (ret[currLine].length() + words[i].length() > charsPerLine)
			{
				// if the next word will overflow into another line
				if (ret[currLine].length() > 0)
				{
					// accounts for scenario where first word is longer than a line
				  currLine++;
					ret.push_back("");
				}
				if (words[i].length() < charsPerLine)
				{
					// if the next word isn't longer than a line
					ret[currLine] += words[i];
				}
				else
				{
					// if the next word is longer than a line
					ret[currLine] += words[i].substr(tempIndex, charsPerLine);
					tempIndex += charsPerLine;
					if (tempIndex < words[i].length())
					{
					  --i;
					}
					else
					{
						tempIndex = 0;
					}
				}
			}
			else
			{
				// if the next word can be added to line without issue
				ret[currLine] += words[i];
			}

			// adding a space if necessary
			if (ret[currLine].length() < charsPerLine)
				ret[currLine] += " ";
		}

		currLine++; // incrementing line count for newline char
	}

	return ret;
}

// Splits a string across a single character
// code based on https://stackoverflow.com/questions/5167625/splitting-a-c-stdstring-using-tokens-e-g 
std::vector<std::string> Text::split(const std::string& str, char sep)
{
  std::vector<std::string> ret;

	std::string::size_type prev = 0;
	std::string::size_type next = 0;

	while((next = str.find(sep, next)) != std::string::npos)
	{
    ret.push_back(str.substr(prev, next - prev));
		prev = ++next;
	}
	
	ret.push_back(str.substr(prev, next - prev));

	return ret;
}

// "Zips" arr1 into arr2 starting at a specified index
template <typename T>
void Text::zipArr(T *arr1, int lenArr1, T *arr2, int index)
{
	for (int i = 0; i < lenArr1; ++i)
	{
		arr2[index + i] = arr1[i];
	}
}

