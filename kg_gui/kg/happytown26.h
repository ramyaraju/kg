
#ifndef _HAPPYTOWN_26_H_
#define _HAPPYTOWN_26_H_

#include "../keygener.h"
#include <string>
#include <Windows.h>
#pragma warning(disable: 4800)
#include <mpirxx.h>
#include "../libs/ECDSA.h"
#pragma warning(default: 4800)

#include "../libs/sha1.h"

#pragma comment(lib, "mpirxx.lib")
#pragma comment(lib, "mpir.lib")

using namespace std;

class happytown26 :
	public CKeygener
{

protected:
	happytown26(const happytown26 &);
	happytown26 & operator=(const happytown26 &);

public:
	happytown26(void) {
		m_ecdsa = NULL;
	}

	virtual ~happytown26(void) {
		m_ecdsa->Clean();
		delete m_ecdsa;
	}

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
	char
		btoa(
		char a
		);

private:
	ECDSA			*m_ecdsa;
	SHA1			m_sha;
	gmp_randstate_t	m_st;
};

#endif // _HAPPYTOWN_26_H_