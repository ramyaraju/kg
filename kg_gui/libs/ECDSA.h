#pragma once

#ifndef _ECDSA_H_
#define _ECDSA_H_

#include "ECC.h"

class ECDSA
{
public:
	ECDSA(void);
	~ECDSA(void);

	void Sign(
		char *hash,
		char *&r,
		char *&s
		);

	bool Verify(
		char *hash,
		char *r,
		char *s
		);

	bool init(
		int base
		);

	bool SetDomain(
		char *a,
		char *b,
		char *p,
		char *Gx,
		char *Gy,
		char *n,
		unsigned int seed
		);

	bool SetKeys(
		char *dA,
		char *Rx,
		char *Ry
		);

	bool Clean(
		);

private:
	char		tmp[255];
	gmp_randstate_t		m_st;
	int			m_base;
	ECC			*ecc;
};

#endif // _ECDSA_H_
