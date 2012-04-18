#pragma once

#ifndef _FACTOR_H_
#define _FACTOR_H_

#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "mpir.h"

void __cdecl odprintf(const char *format, ...);
void db(char *frm, mpz_t a, int base);

class Factor
{
public:
	Factor(int type);
	Factor();
	~Factor(void);

	/*
		Factor the number using Pollard's-Rho algorithms improved by Brent
		Input:
			n - hexadecimal string representing a number to be factorise
		Output:
			factors - number of prime factors found
			function returns an array of pointers to char strings containing consequent prime factors
			
	*/
		 
	bool Start(
			char *n
		);

	bool Start(
		mpz_t n
		);
	
	void brent(
		mpz_t n,
		mpz_t factor
		);

	int 
		GetNumFactors();

	char * 
		GetFactor(
		int i
		);

	void 
		GetFactor(
		mpz_t rop,
		int i
		);

public:
	const enum {
		FP_STR = 1,
		FP_BIG
	};

private:
	void f(mpz_t rop, mpz_t a, mpz_t n);

private:
	gmp_randstate_t m_st;
	int m_mode, m_factors;

	// primes
	char **fs;
	mpz_t *fb;
};
#endif // _ECDLP_H_