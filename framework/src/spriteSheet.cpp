#include "spriteSheet.h"
#include "openGLRenderer.h"
#include "fileLoader.h"
#include <basicComponents.h>
#include <glm/gtc/type_ptr.hpp>

void SpriteSheet::InitRenderProgram(OpenGLRenderer* openGLRenderer)
{
	auto tvs = openGLRenderer->fileLoader->LoadTextFileInString("assets/shaders/tilemapVertex.glsl");
	auto tfs = openGLRenderer->fileLoader->LoadTextFileInString("assets/shaders/tilemapFragment.glsl");
	GLProgram = openGLRenderer->CreateOpenGLProgram(tvs.c_str(), tfs.c_str());
}


void SpriteSheet::LoadSpriteSheet(OpenGLRenderer* openGLRenderer, const char* path, int spritePixelSizeIn)
{
    spritePixelSize = spritePixelSizeIn;
	GLTexture = openGLRenderer->CreateTexture(path, sheetPixelWidth, sheetPixelHeight);
	
	numberRows = sheetPixelHeight / spritePixelSize;
	numberColumns = sheetPixelWidth / spritePixelSize;
}

void SpriteSheet::RenderSprite(OpenGLRenderer* openGLRenderer, glm::mat4 mvp, int index)
{
    glUseProgram(GLProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, GLTexture);
    glUniform1i(glGetUniformLocation(GLProgram, "tex"), 0);

    glBindVertexArray(openGLRenderer->VAO);

    glUniformMatrix4fv(glGetUniformLocation(GLProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

    glm::vec2 uvScale = { (float)spritePixelSize / (float)sheetPixelWidth, (float)spritePixelSize / (float)sheetPixelHeight };

    int colOffset = index % numberColumns;
    int rawOffset = (float)(numberRows - 1) - (index / numberColumns);

    glm::vec2 uvOffset = { (float)colOffset * uvScale.x, (float)rawOffset * uvScale.y };

    glUniform2fv(glGetUniformLocation(GLProgram, "uvOffset"),
        1, glm::value_ptr(uvOffset));
    glUniform2fv(glGetUniformLocation(GLProgram, "uvScale"),
        1, glm::value_ptr(uvScale));

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
