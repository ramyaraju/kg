#include "gauss.h"

/************************************************************************/
/* Z_p group operations                                                 */
/************************************************************************/

// addition
int gauss_addmod(int a, int b, int n)
{
	unsigned int r;
	if(!a) return b%n;
	if(!b) return a%n;
	if(n<0) return -1; 

	if(a<0) a+=n;
	if(b<0) b+=n;

	r = (unsigned int)a+(unsigned int)b;
	r = r%n;
	return r;
}

// subtraction
int gauss_submod(int a, int b, int n)
{
	int r;
	if(!a) return (n-b)%n;
	if(!b) return a%n;
	if(n<0) return -1; 

	if(a<0) a+=n;
	if(b<0) b+=n;

	r = a-b;
	r = r%n;
	if (r<0)
		r+=n;
	return r;
}

// multiplication
int gauss_mulmod(int a, int b, int n)
{
	int r;
	if(!n) return -1;

	a %= n;
	b %= n;
	if (!b||!a) return 0;
	if (a<0) a += n;
	if (b<0) b += n;

	r = 0;

	while(b)
	{
		if(b&1) r = gauss_addmod(r,a,n);
		b>>=1;
		a = gauss_addmod(a,a,n);
	}
	return r;
}

// division
int gauss_divmod(int a, int b, int n)
{
	// r = (a^-1 mod p) ^ b mod p
	unsigned int r = gauss_invmod(b,n);
	r = gauss_mulmod(a,r,n);
	return r;
}

// power
int gauss_powmod(int b, int p, int n)
{
	int val;
	if(!n) return -1;
	p = p % n;
	if(p==0) return 1;
	if(p<0) p += n-1;

	val = 1;

	while(p)
	{
		if(p&1) val = gauss_mulmod(val,b,n);
		p = p>>1;
		b = gauss_mulmod(b,b,n);
	}
	return val;
}

// inversion modulo
int gauss_invmod(int a, int n)
{
	int gc,x,y;
	if(a<0) a+=n;
	if(a>n){
		x = a;
		a = n;
		n = x;
	}
	gc = gauss_egcd(a,n,x,y);
	if(gc==1) {
		if(x<0) x += n;
		return x;
	}
	return -1;
}

// extended euclidean algorithms
int gauss_egcd(int a, int b, int &x, int &y)
{
	int p = b;
	int x1,x2,y1,y2;
	int q, r;
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
	if (x2 < 0)
		x2 += p;
	if(y2 < 0)
		y2 += p;
	x = x2;
	y = y2;
	return a;
}

void gauss(
		   int (*A)[7],	// square matrix A
		   int n,		// size of A
		   int p,		// modulo
		   bool &err
		   )
{
	// forward elimination
	int i_max;
	int t;
	for(int i=0;i<n;i++)
	{
		i_max = i;

		for(int j=i+1; j<n; j++)
		{
			if( A[j][i] > A[i_max][i] )
			{
				i_max = j;
			}
		}

		for(int j=0; j<n; j++)
		{
			t = A[i_max][j];
			A[i_max][j] = A[i][j];
			A[i][j] = t;
		}
		for(int j=n; j>=i; j--)
		{
			for(int k=i+1; k<n; k++)
			{
				t = gauss_divmod(A[k][i],A[i][i],p);
				t = gauss_mulmod(t, A[i][j], p);
				t = gauss_submod(A[k][j], t, p);
				A[k][j] = t;
			}
		}
	}

	// back substitution
	for (int i=n-1; i>=0; i--)
	{
		A[i][n] = gauss_divmod(A[i][n],A[i][i],p);
		A[i][i] = 1;
		for(int j=i-1; j>=0; j--)
		{
			t = gauss_mulmod(A[j][i], A[i][n],p);
			A[j][n] = gauss_submod(A[j][n], t, p);
			A[j][i] = 0;
		}
	}
	return;
}
