// Crc32.h
//
#ifndef CORE_CRC32_H
#define CORE_CRC32_H

#include "s2Engine_API.h"

#include <cstdint>
#include <vector>

namespace s2 {
namespace Crc32 {

uint32_t S2ENGINE_API compute( const std::vector<uint8_t> &data, uint32_t initValue = 0xFFFFFFFF, uint32_t poly = 0xEDB88320L );

}
}

#endif
