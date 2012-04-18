#include "ECDSA.h"

ECDSA::ECDSA(void)
{
}

ECDSA::~ECDSA(void)
{
}
// sign a hash using ECDSA, hash must be a string in base used during initialisation.
// private key and a generator point are required for it to work (look SetDomain 
// and SetKeys)
void ECDSA::Sign( char *hash, char *&r, char *&s )
{
	mpz_t z,k,rr,ss,t,u;
	ECPoint kG;

	mpz_init_set_str(z, hash, m_base);
	mpz_inits(k, rr, ss, t, u, NULL);

	do 
	{
		mpz_urandomm(k, m_st, ecc->m_n);
		ecc->MultiplePoint(k, ecc->m_G, kG);
		mpz_set(rr, kG.x);
		mpz_mod(rr, rr, ecc->m_n);
	} while (!mpz_cmp_ui(rr, 0));

	do 
	{
		mpz_set(t,z);
		mpz_addmul(t,rr,ecc->m_dA);
		mpz_set(u,k);
		mpz_invert(u,u,ecc->m_n);
		mpz_mul(ss, t, u);
		mpz_mod(ss, ss, ecc->m_n);
	} while (!mpz_cmp_ui(ss, 0));

	mpz_get_str(r, m_base, rr);
	mpz_get_str(s, m_base, ss);

	mpz_clears(z,k,rr,ss,t,u,NULL);

	return;
}

// verify signature on a hash using ECDSA, public key and a generator point are required.
bool ECDSA::Verify( char *hash, char *r, char *s )
{
	mpz_t z, rr, ss, w, u1, u2;
	ECPoint uG, uQ, kG;

	mpz_init_set_str(z, hash, m_base);
	mpz_init_set_str(rr, r, m_base);
	mpz_init_set_str(ss, s, m_base);
	mpz_inits(w, u1, u2, NULL);

	// check if public key is not a point at infinity
	if (ecc->IsAtInfinity(ecc->m_R))
		return false;

	if (
		(mpz_cmp_ui(rr, 0)<=0) ||
		(mpz_cmp(rr, ecc->m_n)>=0) ||
		(mpz_cmp_ui(ss, 0)<=0) ||
		(mpz_cmp(ss, ecc->m_n)>=0)
		)
	{
		return false;
	}

	// calculate w=s^-1(mod n)
	mpz_invert(w, ss, ecc->m_n);

	// calculate:
	// u1 = zw (mod n)
	// u2 = rw (mod n)
	mpz_mul(u1, z, w);
	mpz_mod(u1, u1, ecc->m_n);
	mpz_mul(u2, rr, w);
	mpz_mod(u2, u2, ecc->m_n);
	ecc->MultiplePoint(u1, ecc->m_G, uG);
	ecc->MultiplePoint(u2, ecc->m_R, uQ);
	ecc->AddPoints(uG, uQ, uQ);
	mpz_set(w, uQ.x);
	mpz_mod(w, w, ecc->m_n);

	if (!mpz_cmp(w,rr))
		return true;

	return false;
}

bool ECDSA::init(int base)
{
	ecc = new ECC;
	if (ecc == NULL)
		return false;
	gmp_randinit_default(m_st);
	ecc->init(base);
	m_base = base;
	return true;
}

bool ECDSA::Clean()
{
	delete ecc;
	return true;
}

bool ECDSA::SetDomain( char *a, char *b, char *p, char *Gx, char *Gy, char *n, unsigned int seed )
{
	return ecc->SetDomain(a, b, p, Gx, Gy, n, seed);
}

bool ECDSA::SetKeys( char *dA, char *Rx, char *Ry )
{
	return ecc->SetKeys(dA, Rx, Ry);
}
