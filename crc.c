/**
 * \file
 * Functions and types for CRC checks.
 *
 * Generated on Mon Sep 27 14:34:35 2021
 * by pycrc v0.9.2, https://pycrc.org
 * using the configuration:
 *  - Width         = 32
 *  - Poly          = 0x04c11db7
 *  - XorIn         = 0xffffffff
 *  - ReflectIn     = True
 *  - XorOut        = 0xffffffff
 *  - ReflectOut    = True
 *  - Algorithm     = table-driven
 */
#include "crc.h" // made my own crc.h for simplicity
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_CRC 0xFFFFFFFF
#define FOUND_TABLE_LEN 0x00FFFFFF
#define FOUND_TABLE_WIDTH 8
#define CHAR_ARR_LEN FOUND_TABLE_WIDTH - 1
#define INDEX_MASK 0xFFFFFF00
#define INDEX_SHIFT 8

#define FIRST_CHAR 32 // range of printable chars
#define LAST_CHAR 126

#define BACKTRACK_LEN 4
#define LMB_MASK 0xFF000000
#define LMB_SHIFT 24

#define INDEX(CRC_VAL) ((CRC_VAL & INDEX_MASK) >> INDEX_SHIFT)
#define REMAINDER(CRC_VAL) ((uint8_t) (CRC_VAL & (~INDEX_MASK)))

#define LMB(CRC_VAL) ((CRC_VAL & LMB_MASK) >> LMB_SHIFT)

#define STUD_NO_MD5 "D47BB709FB24B803882325485E1253D8"
#define CRC_STUD_NO_MD5 0x9228124e // used pycrc.py

static unsigned char values[FOUND_TABLE_LEN][FOUND_TABLE_WIDTH];

int main(void){
    time_t t;
    srand((unsigned) time(&t));
    //crc_4_char(DEFAULT_CRC, CRC_STUD_NO_MD5);
    //crc_collision();
    spec_crc_collision(CRC_STUD_NO_MD5);
}

/* generates and prints a 4 byte sequence that can
* be appended to any object with crc32=init_crc32 to
* make and object with crc32=end_crc32
*/
void crc_4_char(uint32_t init_crc32, uint32_t end_crc32){
    clock_t begin = clock();
    uint8_t indices[BACKTRACK_LEN];
    unsigned char chars[BACKTRACK_LEN+1] = {0};
    index_order(end_crc32, indices);
    char_seq(init_crc32, indices, chars);
    clock_t end = clock();
    double total_runtime = 1000.0*(end - begin) / (double) CLOCKS_PER_SEC;
    printf("Hex values of chars. \n");
    for (int i = 0; i < BACKTRACK_LEN; i++){
        printf("%x\n", chars[i] & 0xff);
    }
    printf("%s", chars);
    printf("Took %lf milliseconds to calculate\n", total_runtime);
}

void char_seq(uint32_t crc32, uint8_t indices[BACKTRACK_LEN], unsigned char chars[BACKTRACK_LEN]){
    for(int i = 0; i < BACKTRACK_LEN; i++) {
        chars[i] = (crc32 & 0xff) ^ indices[i];
        crc32 = crc_table[indices[i]] ^ (crc32 >> 8);
    }
}

void index_order(uint32_t crc32, uint8_t indices[BACKTRACK_LEN]){
    uint8_t tbl_index;
    uint8_t lmb;

    for(int i = BACKTRACK_LEN - 1; i >= 0; i--) {
        lmb = LMB(crc32);
        tbl_index = crc_lmb_rev_lut[lmb];
        crc32 = (crc32^crc_table[tbl_index]) << 8;
        indices[i] = tbl_index;
    }
}

void spec_crc_collision(uint32_t crc32){
    clock_t begin = clock();
    unsigned char rand_seq[9] = {0};
    unsigned long tries = 0;

    uint32_t test_crc32;

    while (1) {
        tries ++;
        for (int i = 0; i < 8; i++){
            rand_seq[i] = rand_char();
        }
        test_crc32 = crc32_update(DEFAULT_CRC, (void *) rand_seq, 8);
        if (test_crc32 == crc32){
            break;
        }
    }
    clock_t end = clock();
    double total_runtime = 1000.0*(end - begin) / (double) CLOCKS_PER_SEC;
    printf("The colliding string is \"%s\"\n", rand_seq);
    printf("The program ran in %lf ms and looked at %ld strings\n", total_runtime, tries);
}
void crc_collision(void){
    clock_t begin = clock();
    memset(values, 0, FOUND_TABLE_LEN*FOUND_TABLE_WIDTH);
    uint32_t crc32;
    unsigned char rand_seq[CHAR_ARR_LEN + 1] = {0};
    unsigned char col_seq[CHAR_ARR_LEN + 1] = {0};

    uint32_t index;
    uint8_t rmb;

    uint32_t tries = 0;
    uint32_t part_collisions = 0;
    uint32_t same_vals = 0;
    clock_t loop_begin = clock();
    while (1) {
        tries ++;
        for (int i = 0; i < CHAR_ARR_LEN; i++){
            rand_seq[i] = rand_char();
        }
        crc32 = crc32_update(DEFAULT_CRC, (void *) rand_seq, CHAR_ARR_LEN);
        index = INDEX(crc32);
        rmb = REMAINDER(crc32);
        if (!values[index][0]){
            values[index][0] = (unsigned char) rmb;
            memcpy((void *)&values[index][1], (void *)rand_seq, CHAR_ARR_LEN);
        }
        else if ((uint8_t) values[index][0] == rmb){
            if (memcmp((void *)rand_seq, (void *)&values[index][1], CHAR_ARR_LEN)){
                memcpy((void *)col_seq, (void *)&values[index][1], CHAR_ARR_LEN);
                break;
            }
            else {
                same_vals ++;
            }
        }
        else {
            part_collisions ++;
        }
    }
    clock_t end = clock();
    double total_runtime = 1000.0*(end - begin) / (double) CLOCKS_PER_SEC;
    double loop_runtime = 1000.0*(end - loop_begin) / (double) CLOCKS_PER_SEC;
    printf("The two colliding strings are \"%s\" and \"%s\"\n", rand_seq, col_seq);
    printf("The program ran in %lf ms with the main loop taking %lf ms\n", total_runtime, loop_runtime);
    printf("%u random values were generated, of which %u were discarded for partial collisions and %u were values already generated.\n", tries, part_collisions, same_vals);
}

inline unsigned char rand_char(void){
    return rand()%(LAST_CHAR - FIRST_CHAR + 1) + FIRST_CHAR;
}

/* adapted from pycrc version */
uint32_t crc32_update(uint32_t crc, const void *data, size_t data_len)
{
    const unsigned char *d = (const unsigned char *)data;
    unsigned int tbl_idx;

    while (data_len--) {
        tbl_idx = (crc ^ *d) & 0xff;
        crc = crc_table[tbl_idx] ^ (crc >> 8);
        d++;
    }
    return crc;
}

/**
 * table used for the table_driven implementation.
 */
const uint32_t crc_table[256] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
    0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
    0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
    0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
    0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
    0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
    0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
    0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
    0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
    0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
    0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
    0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
    0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
    0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
    0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
    0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
    0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

/**
 * Array crc_table indices indexed by left-most-byte
 */
const uint8_t crc_lmb_rev_lut[256] = {
    0, 65, 195, 130, 134, 199, 69, 4,
    77, 12, 142, 207, 203, 138, 8, 73,
    154, 219, 89, 24, 28, 93, 223, 158,
    215, 150, 20, 85, 81, 16, 146, 211,
    117, 52, 182, 247, 243, 178, 48, 113,
    56, 121, 251, 186, 190, 255, 125, 60,
    239, 174, 44, 109, 105, 40, 170, 235,
    162, 227, 97, 32, 36, 101, 231, 166,
    234, 171, 41, 104, 108, 45, 175, 238,
    167, 230, 100, 37, 33, 96, 226, 163,
    112, 49, 179, 242, 246, 183, 53, 116,
    61, 124, 254, 191, 187, 250, 120, 57,
    159, 222, 92, 29, 25, 88, 218, 155,
    210, 147, 17, 80, 84, 21, 151, 214,
    5, 68, 198, 135, 131, 194, 64, 1,
    72, 9, 139, 202, 206, 143, 13, 76,
    149, 212, 86, 23, 19, 82, 208, 145,
    216, 153, 27, 90, 94, 31, 157, 220,
    15, 78, 204, 141, 137, 200, 74, 11,
    66, 3, 129, 192, 196, 133, 7, 70,
    224, 161, 35, 98, 102, 39, 165, 228,
    173, 236, 110, 47, 43, 106, 232, 169,
    122, 59, 185, 248, 252, 189, 63, 126,
    55, 118, 244, 181, 177, 240, 114, 51,
    127, 62, 188, 253, 249, 184, 58, 123,
    50, 115, 241, 176, 180, 245, 119, 54,
    229, 164, 38, 103, 99, 34, 160, 225,
    168, 233, 107, 42, 46, 111, 237, 172,
    10, 75, 201, 136, 140, 205, 79, 14,
    71, 6, 132, 197, 193, 128, 2, 67,
    144, 209, 83, 18, 22, 87, 213, 148,
    221, 156, 30, 95, 91, 26, 152, 217
};

