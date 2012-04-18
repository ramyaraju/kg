#include "ECDLP.h"

ECDLP::ECDLP(void)
{
}

ECDLP::~ECDLP(void)
{
}

bool ECDLP::init( char *a, char *b, char *q, int base )
{
	mpz_init_set_str(m_a, a, base);
	mpz_init_set_str(m_b, b, base);
	mpz_init_set_str(m_p, q, base);
	ecc = new ECC;
	ecc->init(16);
	ecc->SetDomain(m_a,m_b,m_p);
	m_base = base;
	return true;
}

bool ECDLP::init( mpz_t a, mpz_t b, mpz_t p )
{
	mpz_init_set(m_a, a);
	mpz_init_set(m_b, b);
	mpz_init_set(m_p, p);
	ecc = new ECC;
	ecc->init(0);
	m_base = 0;
	return true;
}

void ECDLP::rho( ECPoint &R, ECPoint &Q, mpz_t np, mpz_t k )
{
	// find such k that k*Q = R;
	int j;
	ECPoint X,Y,t1,t2;
	mpz_t c1,c2,d1,d2;

	mpz_t a[NB_BRANCHES], b[NB_BRANCHES];
	ECPoint P[NB_BRANCHES];

	// initialise variables
	mpz_inits(c1,c2,d1,d2,NULL);
	for (j=0; j<NB_BRANCHES; j++)
		mpz_inits(a[j], b[j], NULL);

	// initialise random number generator
	gmp_randinit_default(m_st);

	// generate c1 and c2 such that 0<c1,d1<np
	mpz_urandomm(c1, m_st, np);
	mpz_urandomm(d1, m_st, np);
	mpz_set_ui(k,0);

	// compute X,Y = c1*R + d1*Q
	ecc->MultiplePoint(c1, R, X);
	ecc->MultiplePoint(d1, Q, Y);
	ecc->AddPoints(X, Y, Y);
	ecc->CopyPoint(Y, X);
	mpz_set(c2, c1);
	mpz_set(d2, d1);

	for(j=0; j<NB_BRANCHES; j++)
	{
		mpz_urandomm(a[j],m_st,np);
		mpz_urandomm(b[j],m_st,np);
		ecc->MultiplePoint(a[j], R, t1);
		ecc->MultiplePoint(b[j], Q, t2);
		ecc->AddPoints(t1,t2,P[j]);
	}

	// find a cycle
	do 
	{
		j = mpz_tdiv_ui(X.x,NB_BRANCHES);
		ecc->AddPoints(P[j], X, X);
		mpz_add(c1, c1, a[j]);
		if (mpz_cmp_ui(c1,0)<0 || mpz_cmp(c1,np)>0)	mpz_mod(c1,c1,np);
		mpz_add(d1, d1, b[j]);
		if (mpz_cmp_ui(d1,0)<0 || mpz_cmp(d1,np)>0)	mpz_mod(d1,d1,np);
		for (int i=0; i<2; i++)
		{
			j = mpz_tdiv_ui(Y.x, NB_BRANCHES);
			ecc->AddPoints(P[j], Y, Y);
			mpz_add(c2, c2, a[j]);
			if (mpz_cmp_ui(c2,0)<0 || mpz_cmp(c2,np)>0)	mpz_mod(c2,c2,np);
			mpz_add(d2, d2, b[j]);
			if (mpz_cmp_ui(d2,0)<0 || mpz_cmp(d2,np)>0)	mpz_mod(d2,d2,np);
		}

	} while (mpz_cmp(X.x, Y.x)!=0);

	// compute (c1-c2)/(d2-d1) mod n
	mpz_sub(c1,c1,c2);
	mpz_sub(d2,d2,d1);
	mpz_invert(d2,d2,np);
	mpz_mul(c1,c1,d2);
	mpz_mod(k,c1,np);

	// cleanup
	mpz_clears(c1,c2,d1,d2,NULL);
	for (j=0; j<NB_BRANCHES; j++)
		mpz_clears(a[j],b[j],NULL);
	return;
}


void ECDLP::rho( char *nps, char *Qx, char *Qy, char *Rx, char *Ry, mpz_t k )
{
	if(!m_base)
		return;
	// find such k that k*Q = R;
	int j;
	ECPoint X,Y,t1,t2,R,Q;
	mpz_t c1,c2,d1,d2,np;

	mpz_t a[NB_BRANCHES], b[NB_BRANCHES];
	ECPoint P[NB_BRANCHES];

	// initialise variables
	mpz_inits(c1,c2,d1,d2,np,NULL);
	for (j=0; j<NB_BRANCHES; j++)
		mpz_inits(a[j], b[j], NULL);

	// initialise random number generator
	gmp_randinit_default(m_st);

	// generate c1 and c2 such that 0<c1,d1<np
	mpz_urandomm(c1, m_st, np);
	mpz_urandomm(d1, m_st, np);
	mpz_set_ui(k,0);
	ecc->SetPoint(Qx, Qy, Q);
	ecc->SetPoint(Rx, Ry, R);
	mpz_set_str(np, nps, m_base);

	// compute X,Y = c1*R + d1*Q
	ecc->MultiplePoint(c1, R, X);
	ecc->MultiplePoint(d1, Q, Y);
	ecc->AddPoints(X, Y, Y);
	ecc->CopyPoint(Y, X);
	mpz_set(c2, c1);
	mpz_set(d2, d1);

	for(j=0; j<NB_BRANCHES; j++)
	{
		mpz_urandomm(a[j],m_st,np);
		mpz_urandomm(b[j],m_st,np);
		ecc->MultiplePoint(a[j], R, t1);
		ecc->MultiplePoint(b[j], Q, t2);
		ecc->AddPoints(t1,t2,P[j]);
	}

	// find a cycle
	do 
	{
		j = mpz_tdiv_ui(X.x,NB_BRANCHES);
		ecc->AddPoints(P[j], X, X);
		mpz_add(c1, c1, a[j]);
		if (mpz_cmp_ui(c1,0)<0 || mpz_cmp(c1,np)>0)	mpz_mod(c1,c1,np);
		mpz_add(d1, d1, b[j]);
		if (mpz_cmp_ui(d1,0)<0 || mpz_cmp(d1,np)>0)	mpz_mod(d1,d1,np);
		for (int i=0; i<2; i++)
		{
			j = mpz_tdiv_ui(Y.x, NB_BRANCHES);
			ecc->AddPoints(P[j], Y, Y);
			mpz_add(c2, c2, a[j]);
			if (mpz_cmp_ui(c2,0)<0 || mpz_cmp(c2,np)>0)	mpz_mod(c2,c2,np);
			mpz_add(d2, d2, b[j]);
			if (mpz_cmp_ui(d2,0)<0 || mpz_cmp(d2,np)>0)	mpz_mod(d2,d2,np);
		}

	} while (mpz_cmp(X.x, Y.x)!=0);

	// compute (c1-c2)/(d2-d1) mod n
	mpz_sub(c1,c1,c2);
	mpz_sub(d2,d2,d1);
	mpz_invert(d2,d2,np);
	mpz_mul(c1,c1,d2);
	mpz_mod(k,c1,np);

	// cleanup
	mpz_clears(c1,c2,d1,d2,NULL);
	for (j=0; j<NB_BRANCHES; j++)
		mpz_clears(a[j],b[j],NULL);
	return;
}
