#include "CRT.h"

CRT::CRT(void)
{
}

CRT::~CRT(void)
{
}
/*
	r = n[0] * n[1] * ... * n[k]
	rop = a[0] mod n[0]
	rop = a[1] mod n[1]
	...
	rop = a[k] mod n[k]
*/
bool CRT::Solve( mpz_t rop, mpz_t *a, mpz_t *n, mpz_t r, int k )
{
	mpz_t N, M;

	if(k<1)
	{
		return false;
	}
	mpz_inits(N, M, NULL);
	mpz_set_ui(rop, 0);

	for(int i=0; i<k; i++)
	{
		// compute N
		if(mpz_divisible_p(r, n[i]) == 0)
		{
			mpz_clears(N, M, NULL);
			return false;
		}
		mpz_divexact(N, r, n[i]);

		// compute M
		mpz_invert(M, N, n[i]);

		// compute a_i * N * M (mod r)
		mpz_mul(N, N, M);
		mpz_mul(N, N, a[i]);
		mpz_mod(N, N, r);

		// add to sum
		mpz_add(rop, rop, N);
	}
	return true;
}