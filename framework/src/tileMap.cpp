#include "tileMap.h"
#include <sstream>
#include "openGLRenderer.h"
#include "fileLoader.h"
#include <basicComponents.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include "spriteSheet.h"

TileMap::~TileMap()
{
    delete spriteSheet;
}

void TileMap::LoadTileMap(std::string dataIn)
{
    std::istringstream stream(dataIn);
    std::string line;

    while (std::getline(stream, line)) {
        if (line.empty()) continue;

        std::istringstream lineStream(line);
        std::string token;
        int colsThisRow = 0;

        while (std::getline(lineStream, token, ',')) {
            data.push_back(std::stoi(token));
            colsThisRow++;
        }

        if (numberColumns == 0) numberColumns = colsThisRow;
        numberRows++;
    }

    std::vector<int> flipped;
    flipped.reserve(data.size());

    for (int y = numberRows - 1; y >= 0; y--)
    {
        for (int x = 0; x < numberColumns; x++)
        {
            flipped.push_back(data[y * numberColumns + x]);
        }
    }

    data = flipped;

    for (auto& element : data)
    {
        for( auto ignore : tilesToIgnore )
        {
            if (element != ignore) continue;

            element = -1;
            break;
        }
    }
}

void TileMap::SetSpriteSheet(SpriteSheet* spriteSheetIn)
{
    spriteSheet = spriteSheetIn;
}

void TileMap::Render(OpenGLRenderer* openGLRenderer, Camera* camera)
{
    Transform2D* camTransform = camera->entity->GetComponent<Transform2D>();
    glm::mat3 view = camera->GetViewMatrix(camTransform->GetWorldPosition(), camTransform->GetWorldRotation());
    glm::mat4 proj = camera->GetProjectionMatrix();

    for (int y = 0; y < numberRows; y++)
    {
        for (int x = 0; x < numberColumns; x++)
        {
            int tileID = data[x + y * numberRows];
            if (tileID < 0) continue;
            bool contains = std::find(tilesToIgnore.begin(), tilesToIgnore.end(), tileID) != tilesToIgnore.end();
            if (contains) continue;

            glm::mat3 model = glm::mat3(1.0f);
            model[2][0] = x * 64.0f;
            model[2][1] = y * 64.0f;

            glm::mat4 mv = openGLRenderer->Mat3ToMat4(view * model);
            glm::mat4 mvp = proj * mv;

            spriteSheet->RenderSprite(openGLRenderer, mvp, tileID);
        }
    }
}

void TileMap::AddTileToIgnore(int id)
{
    bool contains = std::find(tilesToIgnore.begin(), tilesToIgnore.end(), id) != tilesToIgnore.end();
    if (contains) return;

    tilesToIgnore.push_back(id);
}

int TileMap::GetTileIndexAtPosition(glm::vec2 position)
{
    int x = (int)glm::round(position.x / 64.0f);
    int y = (int)glm::round(position.y / 64.0f);

    if (x >= numberColumns || x < 0) return -2;
    if (y >= numberRows || y < 0) return -2;

    return data[x + y * numberColumns];
}

glm::vec2 TileMap::GetTileMapElementByIndex(int id)
{
    for (int y = 0; y < numberRows; y++)
    {
        for (int x = 0; x < numberColumns; x++)
        {
            if (data[y * numberColumns + x] == id)
            {
                return glm::vec2(x * 64.0f, y * 64.0f);
            }
        }
    }

    return glm::vec2(-1, -1);
}
