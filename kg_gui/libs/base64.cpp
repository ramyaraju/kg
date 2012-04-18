#include "base64.h"

const char *map = "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ0123456789+=";

base64::base64(void)
{
}

base64::~base64(void)
{
}

void base64::initialise( char *input[] )
{
	int c;
	for (c = 0; c<0x100; c++)
	{
		input[c] = ConvertIntToBinaryStr(c);
	}
}

char* base64::ConvertIntToBinaryStr( unsigned int i )
{
	char *tab = new char[0x10];
	memset(tab, 0x30, 8);
	tab[8] = 0;
	int k = 0;
	while(i!=0)
	{
		if(i&1)
			tab[7-k] = '1';
		k++;
		i = i>>1;
	}
	return tab;
}

int base64::ConvertBinaryStrToInt( char *str )
{
	int len = strlen(str);
	int a = 0, k =0;
	char t = 0;
	while(len--)
	{
		a = a<<1;
		if(str[k++] == '1')
		{
			a = a | 1;
		}
	}
	return a;
}

char *base64::ConvertBinaryStrToB64(char *input, int inLen)
{
	char tmp2[7];
	char *tmp = new char[inLen];
	int t;
	memcpy(tmp, input, inLen);
	memset(tmp2, 0, 7);
	int sl = inLen;
	while((sl%6)!=0)
	{
		strcat(tmp, "0");
		sl = strlen(tmp);
	}
	sl = strlen(tmp)/6;
	char *output = new char[sl+1];
	memset(output, 0, sl+1);
	char *p = tmp;
	int k = 0;
	for(int i=0;i<sl;i++)
	{
		memcpy(tmp2,p,6);
		t = ConvertBinaryStrToInt(tmp2);
		output[k++] = map[t];
		p += 6;
	}

	// cleaning up
	delete [] tmp;

	return output;
}

char *base64::ConvertB256To64( char* input, int inLen)
{
	char tmp2[7];
	char *tmp = new char[inLen*9];
	
	int t,k;
	memset(tmp, 0, inLen*8+8);
	memset(tmp2, 0, 7);
	

	for (int i=0; i<=inLen;i++)
	{
		strcat(tmp, ConvertIntToBinaryStr(input[i]));
	}
	int sl = strlen(tmp);
	while((sl%6)!=0)
	{
		strcat(tmp, "0");
		sl = strlen(tmp);
	}
	sl = strlen(tmp)/6;
	char *output = new char[sl+1];
	memset(output, 0, sl+1);
	char *p = tmp;
	k = 0;
	for(int i=0;i<sl;i++)
	{
		memcpy(tmp2,p,6);
		t = ConvertBinaryStrToInt(tmp2);
		output[k++] = map[t];
		p += 6;
	}

	// cleaning up
	delete [] tmp;

	return output;
}
