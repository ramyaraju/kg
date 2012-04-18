#include "crc64.h"

UINT64 CRC64_table[256];

// find an entry with the specific topbyte
// entry -> value from an array
// idx -> index, returned valu
int find_entry_crc64(unsigned char topbyte, UINT64 *entry, UINT64 *idx){
	UINT64 i;

	*idx = -1;
	for(i=0;i<256;i++){
		*entry = CRC64_table[i];
		if(topbyte == *entry>>56){
			*idx = i;
			break;
		}
	}

	return (*idx!=-1);
}

// generate table for crc
VOID CRC64_gen(UINT64 poly)
{
    for(INT i=0; i<256; ++i)
    {
    	UINT64 crc = i;
    	for(UINT j=0; j<8; ++j)
    	{
    		if(crc & 1)
            {
                crc >>= 1;
    			crc ^= poly;
            } else 
			{
    			crc >>= 1;
            }
    	}
        CRC64_table[i] = crc;
    }
}

// compute backward crc, from the position n (with crc=fcrc) to position k
UINT64 bcrc64(PBYTE stream, UINT n, UINT k, UINT64 fcrc)
{
	UINT64 crc = fcrc;
	UINT64 idx = 0, a2 = 0;
	BYTE elem;

	if (k > n)
		return -1;

	for (UINT i=n; i>k; --i)
	{
		find_entry_crc64(crc >> 56, &a2, &idx);
		elem = (BYTE)(stream[i-1]^idx);
		crc = ((crc ^ a2) << 8) | (BYTE)elem;

	}

	return crc;
}

// compute normal crc
UINT64 crc64(PBYTE stream, UINT n)
{
    UINT64 crc = 0;

    for(UINT i=0; i<n; ++i)
    {
        BYTE index = (BYTE)(stream[i] ^ crc);
        crc = (crc >> 8);
        crc ^= CRC64_table[index];
    }

    return crc;
}

UINT64 rev_crc64(UINT64 crc, UINT64 target){
	UINT64 a,b,c,d,e,f,g,h;			// crc64 elements we need to find.
	UINT64 idx,sol;					// helping variables
	UINT64 ai,bi,ci,di,ei,fi,gi,hi; // indexes

	find_entry_crc64((unsigned char)(target>>56), &h, &hi);
	target ^= h;
	find_entry_crc64((unsigned char)(target>>48), &g, &gi);
	target ^= g>>8;
	find_entry_crc64((unsigned char)(target>>40), &f, &fi);
	target ^= f>>16;
	find_entry_crc64((unsigned char)(target>>32), &e, &ei);
	target ^= e>>24;
	find_entry_crc64((unsigned char)(target>>24), &d, &di);
	target ^= d>>32;
	find_entry_crc64((unsigned char)(target>>16), &c, &ci);
	target ^= c>>40;
	find_entry_crc64((unsigned char)(target>>8),  &b, &bi);
	target ^= b>>48;
	find_entry_crc64((unsigned char)(target),     &a, &ai);

	crc ^= a<<8;
	crc ^= b<<16;
	crc ^= c<<24;
	crc ^= d<<32;
	crc ^= e<<40;
	crc ^= f<<48;
	crc ^= g<<56;

	idx = ai | bi<<8 | ci<<16 | di<<24 | ei<<32 | fi<<40 | gi<<48 | hi<<56;

	sol = crc ^ idx;

	return sol;
}
