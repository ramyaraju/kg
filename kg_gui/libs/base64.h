#pragma once
#include <memory.h>
#include <string.h>

class base64
{
public:
	base64(void);
	~base64(void);
	static char *ConvertB256To64(char* input, int inLen);
	static char *ConvertBinaryStrToB64(char *input, int inLen);
	static void initialise(char *input[]);
	static char* ConvertIntToBinaryStr(unsigned int i);
	static int ConvertBinaryStrToInt(char *str);
};
