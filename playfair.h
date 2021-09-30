#ifndef PLAYFAIR_H
#define PLAYFAIR_H

struct params{
    const char* ciphertext;
    int len;
    char* key;
    double* retmae;
};
struct g2_freq {
    char gram[3];
    double freq;
};
struct g3_freq {
    char gram[4];
    double freq;
};
struct g4_freq {
    char gram[5];
    double freq;
};
struct g5_freq {
    char gram[6];
    double freq;
};

void *threaded_rand(void *_args);
double test_rand_keys(const char *ciphertext, int len, char *key);
double test_rand_key(const char *ciphertext, int len, char *key);
void random_swap(char *key);
void random_key(char *key);
double mae(char key[], const char *ciphertext, int len);
double digram_mae(char* plaintext, int len);
double inline_digram_mae(char* plaintext, int len);
double trigram_mae(char* plaintext, int len);
double quadrigram_mae(char* plaintext, int len);
double quintigram_mae(char* plaintext, int len);
char* playfair_decode(char key[], const char *ciphertext, int len);
int digram_decrypt(const char * in, char * out, const int key_mappings[25], const char key[]);
int char_pos(char in);
int pos_char(int pos);

extern struct g2_freq inline_digrams[256];
extern struct g2_freq digrams[256];
extern struct g3_freq trigrams[256];
extern struct g4_freq quadrigrams[256];
extern struct g5_freq quintigrams[256];


#endif