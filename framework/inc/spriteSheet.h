#pragma once
#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>



class OpenGLRenderer;
class Camera;

class SpriteSheet
{
public:

	SpriteSheet() {};
	~SpriteSheet() {};

	void InitRenderProgram(OpenGLRenderer* openGLRenderer);
	void LoadSpriteSheet(OpenGLRenderer* openGLRenderer, const char* path, int spritePixelSize);

	void RenderSprite(OpenGLRenderer* openGLRenderer, glm::mat4 mvp, int index);


	int spritePixelSize = 0;
	int sheetPixelWidth = 0;
	int sheetPixelHeight = 0;

	int numberColumns = 0;
	int numberRows = 0;

	GLuint GLProgram = 0;
	GLuint GLTexture = 0;
};