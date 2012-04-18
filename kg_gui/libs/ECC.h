#pragma once

#ifndef _ECC_H_
#define _ECC_H_

#include "ECPoint.h"

class ECC
{
public:
	ECC(void);
	~ECC(void);

	void init(
		int base
		);

	bool SetDomain(
		char *a,
		char *b,
		char *q,
		char *Gx,
		char *Gy,
		char *n,
		unsigned int  seed
		);

	bool SetDomain(
		mpz_t a,
		mpz_t b,
		mpz_t q,
		ECPoint &G,
		mpz_t n
		);

	bool SetDomain(
		mpz_t a,
		mpz_t b,
		mpz_t p
		);

	bool SetPoint(
		char *x,
		char *y,
		ECPoint &R
		);

	void CopyPoint(
		ECPoint &P,
		ECPoint &R
		);

	void AddPoints(
		ECPoint &P,
		ECPoint &Q,
		ECPoint &R
		);

	void DoublePoint(
		ECPoint &P,
		ECPoint &R
		);

	void MultiplePoint(
		mpz_t k,
		ECPoint &P,
		ECPoint &R
		);

	bool SetKeys(
		char *dA,
		char *Rx,
		char *Ry
		);

	void clean(
		);

	void ClearPoint(
		ECPoint &R
		);

	int ComparePoints( 
		ECPoint &P,
		ECPoint &R
		);

	void GetPoint(
		ECPoint &P,
		mpz_t &x,
		mpz_t &y
		);

	bool IsAtInfinity(
		ECPoint &P
		);


public:
	mpz_t			m_a,m_b,m_p,m_n;
	ECPoint			m_G, m_P, m_R;
	mpz_t			m_k,m_x,m_y;
	mpz_t			m_dA;
	int				m_base;

private:
	gmp_randstate_t m_st;
	bool			m_isGenerator;
};
#endif // _ECC_H_
