#include "happy_kgme_8.h"

bool
happy_kgme_8::init(...)
{
	m_ecc = new ECC;
	if (m_ecc == NULL)
	{
		return false;
	}
	// init the curve with base
	m_ecc->init(16);
	if (m_ecc->SetDomain("-3", "265CB85B78B5E", "2C7DF84AB895F", "A78BC7D44A14", "261D23CFB8C77", "7A7908EDC89", GetTickCount()) == false)
	{
		delete m_ecc;
		m_ecc = NULL;
		return false;
	}

	if (m_ecc->SetKeys("19EE272D8F33F", "1B30A0494193F", "1A078B0630B9F") == false)
	{
		delete m_ecc;
		m_ecc = NULL;
		return false;
	}
	gmp_randinit_default(m_st);
	return true;
}

bool 
happy_kgme_8::GenerateSerial( string name, string& serial, ... )
{
	if (m_ecc == NULL)
	{
		return false;
	}
	mpz_class h = GenerateHash(name);
	
	// sign the hash
	ECPoint Gi;
	mpz_class k,n,r,s,x;

	n = mpz_class (m_ecc->m_n);
	mpz_urandomm(k.get_mpz_t(), m_st, n.get_mpz_t());

	// compute r = k*G+h (mod n)
	m_ecc->MultiplePoint(k.get_mpz_t(), m_ecc->m_G, Gi);
	x = mpz_class(Gi.x);
	r = (x+h)%n;
	if (r < 0)
	{
		r += n;
	}
	string rr = r.get_str(-16);

	// compute s = k-r*dA
	x = mpz_class(m_ecc->m_dA);
	s = (k-r*x)%n;
	if (s < 0)
	{
		s += n;
	}
	string ss = s.get_str(-16);

	if (rr.length() > 16 || ss.length() > 16)
	{
		return false;
	}
	serial.resize(32, '0');
	copy(rr.begin(),rr.end(),serial.begin()+16-rr.size());
	copy(ss.begin(),ss.end(),serial.begin()+32-ss.size());
	return true;
}

bool
happy_kgme_8::GenerateLicense( string name, string license, ... )
{
	return NULL;
}

mpz_class 
happy_kgme_8::GenerateHash( string name )
{
	mpz_class c,s,t,m;
	c = name.length();
	s = 1;
	t = 0;
	m = 3890356259;
	for (unsigned int i=0; i<name.length(); i++)
	{
		s = s*c;
		t = name[i];
		s = s+t;
		if (s > m)
			break;
	}
	return s;
}

string
happy_kgme_8::GetName()
{
	return "Happy KeygenMe #8";
}