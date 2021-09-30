#ifndef CRC_H

#include <stdlib.h>
#include <stdint.h>

extern const uint32_t crc_table[256];
extern const uint8_t crc_lmb_rev_lut[256];

unsigned char rand_char(void);
void crc_collision(void);
uint32_t crc32_update(uint32_t crc, const void *data, size_t data_len);

#endif
