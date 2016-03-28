#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return textureID;
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Akari", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	SDL_Event event;
	bool done = false;

	glViewport(0, 0, 640, 360);
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(program.programID);

	Matrix projectionMatrix;
	Matrix modelMatrixAkane;
	Matrix modelMatrixBun1;
	Matrix modelMatrixBun2;
	Matrix modelMatrixAkari;
	Matrix viewMatrix;
	projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);

	GLuint akaneTex = LoadTexture("akane.png");
	GLuint bun1Tex = LoadTexture("bun.png");
	GLuint bun2Tex = LoadTexture("bun.png");
	GLuint akariTex = LoadTexture("akari2.png");

	//float bun1[] = { -3.0f, -2.0f, -1.0f, -2.0f, -1.0f, -0.5f, -3.0f, -2.0f, -1.0f, -0.5f, -3.0f, -0.5f };
	//float bun1[] = { 1.0f, 0.0f, 3.0f, 0.0f, 3.0f, 1.0f, 1.0f, 0.0f, 3.0f, 1.0f, 1.0f, 1.0f };
	//float bun1[] = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f }; 
	//float bun1[] = { -2.0f, -2.0f, 0.0f, -2.0f, 0.0f, 0.0f, -2.0f, -2.0f, 0.0f, 0.0f, -2.0f, 0.0f };
	//float bun1[] = { -3.0f, 0.0f, -1.0f, 0.0f, -1.0f, 2.0f, -3.0f, 0.0f, -1.0f, 2.0f, -3.0f, 2.0f };

	float akane[] = { -4.5f, -1.5f, -0.5f, -1.5f, -0.5f, 1.5f, -4.5f, -1.5f, -0.5f, 1.5f, -4.5f, 1.5f };
	float bun1[] = { 1.5f, 0.0f, 3.5f, 0.0f, 3.5f, 2.0f, 1.5f, 0.0f, 3.5f, 2.0f, 1.5f, 2.0f };
	float bun2[] = { -1.5f, 0.0f, -1.5f, 2.0f, -3.5f, 2.0f, -1.5f, 0.0f, -3.5f, 2.0f, -3.5f, 0.0f };
	float akari[] = { -1.5f, -3.0f, 1.5f, -3.0f, 1.5f, -1.0f, -1.5f, -3.0f, 1.5f, -1.0f, -1.5f, -1.0f };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	float lastFrameTicks = 0.0f;
	//position of y
	float yPos = 0.0f; 

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		glClearColor(0.8f, 0.7f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		program.setModelMatrix(modelMatrixAkane);
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		//Akane
		glBindTexture(GL_TEXTURE_2D, akaneTex);
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, akane);
		glEnableVertexAttribArray(program.positionAttribute);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Bun1
		program.setModelMatrix(modelMatrixBun1);
		glBindTexture(GL_TEXTURE_2D, bun1Tex);
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, bun1);
		glEnableVertexAttribArray(program.positionAttribute);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//translate center of object to origin
		modelMatrixBun1.Translate(2.5, 1.0, 0.0);
		//rotate counter-clockwise
		modelMatrixBun1.Rotate(elapsed * 5);
		//translate back to original position
		modelMatrixBun1.Translate(-2.5, -1.0, 0.0);

		//Bun2
		program.setModelMatrix(modelMatrixBun2);
		glBindTexture(GL_TEXTURE_2D, bun2Tex);
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, bun2);
		glEnableVertexAttribArray(program.positionAttribute);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//translate center of object to origin
		modelMatrixBun2.Translate(-2.5, 1.0, 0.0);
		//rotate clockwise
		modelMatrixBun2.Rotate(-elapsed*5);
		//translate back to original position
		modelMatrixBun2.Translate(2.5, -1.0, 0.0);

		//Akari
		program.setModelMatrix(modelMatrixAkari);
		glBindTexture(GL_TEXTURE_2D, akariTex);
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, akari);
		glEnableVertexAttribArray(program.positionAttribute);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		modelMatrixAkari.Translate(0.0, elapsed, 0.0);

		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
