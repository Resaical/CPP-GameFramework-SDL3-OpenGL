#pragma once
#include <string>

class FileLoader
{
public:

	FileLoader() {};
	~FileLoader() {};

	std::string LoadTextFileInString(const char* path);

	unsigned char* LoadImageData(const char* path, int& w, int& h, int& channels);
	void ReleaseImageData(unsigned char* data);
};