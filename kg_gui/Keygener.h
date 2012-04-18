#ifndef _KEYGENER_H_
#define _KEYGENER_H_

#include <string>

class CKeygener
{
protected:
	CKeygener(const CKeygener &);
	CKeygener & operator=(const CKeygener &);

public:
	CKeygener(){};

public:
	virtual ~CKeygener()
	{ }

public:

	// initialise the class
	virtual
	bool
	init(...) = 0;

	// generate serial from name and any additional data
	virtual
	bool
	GenerateSerial(
		std::string name,
		std::string& serial,
		...
	) = 0;

	// generate license from name and any additional data
	virtual
	bool
	GenerateLicense(
		std::string name,
		std::string license,
		...
	) = 0;

	// return the name of the keymaker
	virtual
	std::string
	GetName(
	) = 0;

};

typedef CKeygener *PCKeygener;

#endif // _KEYGENER_H_