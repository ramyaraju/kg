#include "BN.h"


__int64 addmod(__int64 a, __int64 b, __int64 n)
{
	__uint64 r;
	if(!a) return b%n;
	if(!b) return a%n;
	if(n<0) return -1; 

	if(a<0) a+=n;
	if(b<0) b+=n;

	r = (__uint64)a+(__uint64)b;
	r = r%n;
	return r;
}

__int64 mulmod(__int64 a, __int64 b, __int64 n)
{
	__int64 r;
	if(!n) return -1;

	a %= n;
	b %= n;
	if (!b||!a) return 0;
	if (a<0) a += n;
	if (b<0) b += n;

	r = 0;

	while(b)
	{
		if(b&1) r = addmod(r,a,n);
		b>>=1;
		a = addmod(a,a,n);
	}
	return r;
}

__int64 powmod(__int64 b, __int64 p, __int64 n)
{
	__int64 val;
	if(!n) return -1;
	p = p % n;
	if(p==0) return 1;
	if(p<0) p += n-1;

	val = 1;

	while(p)
	{
		if(p&1) val = mulmod(val,b,n);
		p = p>>1;
		b = mulmod(b,b,n);
	}
	return val;
}

__int64 gcd(__int64 a, __int64 b)
{
	__int64 g=0;

	if (a < 0) a = -a;
	if (b < 0) b = -b;
	if (!(a+b)) return -1;

	while(a>0)
	{
		g = a;
		a = b % a;
		b = g;
	}
	return g;
}

__int64 egcd(__int64 a, __int64 b, __int64 &x, __int64 &y)
{
	__int64 x1,x2,y1,y2;
	__int64 q, r;
	if(!b){
		x = 1;
		y = 0;
		return a;
	}

	x1 = 0;
	x2 = 1;
	y1 = 1;
	y2 = 0;

	while(b>0)
	{
		q = a/b;
		r = a - q*b;
		x = x2 - q*x1;	
		y = y2 - q*y1;
		a = b;
		b = r;
		x2 = x1;
		x1 = x;
		y2 = y1;
		y1 = y;
	}
	x = x2;
	y = y2;
	return a;
}

__int64 invmod(__int64 a, __int64 n)
{
	__int64 gc,x,y;
	if(a<0) a+=n;
	if(a>n){
		x = a;
		a = n;
		n = x;
	}
	gc = egcd(a,n,x,y);
	if(gc==1) {
		if(x<0) x += n;
		return x;
	}
	return -1;
}
