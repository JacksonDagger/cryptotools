#ifndef CRC_H

#include <stdlib.h>
#include <stdint.h>

extern const uint32_t crc_table[256];
extern const uint8_t crc_lmb_rev_lut[256];

void spec_crc_collision(uint32_t crc32);
void crc_4_char(uint32_t init_crc32, uint32_t end_crc32);
void char_seq(uint32_t crc32, uint8_t indices[], unsigned char chars[]);
void index_order(uint32_t crc32, uint8_t indices[]);
unsigned char rand_char(void);
void crc_collision(void);
uint32_t crc32_update(uint32_t crc, const void *data, size_t data_len);

#endif
