#include "Base32.h"

bool B32Encode(char* inBuf, int inLen, char* outBuf)
{
	const char *alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
	if( (inBuf == NULL) || (outBuf == NULL) || (inLen == 0) ) return false; // bad input

	int div = inLen / 5;	// divisor or number of full blocks to process
	int rem = inLen % 5;	// reminder

	unsigned __int64 val;
	char *p = inBuf;
	char *o = outBuf;
	

	for(int j = 0; j < div; j++)
	{
		val = 0;
		for(int x=0; x<5; x++)
		{
			val = val << 8;
			val |= p[x];
		}
		p += 5;

		for(int i=0; i<8; i++)
		{
			o[7-i] = alpha[val&0x1F];
			val = val >> 5;
		}
		o += 8;
	}

	if(!rem) return true;
	// padding
	for(int k=0; k<rem; k++)
	{
		val = val << 8;
		val |= p[k];
	}

	// calculate number of characters used in b32
	unsigned int t = (rem*8-1)/5 + 1;
	val = val << 8*(5-rem);
	t = 8-t;
	for(int l=0; l<8; l++)
	{
		if(!t)
		{
			o[7-l] = alpha[val&0x1F];
		} else {
			o[7-l] = '=';
			t--;
		}
		val = val >> 5;
	}
	return true;
}
