#include "ECC.h"

ECC::ECC(void)
{
}

ECC::~ECC(void)
{
}

void ECC::init(int base)
{
	mpz_inits(m_a, m_b, m_p, m_n, m_k, m_x, m_y, m_dA, NULL);
	gmp_randinit_default(m_st);
	m_base = base;
	m_isGenerator = false;
	return;
}

void ECC::clean()
{
	mpz_clears(m_a, m_b, m_p, m_n, m_k, m_x, m_y, m_dA, NULL);
	return;
}

bool ECC::SetDomain( char *a, char *b, char *p, char *Gx, char *Gy, char *n, unsigned int seed )
{
	mpz_set_str( m_a,	 a,		m_base );
	mpz_set_str( m_b,	 b,		m_base );
	mpz_set_str( m_p,	 p,		m_base );
	if ( Gx != NULL && Gy != NULL )
	{
		if ( !SetPoint(Gx, Gy, m_G) )
		{
			return false;
		}
		mpz_set_str( m_n,	 n,		m_base );
		m_isGenerator = true;
	}

	// check if the curve is correct by computing delta:
	// d = -16(4a^3+27b^2) != 0

	mpz_t delta;
	mpz_t tmp;
	mpz_inits(delta, tmp, NULL);

	mpz_powm_ui(delta, m_a, 3, m_p);
	mpz_mul_ui(delta, delta, 4);
	mpz_mod(delta, delta, m_p);
	mpz_mul(tmp, m_b, m_b);
	mpz_mod(tmp, tmp, m_p);
	mpz_mul_ui(tmp, tmp, 27);
	mpz_mod(tmp, tmp, m_p);
	mpz_add(delta, delta, tmp);
	mpz_mod(delta, delta, m_p);
	mpz_mul_si(delta, delta, -16);
	mpz_mod(delta, delta, m_p);
	mpz_clear(tmp);
	if ( mpz_cmp_ui(delta, 0) == 0 )
	{
		mpz_clear(delta);
		return false;
	}

	if ( Gx != NULL && Gy != NULL )
	{
		// check if point G lies on the curve.
		// y^2 = x^3 + ax^2 + b
		mpz_t x,y, left, right;
		mpz_inits(x,y,left,right,NULL);
		mpz_set(x, m_G.x);
		mpz_set(y, m_G.y);

		mpz_mul(left, y, y);
		mpz_mod(left, left, m_p);

		mpz_powm_ui(right, x, 3, m_p);
		mpz_mul(x, x, m_a);
		mpz_mod(x, x, m_p);
		mpz_add(x, x, m_b);
		mpz_mod(x, x, m_p);
		mpz_add(right, right, x);
		mpz_mod(right, right, m_p);

		if ( mpz_cmpabs(left, right) != 0 )
		{
			mpz_clears(x, y, left, right, NULL);
			return false;
		}
		mpz_clears(x, y, left, right, NULL);
	}
	return true;
}

bool ECC::SetDomain( mpz_t a, mpz_t b, mpz_t q, ECPoint &G, mpz_t n )
{
	mpz_set( m_a, a );
	mpz_set( m_b, b );
	mpz_set( m_p, q );
	CopyPoint(G, m_G);
	mpz_set( m_n, n );

	// check if the curve is correct by computing delta:
	// d = -16(4a^3+27b^2) != 0

	mpz_t delta;
	mpz_t tmp;
	mpz_inits(delta, tmp, NULL);

	mpz_powm_ui(delta, m_a, 3, m_p);
	mpz_mul_ui(delta, delta, 4);
	mpz_mod(delta, delta, m_p);
	mpz_mul(tmp, m_b, m_b);
	mpz_mod(tmp, tmp, m_p);
	mpz_mul_ui(tmp, tmp, 27);
	mpz_mod(tmp, tmp, m_p);
	mpz_add(delta, delta, tmp);
	mpz_mod(delta, delta, m_p);
	mpz_mul_si(delta, delta, -16);
	mpz_mod(delta, delta, m_p);
	mpz_clear(tmp);
	if ( mpz_cmp_ui(delta, 0) == 0 )
	{
		mpz_clear(delta);
		return false;
	}

	// check if point G lies on the curve.
	// y^2 = x^3 + ax^2 + b
	mpz_t x,y, left, right;
	mpz_inits(x,y,left,right,NULL);
	mpz_set(x, m_G.x);
	mpz_set(y, m_G.y);

	mpz_mul(left, y, y);
	mpz_mod(left, left, m_p);

	mpz_powm_ui(right, x, 3, m_p);
	mpz_mul(x, x, m_a);
	mpz_mod(x, x, m_p);
	mpz_add(x, x, m_b);
	mpz_mod(x, x, m_p);
	mpz_add(right, right, x);
	mpz_mod(right, right, m_p);

	if ( mpz_cmpabs(left, right) != 0 )
	{
		mpz_clears(x, y, left, right, NULL);
		return false;
	}
	mpz_clears(x, y, left, right, NULL);

	return true;
}

bool ECC::SetDomain( mpz_t a, mpz_t b, mpz_t q )
{
	mpz_set( m_a, a );
	mpz_set( m_b, b );
	mpz_set( m_p, q );

	// check if the curve is correct by computing delta:
	// d = -16(4a^3+27b^2) != 0

	mpz_t delta;
	mpz_t tmp;
	mpz_inits(delta, tmp, NULL);

	mpz_powm_ui(delta, m_a, 3, m_p);
	mpz_mul_ui(delta, delta, 4);
	mpz_mod(delta, delta, m_p);
	mpz_mul(tmp, m_b, m_b);
	mpz_mod(tmp, tmp, m_p);
	mpz_mul_ui(tmp, tmp, 27);
	mpz_mod(tmp, tmp, m_p);
	mpz_add(delta, delta, tmp);
	mpz_mod(delta, delta, m_p);
	mpz_mul_si(delta, delta, -16);
	mpz_mod(delta, delta, m_p);
	mpz_clear(tmp);
	if ( mpz_cmp_ui(delta, 0) == 0 )
	{
		mpz_clear(delta);
		return false;
	}

	return true;
}

void ECC::GetPoint( ECPoint &P, mpz_t &x, mpz_t &y)
{
	if(mpz_cmp(x,y) != 0)
	{
		mpz_set(y, P.y);
	}
	mpz_set(x, P.x);
	return;
}

bool ECC::SetPoint( char *Rx, char *Ry, ECPoint &R )
{
	mpz_t x,y, left, right;
	mpz_inits(x,y,left,right,NULL);
	mpz_set_str( x, Rx, m_base );
	mpz_set_str( y, Ry, m_base );

	mpz_mul(left, y, y);
	mpz_mod(left, left, m_p);

	mpz_powm_ui(right, x, 3, m_p);
	mpz_mul(x, x, m_a);
	mpz_mod(x, x, m_p);
	mpz_add(x, x, m_b);
	mpz_mod(x, x, m_p);
	mpz_add(right, right, x);
	mpz_mod(right, right, m_p);

	if ( mpz_cmpabs(left, right) != 0 )
	{
		mpz_clears(x, y, left, right, NULL);
		R.marker = 1;
		return false;
	}
	mpz_clears(x, y, left, right, NULL);

	mpz_set_str( R.x, Rx, m_base );
	mpz_set_str( R.y, Ry, m_base );
	R.marker = 0;

	return true;
}

void ECC::CopyPoint( ECPoint &P, ECPoint &R )
{
	R.marker = P.marker;
	mpz_set(R.x, P.x);
	mpz_set(R.y, P.y);
	return;
}

void ECC::ClearPoint( ECPoint &R )
{
	mpz_set_ui(R.x,0);
	mpz_set_ui(R.y,0);
	R.marker = 1;
	return;
}

int ECC::ComparePoints( ECPoint &P, ECPoint &R )
{
	int ret = -1;
	if (mpz_cmp(P.x,R.x) == 0)
	{
		ret = 0;
	}
	return ret;
}

bool ECC::SetKeys( char *dA, char *Rx, char *Ry )
{
	mpz_set_str(m_dA, dA, m_base);
	if ( !SetPoint(Rx, Ry, m_R) )
		return false;
	MultiplePoint(m_dA, m_G, m_P);
	if (ComparePoints(m_P, m_R) != 0)
	{
		ClearPoint(m_P);
		return false;
	}
	return true;
}

void ECC::MultiplePoint( mpz_t k, ECPoint &P, ECPoint &R )
{
	ECPoint Pi;
	mpz_t ki;
	mpz_init_set(ki, k);
	CopyPoint(P, Pi);
	ClearPoint(R);

	while (mpz_cmp_ui(ki, 0) > 0)
	{
		if (mpz_tstbit(ki, 0)==1)
			AddPoints(Pi, R, R);
		DoublePoint(Pi, Pi);
		mpz_div_2exp(ki, ki, 1);
	}
	mpz_clear(ki);
	return;
}

void ECC::DoublePoint( ECPoint &Pi, ECPoint &R )
{
	ECPoint P;
	CopyPoint(Pi, P);
	mpz_t s,inv,xt;
	mpz_inits(s,inv,xt,NULL);

	if (mpz_cmp_ui(P.x, 0) == 0)
	{
		mpz_set_ui(R.x, 0);
		mpz_set_ui(R.y, 1);
		R.marker = 1;
		mpz_clears(s,inv,xt,NULL);
		return;
	}

	// compute slope
	mpz_mul(s, P.x, P.x);
	mpz_mul_ui(s, s, 3);
	mpz_add(s, s, m_a);
	mpz_mul_ui(inv, P.y, 2);
	mpz_mod(inv, inv, m_p);
	mpz_invert(inv, inv, m_p);
	mpz_mul(s, s, inv);
	mpz_mod(s, s, m_p);

	// compute new coordinates
	mpz_mul(xt, s, s);
	mpz_mod(xt, xt, m_p);
	mpz_submul_ui(xt, P.x, 2);
	mpz_mod(xt, xt, m_p);
	mpz_set(R.x, xt);
	mpz_sub(xt, P.x, R.x);
	mpz_mul(xt, xt, s);
	mpz_sub(xt, xt, P.y);
	mpz_mod(xt, xt, m_p);
	mpz_set(R.y, xt);
	R.marker = 0;
	if ((mpz_cmp_ui(R.x,0)==0) && (mpz_cmp_ui(R.y,0)==0))
	{
		R.marker = 1;
	}
	mpz_clears(s,inv,xt,NULL);
	return;
}

void ECC::AddPoints( ECPoint &P, ECPoint &Q, ECPoint &R )
{
	mpz_t s,inv,xt;
	mpz_inits(s,inv,xt,NULL);
	if(P.marker)
	{
		R.marker = Q.marker;
		mpz_set(R.x, Q.x);
		mpz_set(R.y, Q.y);
		mpz_clears(s,inv,xt,NULL);
		return;
	} else if (Q.marker)
	{
		R.marker = P.marker;
		mpz_set(R.x, P.x);
		mpz_set(R.y, P.y);
		mpz_clears(s,inv,xt,NULL);
		return;
	}
	if (
		(mpz_cmp(P.x, Q.x)==0) &&
		(mpz_cmp(P.y, Q.y)==0)
		)
	{
		DoublePoint(P, R);
		mpz_clears(s,inv,xt,NULL);
		return;
	}

	// slope
	mpz_sub(xt, Q.x, P.x);
	mpz_mod(xt, xt, m_p);
	mpz_invert(inv, xt, m_p);
	mpz_sub(s, Q.y, P.y);
	mpz_mul(s, s, inv);
	mpz_mod(s, s, m_p);

	// set x
	mpz_mul(xt, s, s);
	mpz_sub(xt, xt, P.x);
	mpz_sub(xt, xt, Q.x);
	mpz_mod(xt, xt, m_p);
	mpz_set(R.x, xt);

	// set y
	mpz_sub(xt, P.x, R.x);
	mpz_mul(xt, xt, s);
	mpz_sub(xt, xt, P.y);
	mpz_mod(xt, xt, m_p);
	mpz_set(R.y, xt);
	R.marker = 0;
	if ((mpz_cmp_ui(R.x,0)==0) && (mpz_cmp_ui(R.y,0)==0))
	{
		R.marker = 1;
	}
	mpz_clears(s,inv,xt,NULL);
	return;
}

bool ECC::IsAtInfinity( ECPoint &P )
{
	if (P.marker == 0)
		return false;
	return true;
}
