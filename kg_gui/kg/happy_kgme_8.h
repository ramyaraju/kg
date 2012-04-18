#ifndef _HAPPY_KGME_8_H_
#define _HAPPY_KGME_8_H_

#include "../keygener.h"
#include <string>
#include <Windows.h>
#pragma warning(disable: 4800)
#include <mpirxx.h>
#include "../libs/ECC.h"
#pragma warning(default: 4800)

#pragma comment(lib, "mpirxx.lib")
#pragma comment(lib, "mpir.lib")

using namespace std;

class happy_kgme_8 : public CKeygener
{
protected:
	happy_kgme_8(const happy_kgme_8 &);
	happy_kgme_8 & operator=(const happy_kgme_8 &);

public:
	happy_kgme_8(void){
		m_ecc = NULL;
	};
	
	virtual ~happy_kgme_8(void){
		delete m_ecc;
	};

public:

	virtual
	bool
	init(...);

	// generate serial from name and any additional data
	virtual
	bool
	GenerateSerial(
		string name,
		string& serial,
		...
	);

	// generate license from name and any additional data
	virtual
	bool
	GenerateLicense(
		string name,
		string license,
		...
	);

	virtual
	string
	GetName(
	);

private:
	mpz_class
	GenerateHash(
		string name
	);

private:
	ECC				*m_ecc;
	gmp_randstate_t	m_st;

};

#endif // _HAPPY_KGME_8_H_