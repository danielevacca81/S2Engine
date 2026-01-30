// Crc32.cpp
//
#include "Crc32.h"

#include <numeric>
#include <algorithm>
#include <unordered_map>

// ------------------------------------------------------------------------------------------------
static inline std::vector<uint32_t> genCrcTable( uint32_t poly )
{
	static constexpr uint32_t kCrcTableSize { 256 };
	static std::unordered_map<uint32_t, std::vector<uint32_t>> crcTableCache;

	// Check if the table for this polynomial is already cached
	const auto it = crcTableCache.find(poly);
	if( it != crcTableCache.end() )
		return it->second;

	// not cached, generate the CRC table
	std::vector<uint32_t> crcTable(kCrcTableSize);
	std::iota(crcTable.begin(), crcTable.end(), 0); // Initialize with 0, 1, 2, ..., 255

	// Generate the CRC table based on the polynomial
	std::for_each( crcTable.begin(), crcTable.end(), [poly] ( auto& crc )
	{
		for( int i = 8; i > 0; --i )
		{
			if( crc & 1 ) crc = ( crc >> 1 ) ^ poly;
			else          crc >>= 1;
		}
	} );
	
	crcTableCache[poly] = std::move( crcTable );
	return crcTableCache[poly];
}

// ------------------------------------------------------------------------------------------------
namespace s2::Crc32 {

uint32_t compute( const std::vector<uint8_t> &data, uint32_t initValue, uint32_t poly )
{
	const auto crcTable = genCrcTable( poly );

	uint32_t crc = initValue;
	std::for_each( data.begin(), data.end(), [&crc, &crcTable] ( auto byte )
	{
		crc = ( crc >> 8 ) ^ crcTable[( crc^byte ) & 0xFF];
	} );

	return crc ^ 0xFFFFFFFF;
}

}