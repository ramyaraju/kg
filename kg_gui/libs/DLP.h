#pragma once

#ifndef _DLP_H_
#define _DLP_H_

#include "Factor.h"
#include "CRT.h"
#include "mpir.h"

#define mod(x,y) if (mpz_cmp_ui(x,0)<0 || mpz_cmp(x,y)>=0)	mpz_mod(x,x,y);

class DLP
{
public:
	DLP(void){};
	~DLP(void){};

	/*
		Solve discrete logarithm using Pollard's-Rho algorithm
		alpha ^ x mod p = beta
		Params:
			char * alpha,
			char * beta,
			char * p
		Returns:
			Secret value x
	*/
		 
	char * Solve(
		char * alpha,
		char * beta,
		char *p
		);

	void rho(
		mpz_t alpha,
		mpz_t beta,
		mpz_t n,
		mpz_t g,
		mpz_t k
	);

	void iterate(
		mpz_t x,
		mpz_t a,
		mpz_t b,
		mpz_t n,
		mpz_t g,
		mpz_t alpha,
		mpz_t beta
		);

private:
	mpz_t m_a,m_b,m_p,m_np;
	gmp_randstate_t m_st;
	int m_base;

};
#endif // _ECDLP_H_