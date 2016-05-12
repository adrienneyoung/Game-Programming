//Functions needed to display graphics (textures and text)

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include <math.h>
#include "Pair.h"

using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

GLuint LoadTexture(const char *image_path);

void DrawText(ShaderProgram *program, Matrix& matrix, int fontTexture, std::string text, float size, float spacing, float x, float y);

class SheetSprite{
public:
	SheetSprite();
	SheetSprite(unsigned int texID, int spriteCountX, int spriteCountY, float width, float height, float size);
	void Draw(ShaderProgram* program, Matrix& matrix, int index, float x, float y);

	float size;
	unsigned int texID;
	int spriteCountX;
	int spriteCountY;
	float width;
	float height;
};

float lerp(float v0, float v1, float t);
