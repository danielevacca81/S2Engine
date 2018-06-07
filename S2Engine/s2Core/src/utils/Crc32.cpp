/*----------------------------------------------------------------------------*
 *  Modulo per il calcolo del Crc32
 *----------------------------------------------------------------------------*/

#include "Crc32.h"

namespace s2 {

static unsigned long crcTable[256];

/* Genera i coefficienti della tabella per il calcolo del Crc32 */
static void _crcGen(unsigned int poly);


// ------------------------------------------------------------------------------------------------
unsigned long computeCRC32( const char *pBuffer, int len, unsigned int initValue, unsigned int poly )
{
	unsigned long crc;
	int c;
	int i;

	crc = initValue;
	c = 0;
	i = 0;

	_crcGen( poly );

	for( i = 0; i < len; i++ )
	{
		c = *pBuffer;
		crc = ( ( crc >> 8 ) & 0x00FFFFFF ) ^ crcTable[( crc^c ) & 0xFF];
		pBuffer++;
	}

	return( crc ^ 0xFFFFFFFF );
}

// ------------------------------------------------------------------------------------------------
void _crcGen( unsigned int poly )
{
	unsigned long crc;
	int i, j;

	for( i = 0; i < 256; i++ )
	{
		crc = i;
		for( j = 8; j > 0; j-- )
		{
			if( crc & 1 )
			{
				crc = ( crc >> 1 ) ^ poly;
			}
			else
			{
				crc >>= 1;
			}
		}
		crcTable[i] = crc;
	}
}

}