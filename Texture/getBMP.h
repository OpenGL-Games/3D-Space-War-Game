#ifndef GETBMP_H
#define GETBMP_H

#include <string>

struct imageFile
{
	int width;
	int height;
	unsigned char *data;
};

imageFile *getBMP(std::string fileName);

#endif
