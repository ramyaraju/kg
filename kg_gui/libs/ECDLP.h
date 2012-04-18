#pragma once

#ifndef _ECDLP_H_
#define _ECDLP_H_

#define NB_BRANCHES 16
#include "ECC.h"

class ECDLP
{
public:
	ECDLP(void);
	~ECDLP(void);
	bool init(
		char *a,
		char *b,
		char *q,
		int  base
		);

	bool init(
		mpz_t a,
		mpz_t b,
		mpz_t p
		);

	void rho(
		char *nps,
		char *Gx,
		char *Gy,
		char *Rx,
		char *Ry,
		mpz_t k
		);

	void rho(
		ECPoint &P,
		ECPoint &Q,
		mpz_t np,
		mpz_t k
		);

private:
	mpz_t m_a,m_b,m_p,m_np;
	gmp_randstate_t m_st;
	int m_base;

	ECC *ecc;
};
#endif // _ECDLP_H_
