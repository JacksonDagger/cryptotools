#ifndef CRC_H

#include <stdlib.h>
#include <stdint.h>

static const uint32_t crc_table[256];

uint32_t crc32_update(uint32_t crc, const void *data, size_t data_len);

#endif
