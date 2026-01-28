// Crc32.h
//
#ifndef CORE_CRC32_H
#define CORE_CRC32_H

#include "s2Engine_API.h"

unsigned long S2ENGINE_API computeCRC32( const char *pBuffer, int len, unsigned int initValue = 0xFFFFFFFF, unsigned int poly = 0xEDB88320L );


#endif
