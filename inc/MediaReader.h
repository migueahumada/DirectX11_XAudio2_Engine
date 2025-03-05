#pragma once
#include <fstream>


using std::fstream;

class MediaReader
{
public:
	MediaReader() = default;
	~MediaReader() = default;
	void decode(const char* filename);

private:


};

