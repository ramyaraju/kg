#include "happytown26.h"

bool
happytown26::init( ... )
{
	m_ecdsa = new ECDSA;
	if (m_ecdsa == NULL)
	{
		return false;
	}
	// init the curve with base
	m_ecdsa->init(16);
	if (m_ecdsa->SetDomain("2982", "3408", "AEBF94CEE3E707", "7A3E808599A525", "28BE7FAFD2A052", "AEBF94D5C6AA71", GetTickCount()) == false)
	{
		delete m_ecdsa;
		m_ecdsa = NULL;
		return false;
	}

	if (m_ecdsa->SetKeys("9D3F1E3CDDA5E5", "9F70A02013BC9B", "9E0B275B93CF5E") == false)
	{
		delete m_ecdsa;
		m_ecdsa = NULL;
		return false;
	}
	gmp_randinit_default(m_st);
	return true;
}

bool
happytown26::GenerateSerial( string name, string& serial, ... )
{
	// generate hash
	unsigned char sha_digest[20];
	m_sha.Reset();
	m_sha << name.c_str();
	if ( !m_sha.Result(sha_digest) )
	{
		return false;
	}
	char sha1[42];
	memset(sha1, 0, 42);
	unsigned char *p = (unsigned char *)sha_digest;
	for (int i=0; i<20; i++)
	{
		unsigned char cl = *p;
		sha1[2*i] = btoa(cl>>4);
		sha1[2*i+1] = btoa(cl);
		p++;
	}
	char *r,*s;
	r = new char[40];
	if ( r == NULL )
	{
		return false;
	}

	s = new char[40];

	if ( s == NULL )
	{
		delete [] r;
		return false;
	}

	m_ecdsa->Sign(sha1, r, s);

	if ( !m_ecdsa->Verify(sha1, r, s) )
	{
		return false;
	}

	serial = r;
	serial += '-';
	serial += s;

	return true;
}

bool
happytown26::GenerateLicense( string name, string license, ... )
{
	// not implemented, not needed
	return false;
}

string
happytown26::GetName()
{
	return "HappyTown 026";
}

char
happytown26::btoa( char a )
{
	char cl = a & 0xF;
	if (cl <0xA)
	{
		return cl+0x30;
	}
	if ( cl >= 0xA )
	{
		return cl+0x37;
	}
	return 0;
}