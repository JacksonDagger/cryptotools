#ifndef PLAYFAIR_H
#define PLAYFAIR_H

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

double test_rand_keys(char *ciphertext, int len, char *key);
double test_rand_key(char *ciphertext, int len, char *key);
void random_swap(char *key);
void random_key(char *key);
double mae(char key[], char* ciphertext, int len);
double digram_mae(char* plaintext, int len);
double inline_digram_mae(char* plaintext, int len);
double trigram_mae(char* plaintext, int len);
char* playfair_decode(char key[], char* ciphertext, int len);
int digram_decrypt(char * in, char * out, int key_mappings[25], char key[]);
int char_pos(char in);
int pos_char(int pos);

extern struct g2_freq inline_digrams[256];
extern struct g2_freq digrams[256];
extern struct g3_freq trigrams[256];


#endif