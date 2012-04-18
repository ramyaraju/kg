#pragma once

#include <stdlib.h>
#include <time.h>
#include "BN.h"

struct ecp 
{
	__int64 x;		// x-coordinate
	__int64 y;		// y-coordinate
	bool inf;	// is this point a point at infinity
};

struct ECSig
{
	__int64 r;	// r signature
	__int64 s;	// s signature
};

struct ECDomain
{
	__int64 q;	// field order
	__int64 FR;	// field representation
	__int64 S;	// seed
	__int64 a;	// curve coefficients
	__int64 b;
	ecp p;	// base point, needs a prime order
	__int64 n;	// order of p
	__int64 h;	// cofactor h = #E(Fq)/n
};

struct ECKeypair
{
	__int64 dA;	// private key
	ecp Q;	// public key
};

// functions for elliptic curves over a finite field of max size 63bit
// see test.cpp on how to use it
class ECC64
{
public:
	ECC64(void);
	ECC64(ECDomain);
	~ECC64(void);
	ECSig Sign(__int64 hash, ECKeypair keys);
	bool Verify(ECKeypair keys, __int64 hash, ECSig sig);
	void addPoints(ecp P, ecp Q, ecp &R);
	void doublePoint(ecp P, ecp &R);
	void multiplePoint(__int64 k, ecp P, ecp &R);
	bool setPoint(__int64 x, __int64 y, ecp &P);
	bool init(ECDomain ecd);
	void copyPoint(ecp P, ecp &R);

private:
	ECDomain m_ecd;

};
