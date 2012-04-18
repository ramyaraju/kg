#ifndef _ECPOINT_H_
#define _ECPOINT_H_

#pragma comment(lib, "mpir.lib")
#include <mpir.h>

class ECPoint
{
public:
	ECPoint(void);
	ECPoint(char *Rx, char *Ry, int base);
	~ECPoint(void);
	int marker;
	mpz_t x;
	mpz_t y;
};
#endif // _ECPOINT_H_
