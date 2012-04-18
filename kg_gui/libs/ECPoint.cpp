#include "ECPoint.h"

ECPoint::ECPoint( void )
{
	mpz_init(x);
	mpz_init(y);
	marker = 1;
}

ECPoint::ECPoint( char *Rx, char *Ry, int base )
{
	mpz_init(x);
	mpz_set_str(x, Rx, base);
	mpz_init(y);
	mpz_set_str(y, Ry, base);
	marker = 0;
}

ECPoint::~ECPoint( void )
{
	mpz_clear(x);
	mpz_clear(y);
	marker = 1;
}