#include "Factor.h"

void db(char *frm, mpz_t a, int base)
{
	char bb[255];
	mpz_get_str(bb,base,a);
	OutputDebugString(frm);
	OutputDebugString(bb);
	OutputDebugString("\n");
}

void __cdecl odprintf(const char *format, ...)
{
	char    buf[4096], *p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = _vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while ( p > buf  &&  isspace(p[-1]) )
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p   = '\0';

	OutputDebugString(buf);
}

Factor::Factor(void)
{
	m_mode = FP_STR;
	fs = new char *[256];
	gmp_randinit_default(m_st);
	m_factors = 0;
}

Factor::Factor(int mode)
{
	if (mode == FP_STR)
	{
		fs = new char *[256];
	} else if (mode == FP_BIG)
	{
		fb = new mpz_t[256];
	}
	m_mode = mode;
	gmp_randinit_default(m_st);
	m_factors = 0;
}

Factor::~Factor(void)
{
	for(int i=0; i<m_factors; i++)
	{
		delete [] fs[m_factors];
	}
	delete [] fs;
}

void Factor::f( mpz_t rop, mpz_t a, mpz_t n )
{
	mpz_mul(rop, rop, rop);
	mpz_add_ui(rop, rop, 0xDEADBEEF);
	mpz_mod(rop, rop, n);
}

void Factor::brent( mpz_t n, mpz_t factor )
{
	mpz_t x,y,d;
	mpz_t t;

	mpz_init_set_ui(t, 0);
	mpz_init_set_ui(x, 2);
	mpz_init_set_ui(y, 2);
	mpz_init_set_ui(d, 1);

	while( mpz_cmp_ui(d, 1) == 0)
	{
		f(x, x, n);
		f(y, y, n);
		f(y, y, n);
		mpz_sub(t, y, x);
		mpz_abs(t, t);
		mpz_gcd(d, t, n);
		if (mpz_cmp(d, n) == 0)
		{
			mpz_set(factor, n);
			return;
		}
	}

	mpz_set(factor, d);
	return;
}

bool Factor::Start( char *n )
{
	if (m_mode != FP_STR)
		return false;
	mpz_t p, t;
	mpz_init_set_str(p, n, 16);
	mpz_init_set_ui(t, 0);


	while( mpz_cmp_ui(p, 1) != 0)
	{
		brent(p, t);
		if((mpz_likely_prime_p(t, m_st, 0) == 0) ||
			(mpz_divisible_p(p, t) == 0))
		{
			mpz_clears(p, t, NULL);
			return false;
		}
		fs[m_factors] = new char[255];
		mpz_get_str(fs[m_factors], 16, t);
		m_factors++;
		mpz_divexact(p, p, t);
	}
	return true;
}

bool Factor::Start( mpz_t n )
{
	if (m_mode != FP_BIG)
		return false;

	mpz_t p, t;
	mpz_init_set(p, n);
	mpz_init_set_ui(t, 0);


	while( mpz_cmp_ui(p, 1) != 0)
	{
		brent(p, t);
		if((mpz_likely_prime_p(t, m_st, 0) == 0) ||
			(mpz_divisible_p(p, t) == 0))
		{
			mpz_clears(p, t, NULL);
			return false;
		}
		mpz_init(fb[m_factors]);
		mpz_set(fb[m_factors], t);
		m_factors++;
		mpz_divexact(p, p, t);
	}
	return true;
}
int Factor::GetNumFactors()
{
	return m_factors;
}

char * Factor::GetFactor( int i )
{
	return fs[i];
}

void Factor::GetFactor( mpz_t rop, int i )
{
	mpz_set(rop, fb[i]);
}