#ifndef _GAUSS_H_
#define _GAUSS_H_

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "mpir.h"

#pragma comment(lib, "mpir.lib")

// group operations
int gauss_addmod(int a, int b, int p);
int gauss_submod(int a, int b, int p);
int gauss_mulmod(int a, int b, int p);
int gauss_divmod(int a, int b, int p);
int gauss_powmod(int a, int b, int p);
int gauss_invmod(int a, int p);
int gauss_egcd(int a, int b, int &x, int &y);
void gauss(int (*A)[7],	int n, int p, bool &err);

#endif