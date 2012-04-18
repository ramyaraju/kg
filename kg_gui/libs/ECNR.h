#pragma once

#ifndef _ECNR_H_
#define _ECNR_H_

#include "ECC.h"

class ECNR
{
public:
	ECNR(void);
	~ECNR(void);

	bool Sign(
		unsigned int hash,
		char *&r,
		char *&s
		);

	bool Sign(
		char *hash,
		char *&r,
		char *&s
		);

	bool Sign(
		char *hash,
		char *&r,
		char *&s,
		char *k
		);

	bool Verify(
		unsigned int hash,
		char *r,
		char *s
		);

	bool Verify(
		char *hash,
		char *rs,
		char *ss
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

#endif // _ECNR_H_
