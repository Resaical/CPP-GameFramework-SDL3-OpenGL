#include "fileLoader.h"
#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::string FileLoader::LoadTextFileInString(const char* path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned char* FileLoader::LoadImageData(const char* path, int& w, int& h, int& channels)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path, &w, &h, &channels, 0);
    
    return data;
}

void FileLoader::ReleaseImageData(unsigned char* data)
{
    stbi_image_free(data);
}
