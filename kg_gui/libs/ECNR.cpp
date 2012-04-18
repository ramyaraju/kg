#include "ECNR.h"

ECNR::ECNR(void)
{
	ecc = NULL;
}

ECNR::~ECNR(void)
{
	ecc = NULL;
}

// sign a hash using Elliptic Curve Nyberg-Rueppel signature scheme with a 
// random key k
bool ECNR::Sign( unsigned int hash, char *&rs, char *&ss )
{
	// generate random 0<k<n
	mpz_t h;
	mpz_t k, n, r, s;
	ECPoint Gi;

	mpz_inits(k,n,r,s,NULL);
	mpz_set(n, ecc->m_n);
	
	mpz_urandomm(k, m_st, n);
	mpz_init_set_ui(h, hash);

	// compute r = k*G+h (mod n)
	ecc->MultiplePoint(k, ecc->m_G, Gi);

	mpz_add(r, Gi.x, h);
	mpz_mod(r, r, n);

	// export r
	mpz_get_str(rs, m_base, r);

	// compute s = k-r*dA (mod n)
	mpz_set(s, k);
	mpz_submul(s, r, ecc->m_dA);
	mpz_mod(s, s, n);

	// export s
	mpz_get_str(ss, m_base, s);
	mpz_clears(h,k,n,r,s,NULL);
	return true;
}


bool ECNR::Sign( char *hash, char *&rs, char *&ss )
{
	// generate random 0<k<n
	mpz_t h;
	mpz_t k, n, r, s;
	ECPoint Gi;

	mpz_inits(k,n,r,s,NULL);
	mpz_set(n, ecc->m_n);

	mpz_urandomm(k, m_st, n);
	mpz_init_set_str(h, hash, m_base);

	// compute r = k*G+h (mod n)
	ecc->MultiplePoint(k, ecc->m_G, Gi);

	mpz_add(r, Gi.x, h);
	mpz_mod(r, r, n);

	// export r
	mpz_get_str(rs, m_base, r);

	// compute s = k-r*dA (mod n)
	mpz_set(s, k);
	mpz_submul(s, r, ecc->m_dA);
	mpz_mod(s, s, n);

	// export s
	mpz_get_str(ss, m_base, s);
	mpz_clears(h,k,n,r,s,NULL);
	return true;
}

// sign a hash using Elliptic Curve Nyberg-Rueppel signature scheme with a
// specific key k
bool ECNR::Sign( char *hash, char *&rs, char *&ss, char *ks )
{
	mpz_t h;
	mpz_t k, n, r, s;
	ECPoint Gi;
	mpz_init_set_str(h, hash, m_base);
	mpz_inits(k,n,r,s,NULL);
	mpz_set(n, ecc->m_n);
	mpz_set_str(k, ks, m_base);
	// make sure k is (mod n)
	mpz_mod(k,k,n);

	// compute r = k*G+h (mod n)
	ecc->MultiplePoint(k, ecc->m_G, Gi);
	mpz_add(r, Gi.x, h);
	mpz_mod(r, r, n);

	// export r
	mpz_get_str(rs, m_base, r);

	// compute s = k-r*dA (mod n)
	mpz_set(s, k);
	mpz_submul(s, r, ecc->m_dA);
	mpz_mod(s, s, n);

	// export s
	mpz_get_str(ss, m_base, s);
	mpz_clears(h,k,n,r,s,NULL);
	return true;
}

// verify the signature with the hash
bool ECNR::Verify( unsigned int hash, char *rs, char *ss )
{
	mpz_t h,r,s,v,n;
	ECPoint t1,t2;

	mpz_init_set_str(r, rs, m_base);
	mpz_init_set_str(s, ss, m_base);
	mpz_init_set_ui(h, hash);
	mpz_init_set(n, ecc->m_n);
	mpz_init(v);

	ecc->MultiplePoint(s, ecc->m_G, t1);
	ecc->MultiplePoint(r, ecc->m_R, t2);
	ecc->AddPoints(t2, t1, t1);
	ecc->GetPoint(t1, v, v);
	mpz_sub(r, r, v);
	mpz_mod(r, r, n);

	if ( mpz_cmp(r,h) == 0 )
		return true;
		
	return false;
}

bool ECNR::Verify( char *hash, char *rs, char *ss )
{
	mpz_t h,r,s,v,n;
	ECPoint t1,t2;
	char tmp[50];

	mpz_init_set_str(r, rs, m_base);
	mpz_init_set_str(s, ss, m_base);
	mpz_init_set_str(h, hash, m_base);
	mpz_init_set(n, ecc->m_n);
	mpz_init(v);

	ecc->MultiplePoint(s, ecc->m_G, t1);
	ecc->MultiplePoint(r, ecc->m_R, t2);
	ecc->AddPoints(t2, t1, t1);
	ecc->GetPoint(t1, v, v);
	mpz_get_str(tmp, m_base, v);
	mpz_sub(r, r, v);
	mpz_mod(r, r, n);

	if ( mpz_cmp(r,h) == 0 )
		return true;

	return false;
}

bool ECNR::init(int base)
{
	ecc = new ECC;
	if (ecc == NULL)
		return false;
	gmp_randinit_default(m_st);
	ecc->init(base);
	m_base = base;
	return true;
}

bool ECNR::Clean()
{
	delete ecc;
	return true;
}

bool ECNR::SetDomain( char *a, char *b, char *p, char *Gx, char *Gy, char *n, unsigned int seed )
{
	return ecc->SetDomain(a, b, p, Gx, Gy, n, seed);
}

bool ECNR::SetKeys( char *dA, char *Rx, char *Ry )
{
	return ecc->SetKeys(dA, Rx, Ry);
}
