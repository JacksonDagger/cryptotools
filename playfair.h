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


extern struct g2_freq inline_digrams[256];
extern struct g2_freq digrams[256];
extern struct g2_freq trigrams[256];


#endif