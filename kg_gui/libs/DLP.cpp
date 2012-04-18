#include "DLP.h"

void DLP::iterate(mpz_t x, mpz_t a, mpz_t b, mpz_t n, mpz_t g, mpz_t alpha, mpz_t beta)
{
	mpz_t t;
	mpz_init_set(t, x);
	mpz_mod_ui(t, t, 3);

	if (mpz_cmp_ui(t, 0) == 0)
	{
		mpz_mul(x, x, x);
		mod(x, n);
		mpz_mul_ui(a, a, 2);
		mod(a, g);
		mpz_mul_ui(b, b, 2);
		mod(a, g);
	} else if (mpz_cmp_ui(t, 2) == 0)
	{
		mpz_mul(x, x, alpha);
		mod(x, n);
		mpz_add_ui(a, a, 1);
		mod(a, g);
	} else if (mpz_cmp_ui(t, 1) == 0)
	{
		mpz_mul(x, x, beta);
		mod(x, n);
		mpz_add_ui(b, b, 1);
		mod(b, g);
	}
	mpz_clear(t);
}

void DLP::rho( mpz_t alpha, mpz_t beta, mpz_t n, mpz_t g, mpz_t k )
{
	// find such k that G ^ k mod P = Y

	// we need to find such a,b,A,B that:
	// alpha^a * beta^b = alpha^A * beta^B

	// init variables
	mpz_t a,b,A,B;
	mpz_t x,X;
	if(mpz_cmp(alpha, beta) == 0)
	{
		mpz_set_ui(k, 1);
		return;
	}

	mpz_init_set_ui(a, 0);
	mpz_init_set_ui(b, 0);
	mpz_init_set_ui(A, 0);
	mpz_init_set_ui(B, 0);
	mpz_init_set_ui(x, 1);
	mpz_init_set_ui(X, 1);

	__int64 i = 0;

	do 
	{
		i++;
		iterate(x, a, b, n, g, alpha, beta);
		iterate(X, A, B, n, g, alpha, beta);
		iterate(X, A, B, n, g, alpha, beta);
	} while (mpz_cmp(x, X) != 0);

	mpz_sub(B, b, B);
	mod(B, g);
	mpz_invert(B, B, g);
	mpz_sub(A, A, a);
	mod(A, g);
	mpz_mul(k, A, B);
	mod(k, g);
	return;
}

char * DLP::Solve( char * alpha, char * beta, char *p )
{
	mpz_t a,b,q,n,phi,X;

	gmp_randinit_default(m_st);
	mpz_inits(a,b,q,n,phi,X,NULL);
	mpz_set_str(a, alpha, 16);
	mpz_set_str(b, beta, 16);
	mpz_set_str(q, p, 16);

	// if p is not prime, return error
	if (mpz_likely_prime_p(q, m_st, 0) == 0)
	{
		return NULL;
	}
	
	// if p is prime then order of that group is p-1
	mpz_set(phi, q);
	mpz_sub_ui(phi, phi, 1);

	// factorize p-1
	Factor *fp = new Factor(Factor::FP_BIG);
	if(fp==NULL)
		return NULL;
	fp->Start(phi);
	int factors = fp->GetNumFactors();

	if (factors > 255)
		return NULL;

	// maximum of 255 prime factors allowed
	mpz_t g[255];
	mpz_t y[255];
	mpz_t f[255];
	mpz_t x[255];
	mpz_t r;

	mpz_init(r);

	for(int j = 0; j<factors; j++)
	{
		mpz_inits(f[j], g[j], y[j], x[j], NULL);
		fp->GetFactor(f[j], j);
		if (mpz_divisible_p(phi, f[j]) == 0)
		{
			for (int i=0; i<=j; i++)
			{
				mpz_clears(f[j], g[j], y[j], x[j], NULL);
			}
			mpz_clear(r);
			delete fp;
			return NULL;
		}
		mpz_divexact(r, phi, f[j]);
		
		mpz_powm(g[j], a, r, q);
		mpz_powm(y[j], b, r, q);
		rho(g[j], y[j], q, f[j], x[j]);
	}

	// solve CRT
	mpz_init(X);
	CRT::Solve(X, x, f, phi, factors);
	mpz_t t1;
	mpz_init(t1);
	mpz_powm(t1, a, X, q);
	mpz_clears(a,q,n,phi,NULL);
	for (int j=0; j<factors; j++)
	{
		mpz_clears(f[j], g[j], y[j], x[j], NULL);
	}

	if(mpz_cmp(t1,b)!=0)
	{
		mpz_clears(b,t1,X,NULL);
		return NULL;
	}

	char *res = new char[255];
	mpz_get_str(res, 16, X);
	mpz_clears(b,t1,X,NULL);

	return res;
}