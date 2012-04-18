#ifndef CRC64_H
#define CRC64_H

#include <stdio.h>
#include <Windows.h>

extern VOID CRC64_gen(UINT64 poly);
extern UINT64 crc64(PBYTE stream, UINT n);
extern UINT64 rev_crc64(UINT64 crc, UINT64 target);
extern UINT64 bcrc64(PBYTE stream, UINT n, UINT k, UINT64 fcrc);

#endif // CRC64_H
