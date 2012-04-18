#pragma once

#include <mpir.h>

class CRT
{
public:
	CRT(void);
	~CRT(void);

	// solve k simultaneous congruences using Gauss's algorithm
	static
	bool
		Solve( mpz_t rop, mpz_t *a, mpz_t *n, mpz_t r, int k );
};
