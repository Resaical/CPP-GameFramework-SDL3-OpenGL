#pragma once
#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>


class SpriteSheet;
class OpenGLRenderer;
class Camera;

class TileMap
{
public:

	TileMap() {};
	~TileMap();

	void LoadTileMap(std::string dataIn);
	void SetSpriteSheet(SpriteSheet* spriteSheetIn);
	void AddTileToIgnore(int id);

	int GetTileIndexAtPosition(glm::vec2 position);

	glm::vec2 GetTileMapElementByIndex(int id);

	void Render(OpenGLRenderer* openGLRenderer, Camera* camera);

	SpriteSheet* spriteSheet = nullptr;

	std::vector<int> tilesToIgnore;
	
	std::vector<int> data;
	int numberColumns = 0;
	int numberRows = 0;
};