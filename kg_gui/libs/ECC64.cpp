#include "ECC64.h"


ECC64::ECC64(void)
{
	srand( (unsigned int)time(NULL) );
}

ECC64::~ECC64(void)
{
}

bool ECC64::init(ECDomain ecd)
{
	__int64 a,b,q,delta,t;
	__int64 left, right,x,y;
	// copy the curve domain data
	m_ecd.a		= ecd.a;
	m_ecd.b		= ecd.b;
	m_ecd.FR	= ecd.FR;
	m_ecd.h		= ecd.h;
	m_ecd.n		= ecd.n;
	m_ecd.p.inf = ecd.p.inf;
	m_ecd.p.x	= ecd.p.x;
	m_ecd.p.y	= ecd.p.y;
	m_ecd.q		= ecd.q;
	m_ecd.S		= ecd.S;

	// check if curve is correct
	a = m_ecd.a;
	b = m_ecd.b;
	q = m_ecd.q;
	
	delta	= powmod(a,3,q);
	delta	= mulmod(delta,4,q);
	t		= mulmod(b,b,q);
	t		= mulmod(b,27,q);
	delta	= addmod(delta,t,q);
	delta	= mulmod(-16,delta,q);
	if(!delta) 
		return false;

	// check if point p is on that curve
	if(m_ecd.p.inf) return false;
	x = m_ecd.p.x;
	y = m_ecd.p.y;
	left	= mulmod(y,y,q);
	right	= mulmod(x,x,q);
	right	= mulmod(x,right,q);
	t		= mulmod(a,x,q);
	right	= addmod(right,t,q);
	right	= addmod(right,b,q);
	if(left!=right) 
		return false;

	return true;
}

void ECC64::copyPoint(ecp P, ecp &R)
{
	R.inf = P.inf;
	R.x = P.x;
	R.y = P.y;
}


void ECC64::addPoints(ecp P, ecp Q, ecp &R)
{
	__int64 inv,s,xt,tt;
	// if any of the points is the point in inifity, then return 
	if(P.inf)
	{
		R.inf = Q.inf;
		R.x = Q.x;
		R.y = Q.y;
		return;
	} else if (Q.inf)
	{
		R.inf = P.inf;
		R.x = P.x;
		R.y = P.y;
		return;
	}
	// check if P and Q are distinct points
	// if they are not, return point double
	if((P.x == Q.x) && (P.y == Q.y))
	{
		doublePoint(P, R);
		return;
	}
	// if P == -Q then return point in infinity
	if(P.y == -Q.y)
	{
		R.inf = true;
		R.x = Q.x;
		R.y = Q.y;
		return;
	}

	// the slope of the line between P and Q
	// the slope is denominated as:
	// s = (Py-Qy)/(Px-Qx) mod p;
	inv = invmod(Q.x-P.x,m_ecd.q);
	s	= mulmod(Q.y-P.y,inv, m_ecd.q);
	xt	= mulmod(s,s,m_ecd.q);
	tt	= addmod(P.x, Q.x, m_ecd.q);
	R.x = addmod(xt,-tt, m_ecd.q);
	if(R.x < 0)
		R.x += m_ecd.q;
	xt	= mulmod(s, P.x-R.x, m_ecd.q);
	R.y = addmod(xt,-P.y,m_ecd.q);
	if (R.y < 0)
		R.y += m_ecd.q;

}

void ECC64::doublePoint(ecp P, ecp &R)
{
	__int64 s,inv,xt;
	if(P.y == 0)
	{
		R.inf = true;
		R.x = 0;
		R.y = 1;
		return;
	}
	s	= mulmod(P.x, P.x, m_ecd.q);
	s	= mulmod(s,3, m_ecd.q);
	s	= addmod(s, m_ecd.a, m_ecd.q);
	inv = mulmod(P.y, 2, m_ecd.q);
	inv = invmod(inv, m_ecd.q);
	s	= mulmod(s,inv,m_ecd.q);
	xt	= mulmod(s,s, m_ecd.q);
	xt	= xt - mulmod(P.x,2, m_ecd.q);
	R.x = xt % m_ecd.q;
	if(R.x < 0)
		R.x += m_ecd.q;
	xt	= mulmod(s, P.x-R.x, m_ecd.q) - P.y;
	R.y = xt % m_ecd.q;
	if (R.y < 0)
		R.y += m_ecd.q;
}

void ECC64::multiplePoint(__int64 k, ecp P, ecp &R)
{
	R.inf = true;
	R.x = 0;
	R.y = 0;

	while(k)
	{
		if((k&1)==1)
			addPoints(P,R,R);
		doublePoint(P,P);
		k>>=1;
	}
	if(R.x < 0)
		R.x += m_ecd.q;
	if (R.y < 0)
		R.y += m_ecd.q;
}

bool ECC64::setPoint(__int64 x, __int64 y, ecp &P)
{
	__int64 t,left,right;
	left	= mulmod(y,y,m_ecd.q);
	right	= mulmod(x,x,m_ecd.q);
	right	= mulmod(x,right,m_ecd.q);
	t		= mulmod(m_ecd.a,x,m_ecd.q);
	right	= addmod(right,t,m_ecd.q);
	right	= addmod(right,m_ecd.b,m_ecd.q);
	if(left!=right) 
		return false;

	P.inf = false;
	P.x = x;
	P.y = y;
	return true;
}

ECSig ECC64::Sign(__int64 hash, ECKeypair keys)
{
	ECSig sig;
	ecp Q;
	__int64 k,t;
	do 
	{
		k = rand() % (m_ecd.n-1);				// random k[1,n-1]
		multiplePoint(k, m_ecd.p,Q);			// compute kP=(x1,y1)
		sig.r = Q.x % m_ecd.n;					// convert x1 to x mod n
		sig.s = powmod(k,-1,m_ecd.n);		// compute k^-1 mod n
		t = mulmod(keys.dA,sig.r,m_ecd.n);	// compute (d*r) mod n
		t = addmod(t,hash,m_ecd.n);			// compute (h+d*r) mod n
		sig.s = mulmod(sig.s,t,m_ecd.n);	// compute k^-1*(h+d*r) mod n
	} while (!sig.r || !sig.s);					// if r == 0 or s == 0, repeat

	return sig;									// r and s produce signature(r,s)
}

bool ECC64::Verify(ECKeypair keys, __int64 hash, ECSig sig)
{
	__int64 w,u1,u2,v;
	ecp P,Q;

	if(sig.r>=m_ecd.q || sig.s>=m_ecd.q)	// if r,s are not in the interval [1,n-1] reject the signature
		return false;

	w	= powmod(sig.s, -1, m_ecd.n);		// compute w = s^-1 mod n
	u1	= mulmod(hash, w, m_ecd.n);			// compute u1 = h*w mod n
	u2	= mulmod(sig.r,w,m_ecd.n);			// compute u2 = r*w mod n
	
	multiplePoint(u1, m_ecd.p, P);			// compute u1*P
	multiplePoint(u2, keys.Q, Q);			// compute u2*Q
	addPoints(P,Q,Q);						// compute u1*P + u2*Q
	
	if(Q.inf)
		return false;						// if Q=inf, reject the signature
	v = Q.x % m_ecd.n;						// convert the x-coordinate x1 of Q to an integer x, compute v = x mod n
	if (v==sig.r)							// if v == r then accept the signature
	{
		return true;
	}
	return false;										// reject otherwise
}
