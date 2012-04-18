#pragma once

#ifdef linux
	#include <stdint.h>
	typedef int64_t __int64;
	typedef uint64_t __uint64;
#endif

#ifdef _WIN32
	typedef unsigned __int64 __uint64;
#endif

	/*	
		powmod(b,p,n)
		computes power of a number in a group Z_n (modulo n)
		return b^p mod n
	*/
	__int64 powmod(__int64 b, __int64 p, __int64 n);

	/*
		mulmod(a,b,n)
		multiply two numbers b and p in a group Z_n (modulo n)
		return a*b mod n
	*/
	__int64 mulmod(__int64 a, __int64 b, __int64 n);

	/*
		gcd(a,b)
		computes greatest common divisor of two numbers a and b
		return GCD(a,b)
	*/
	__int64 gcd(__int64 a, __int64 b);

	/*
		invmod(a,n)
		computes inversion of a number in a group Z_n (modulo n)
		return a^-1 mod n
	*/
	__int64 invmod(__int64 a, __int64 n);

	/*
		egcd(a,b,x,y)
		computes greatest common divisor using extended euclidean algorithm
		return x = a*x + b*y = gcd(a,b)
	*/
	__int64 egcd(__int64 a, __int64 b, __int64 &x, __int64 &y);

	/*
		addmod(a,b,n)
		adds two numbers in a group Z_n (modulo n)
		r = a+b mod n
	*/
	__int64 addmod(__int64 a, __int64 b, __int64 n);
