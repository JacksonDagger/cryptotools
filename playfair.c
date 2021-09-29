#include "playfair.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define LEN_CMP 32
#define KEY_LEN 25
#define EXCLUDED_LET 'J'
#define INT_A 65
#define INT_J 74
#define KEY_BOX 5

#define TEST_SWAPS 16
#define MAX_ITERS 6400
#define RUN_TIME 600

char* real_ct = "ZQGSTOVNXFPFFVVDTKUXDHWTUQFQXUKXIFAYWTIFAYDYNHIBPLXUQGAGCHXYXMPFFVWTVDFRBDFZCLFVXULNZHEYAYYBWTLOPYGWNXDAIADIFHYDBDAYPWDPAWQENXPHHINXXMPFFVVTOHWTUQFQXUKXIFFZCLFVIGWTTVWPXNFZCLFVOYGBWGVQUXGADHBISKZUPHLIUXWEDHNBUDXUHPHTSTEHAFGABAFQXUTFFDKXIFAUIAFQHVDYHRHQAYMHTVCUEYAYYBWTDAIADPFZCLFVOYGSTGVBPYHZQRVSEKBSLPROQFFUIAOWDYHKPGNLXNTVHQSFDQFTGAUDZIXNBTDTXYRPHXXUTSXGIPGPXHHAYDBDAYAHBYIADGXUQFUDHNZPIADTLKVTEHGPWADHWTDCGHSUDZCYWAUDZIXNFZCLFVOYXKPFFVZUPHRTSHCYCDHFGWUTSGTFFZCLFVGWFTVAHBQHGPHTAHIAWTVDFRBDQNLGKNWEFZCLFVVTOHWTUQFQXUKXIFFZCLFVITDEUDDTUOQEDEQUAHIFDPHVUDRLBDEHAYWGCDNXGPCUVDHCFVEHIAAYYBWTGFTVBNXEPXDHTSWTBTFZCLFVOYGBSVBDEHDHSEQEUXAIUDSYTVSHCYCDNFVDVBDQMFPFFVVDHABAQEDUPWPIFTEHFTRMVYPDRIUQOHDUUXAYWIDISYDXBDTVCUHAUDQKSYQZYBCGITDEUDDTUASGHQWEDHTGKVNXOFEHDFMHDHAYSTEHITUXDQXUDYHRIBHLFLDVCNBTAYYBWTXFPFFVVTOHWTHXBGXMPFFVVDHOVFYIPHHWIAUDNCUXVYEFGWXFPFFVAITVWSUDRLDQAIIQBADENFUZQFWSBFVDVTAUIAITUXHZVSUDNCUXVYEFGWGDHZEVHBXUEYAYSTEHITIBABICDGFZCLFVAYHVDYSBDPKTUDXUSHLUDHFZCLFVPHKSLBGBQUAYVHZFPFFVPHHDDHAYWGAYHVMUIFVDGATGHLAYAGBDQUQZPFFVOTKVSHCYCDEFPFFVABGTTQDEIPYSPHVTSTXYXMPFFVUDVDTKUXDHVTNYDAIAIDFZCLFVVTOHAYYBWTXFPFFVPWTFKDXYXGABLYLGOYLGBDAYAHYDBDWTPHMFPFFVUDTVCUEFPFFVOWGHIAIAUDNCUXVYEFGWGFGWDGBDFZCLFVWTTSIOFINRHYPTNCUQPFUZYSPHTBFQELDPQGQHXYXMPFFVTUUXHQSUEDEFPFFVRSAGBDFVXFPFHCGWNWQZPFFVOWDYNRBWDHIAOWIOIGXYNVIAHGQNDYDSDLRUTOUVDYHKPGRPDEIFBYQUYOWADTDLBDSYDEIFAYQGWTTSBTSGNDFZCLFVVTOHWTUQFQXUKXIFAYAGBDQUQZPFFVUDFZHVPHNDEFPDWOHDHVOTXYXGFZCLFVOHPDVWPHNFGXHXVTEHSYKVVDUOYAROHZHLVSDKCDEFPFFVZUFQSKHZFQELDPWHHQUNGSAYWGOFDLPDHQXUFAABVPGAZDABUKWGFMVTZBHPHVPFQZPFFVGWAYUQHVFVQGQHXKPFFVOYGBFQHVCDNXGWAYSIHSOHPDIAFTHVZNIZXNAIFQXYNPDQOYPYFZCLFVVDHOWCGDDQUASTRAVSDKCDHFGWUTSGTFPHVSDQAYWKGWUIYSPHQKPFFVTVDYSUTBQKPFFVDIFTWPNQUXAIMUIFVDGATGHLBHEHUXVSVDHUIFDYDKQENXFZCLFVVTOHWTNXGWVGHXBGXMPFFVITIFTVSGNRRABYLGOYGUPWAYWGODBDIGXYXMPFHCGWNWHZXYYVKBIOAIAYVYGNQZVDOYLGBDAYQGWTVDFRBDFZCLFVUDVDTKUXDHRLTSDCNRXGAYQGWTDXFTRMDHOSFMAIWTUXYFEYAYAHBYIADGUDNLDHTGFPQNGSUDCDEMQZPFFVVDHAUDRASTGSDHVYPDHWUDOVWTYOXDXYYPKIGBFMOYGSUDNCUXVYEFGWDAIAAIAIHOHEIAGPCUWTBTFAHOWGPDAIWTNZSYAGTSWOUKAHIAUDVKNXXNAFYBNEHAUDVSDKCDNFVDTVOWUDKSVBQZQHHAUDXUWPHTSIDQEDVSYOYSSVPHWTUQSGTFFZCLFVVDHUDZWADERLIFXNHODVENDMGBDENYGFFDTSWTUQFQXUKXIFAYAHPDVWPHHAIAZFPFFVVTOHAYYBWTGAIDUXEDKWGBDEEYAYWTDENQUXPBQHAUIAIDFZCLFVPWDPSTKYTOEFPFFVXULNZHHAUDQKSYQZYBFHIAUDFQHVUNGSWTUQSGTFAYAHPYHQCNFQLYLGWTTSOSKBHQOYLGBDAYWGTUVYNVIAWPEHGWFZCLFVOYGBFQHVXHQGWGXUEYAYWGXYGVNXXGBDTVQZXUQWPFFVUDFZHVPHNDEFPFFVTVUDFTRMDHVYPDYHIAWTUXHZABVNDYEDWTPHHGHVUDFILXHOVFFOYSOHYOAYWIDTWGVSDTSTKBDHAYYBWTGAUDKNEMUWPHOFLKUXITPFFADXBDDPFZCLFVVDTKUXDHWTUQFQXUKXIFGWVTGPHTQHNFTSW";

int main(void) {
    time_t t;
    srand((unsigned) time(&t));
    int len = strlen(real_ct);
    char key[26] = {0};
    char top_keys[8][26] = {0};
    
    double test_mae = test_rand_keys(real_ct, len, key);

    printf("Best key is %s with mae of %f", key, test_mae);
}

double test_rand_keys(char *ciphertext, int len, char *key){
    clock_t begin = clock();
    char test_key[26] = {0};
    double test_mae = test_rand_key(ciphertext, len, test_key);
    double best_mae = test_mae;

    memcpy(key, test_key, KEY_LEN+1);
    clock_t now = clock();

    while ((double)(now - begin) / CLOCKS_PER_SEC < RUN_TIME){
        test_mae = test_rand_key(ciphertext, len, test_key);
        if (test_mae < best_mae){
            best_mae = test_mae;
            memcpy(key, test_key, KEY_LEN+1);
        }
    }
    return best_mae;
}

double test_rand_key(char *ciphertext, int len, char *key){
    char test_key[KEY_LEN+1] = {0};
    char best_key[KEY_LEN+1];
    int iters;
    int fails = 0;

    random_key(test_key);
    memcpy(best_key, test_key, KEY_LEN+1);

    double best_mae = mae(best_key, ciphertext, len);
    double test_mae = best_mae;

    for(iters=0; iters<MAX_ITERS; iters++){
        memcpy(test_key, best_key, KEY_LEN+1);
        random_swap(test_key);
        test_mae = mae(test_key, ciphertext, len);
        fails++;
        if (test_mae < best_mae){
            best_mae = test_mae;
            memcpy(best_key, test_key, KEY_LEN+1);
            fails = 0;
        }
        else if (fails >= TEST_SWAPS){
            break;
        }
    }
    memcpy(key, best_key, KEY_LEN+1);
    return best_mae;
}

void random_swap(char *key){
    int i1 = rand()%KEY_LEN;
    int i2 = rand()%KEY_LEN;
    while(i2 == i1){
        i2 = rand()%KEY_LEN;
    }
    char temp = key[i2];
    key[i2] = key[i1];
    key[i1] = temp;
}
void random_key(char *key){
    char alphabet[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    int i = 0;
    for(int len = KEY_LEN; len > 0; len--){
        i = rand()%len;
        key[KEY_LEN-len]=alphabet[i];
        alphabet[i] = alphabet[len-1];
    }
    key[KEY_LEN] = '\0';
}

double mae(char key[], char* ciphertext, int len){
    char* plaintext = playfair_decode(key, ciphertext, len);
    double mae = 0;
    mae += 2*digram_mae(plaintext, len);
    mae += inline_digram_mae(plaintext, len);
    mae += trigram_mae(plaintext, len);

    free(plaintext);
    return mae;
}

double digram_mae(char* plaintext, int len){
    unsigned int new_digrams[25][25] = {0};
    double mae = 0.0;
    for(int i = 0; i < len-1; i++) {
        new_digrams[char_pos(plaintext[i])][char_pos(plaintext[i+1])] += 1;
    }
    for(int i = 0; i < LEN_CMP; i ++){
        double expected_freq = digrams[i].freq;
        int index0 = char_pos(digrams[i].gram[0]);
        int index1 = char_pos(digrams[i].gram[1]);
        double freq = ((double) new_digrams[index0][index1])/((double) len-1);
        mae += fabs(freq-expected_freq);
    }
    return mae/LEN_CMP;
}

double inline_digram_mae(char* plaintext, int len){
    unsigned int new_digrams[25][25] = {0};
    double mae = 0.0;
    for(int i = 0; i < len-1; i+=2) {
        new_digrams[char_pos(plaintext[i])][char_pos(plaintext[i+1])] += 1;
    }
    for(int i = 0; i < LEN_CMP; i ++){
        double expected_freq = inline_digrams[i].freq;
        int index0 = char_pos(inline_digrams[i].gram[0]);
        int index1 = char_pos(inline_digrams[i].gram[1]);
        double freq = ((double) new_digrams[index0][index1])/((double) len/2);
        mae += fabs(freq-expected_freq);
    }
    return mae/LEN_CMP;
}

double trigram_mae(char* plaintext, int len){
    unsigned int new_trigrams[25][25][25] = {0};
    double mae = 0.0;
    for(int i = 0; i < len-2; i++) {
        new_trigrams[char_pos(plaintext[i])][char_pos(plaintext[i+1])][char_pos(plaintext[i+2])] += 1;
    }
    for(int i = 0; i < LEN_CMP; i ++){
        double expected_freq = trigrams[i].freq;
        int index0 = char_pos(trigrams[i].gram[0]);
        int index1 = char_pos(trigrams[i].gram[1]);
        int index2 = char_pos(trigrams[i].gram[2]);
        double freq = ((double) new_trigrams[index0][index1][index2])/((double) len-2);
        mae += fabs(freq-expected_freq);
    }
    return mae/LEN_CMP;
}

char* playfair_decode(char key[], char* ciphertext, int len){
    int key_mappings[25] = {0}; // map characters in the key to their index
    for(int i = 0; i < 25; i++){
        key_mappings[char_pos(key[i])]=i;
    }
    int zeroes = 0;
    for(int i = 0; i < 25; i++){
        zeroes += key_mappings[i] == 0;
    }
    if (zeroes != 1) {
        printf("Reverse key mapping messed up");
    }

    char *plaintext = (char *) malloc((len+1)*sizeof(char));
    memset((void *) plaintext, 0, len+1);

    for(int i = 0; i < len - 1; i += 2){
        digram_decrypt(&(ciphertext[i]), &(plaintext[i]), key_mappings, key);
    }
    return plaintext;
}

inline int digram_decrypt(char * in, char * out, int key_mappings[25], char key[]){
    int first_i = key_mappings[char_pos(in[0])];
    int second_i = key_mappings[char_pos(in[1])];

    if (first_i % KEY_BOX == second_i % KEY_BOX){
        out[0] = key[(KEY_LEN + first_i - KEY_BOX) % KEY_LEN];
        out[1] = key[(KEY_LEN + second_i - KEY_BOX) % KEY_LEN];
    }
    else if (first_i / KEY_BOX == second_i / KEY_BOX){
        out[0] = key[(KEY_LEN + first_i - 1) % KEY_LEN];
        out[1] = key[(KEY_LEN + second_i - 1) % KEY_LEN];
    }
    else {
        out[0] = key[first_i / KEY_BOX + second_i % KEY_BOX];
        out[1] = key[second_i / KEY_BOX + first_i % KEY_BOX];
    }
}

inline int char_pos(char in){
    int pos = (int) in;
    pos -= pos > INT_J;
    pos -= INT_A;
    return pos;
}

inline int pos_char(int pos){
    pos += INT_A;
    pos += pos >= INT_J;
    return (char) pos;
}

struct g2_freq inline_digrams[256] = {{"TH", 0.027750895822980147},  {"HE", 0.027574346768498417},  {"CO", 0.021100881437501634},  {"MA", 0.019832343786781052},  {"OM", 0.01979311066356289},  {"IN", 0.01592210917270421},  {"ER", 0.015516700232783199},  {"OT", 0.014195851751105066},
{"DO", 0.013574660633484163},  {"AN", 0.012816153584599692},  {"OU", 0.01235843381372113},  {"AT", 0.011783014673188083},  {"ED", 0.01133837261004891},  {"ND", 0.011063740747521774},  {"IT", 0.010521015876337195},  {"RE", 0.010377161091203934},
{"ON", 0.009893285904846599},  {"TO", 0.00975596997358303},  {"AS", 0.00933094447205294},  {"EN", 0.008958229801480396},  {"HA", 0.008820913870216829},  {"XM", 0.008670520231213872},  {"ES", 0.008637825961865405},  {"NG", 0.008637825961865405},
{"MX", 0.008605131692516936},  {"MM", 0.008513587738341223},  {"AL", 0.00850704888447153},  {"TI", 0.00838281066094735},  {"EA", 0.008376271807077656},  {"NT", 0.008330499829989799},  {"SE", 0.007362749457275128},  {"TE", 0.007343132895666048},
{"EC", 0.007186200402793399},  {"ST", 0.007127350717966155},  {"AI", 0.00710119530248738},  {"ET", 0.006957340517354118},  {"SH", 0.0068788742709177935},  {"SA", 0.006571548139042188},  {"LI", 0.006565009285172495},  {"HI", 0.006466926477127089},
{"IC", 0.006224988883948421},  {"OR", 0.006153061491381791},  {"AR", 0.0059895901446394475},  {"LE", 0.005610336620197212},  {"TA", 0.005538409227630581},  {"ID", 0.005401093296367013},  {"VE", 0.005374937880888238},  {"CE", 0.005329165903800382},
{"IS", 0.00523762194962467},  {"UT", 0.005211466534145895},  {"ME", 0.005211466534145895},  {"YO", 0.0051526168493186515},  {"DT", 0.00506107289514294},  {"OF", 0.004995684356446002},  {"WA", 0.0047602856171370285},  {"EL", 0.004629508539743154},
{"BE", 0.004564120001046217},  {"HO", 0.004498731462349279},  {"RO", 0.004485653754609892},  {"NO", 0.004407187508173567},  {"NE", 0.00439410980043418},  {"SO", 0.004374493238825099},  {"EM", 0.0043156435539978555},  {"TS", 0.00428948813851908},
{"EW", 0.0039690842989040884},  {"OW", 0.0039560065911647},  {"DI", 0.003877540344728376},  {"TX", 0.00377945753668297},  {"XT", 0.0037598409750738892},  {"DE", 0.003714068997986033},  {"LO", 0.0036879135825072583},  {"RS", 0.0036748358747678705},
{"WH", 0.003642141605419402},  {"DA", 0.0036159861899406273},  {"RA", 0.0036159861899406273},  {"TT", 0.0036159861899406273},  {"AD", 0.0035309810896346086},  {"NA", 0.0034982868202861404},  {"RI", 0.0034917479664164463},  {"LY", 0.0034721314048073654},
{"GH", 0.003360970889022572},  {"CH", 0.0032825046425862476},  {"US", 0.0032563492271074725},  {"WI", 0.0032432715193680853},  {"TC", 0.0032432715193680853},  {"RT", 0.0031909606884105352},  {"TW", 0.0031386498574529857},  {"EX", 0.0031255721497135984},
{"UR", 0.0030667224648863547},  {"FO", 0.003060183611016661},  {"HT", 0.00298825621845003},  {"SI", 0.0029817173645803365},  {"NI", 0.002962100802971255},  {"KE", 0.002942484241362174},  {"NC", 0.0029359453874924805},  {"LA", 0.0028836345565349305},
{"UL", 0.002850940287186462},  {"XE", 0.0028378625794470745},  {"LD", 0.002779012894619831},  {"AY", 0.00275939633301075},  {"WO", 0.0026940077943138126},  {"WE", 0.0026743912327047317},  {"XL", 0.0026482358172259567},  {"NS", 0.0026286192556168753},
{"IL", 0.0026024638401381008},  {"SC", 0.0025959249862684067},  {"LX", 0.0025959249862684067},  {"MI", 0.0025959249862684067},  {"TL", 0.0025828472785290194},  {"EE", 0.002569769570789632},  {"AB", 0.002523997593701776},  {"LL", 0.0024782256166139198},
{"FT", 0.002471686762744226},  {"CA", 0.0024651479088745325},  {"GE", 0.002452070201135145},  {"EO", 0.002452070201135145},  {"EI", 0.0024389924933957575},  {"RY", 0.002399759370177595},  {"DS", 0.0023932205163079015},  {"OL", 0.002360526246959433},
{"XA", 0.0023539873930897392},  {"AX", 0.0023474485392200456},  {"UN", 0.002340909685350352},  {"AW", 0.002340909685350352},  {"EP", 0.0023212931237412706},  {"EF", 0.002314754269871577},  {"IE", 0.0022755211466534146},  {"AA", 0.00219705490021709},
{"AC", 0.0021905160463473964},  {"GA", 0.0021774383386080087},  {"MO", 0.0021643606308686214},  {"IM", 0.002151282923129234},  {"EY", 0.0021382052153898464},  {"EG", 0.0021316663615201527},  {"IO", 0.002125127507650459},  {"PE", 0.002125127507650459},
{"DC", 0.002112049799911072},  {"KI", 0.002085894384432297},  {"TY", 0.0020335835534747468},  {"GO", 0.0020335835534747468},  {"OS", 0.002027044699605053},  {"TR", 0.0020008892841262785},  {"XO", 0.001961656160908116},  {"EH", 0.0019485784531687285},
{"RD", 0.0019289618915596474},  {"RC", 0.0019224230376899537},  {"OK", 0.0018962676222111787},  {"AG", 0.0018766510606020976},  {"TD", 0.001870112206732404},  {"AM", 0.0018374179373839353},  {"OI", 0.0018178013757748541},  {"FI", 0.001798184814165773},
{"YC", 0.0017916459602960793},  {"IG", 0.0017785682525566918},  {"QU", 0.0017654905448173043},  {"OX", 0.001752412837077917},  {"EV", 0.0017131797138597547},  {"OO", 0.0016870242983809797},  {"PR", 0.0016543300290325113},  {"SX", 0.0016477911751628174},
{"BU", 0.0016477911751628174},  {"GT", 0.0016412523212931238},  {"EB", 0.0016216357596840426},  {"CT", 0.0016085580519446551},  {"UE", 0.0016085580519446551},  {"AV", 0.0015824026364658803},  {"SS", 0.0015758637825961865},  {"TU", 0.0015627860748567992},
{"AP", 0.0015627860748567992},  {"XS", 0.0015562472209871053},  {"UG", 0.0015366306593780242},  {"IR", 0.0015104752438992494},  {"FA", 0.0015039363900295555},  {"UP", 0.0015039363900295555},  {"SU", 0.0014908586822901682},  {"BO", 0.0014843198284204744},
{"GR", 0.0014647032668113932},  {"UC", 0.001445086705202312},  {"IF", 0.001445086705202312},  {"PO", 0.0014189312897235373},  {"DN", 0.0014189312897235373},  {"OD", 0.0014123924358538436},  {"PL", 0.0014058535819841498},  {"YT", 0.0014058535819841498},
{"DH", 0.0013993147281144561},  {"OP", 0.0013862370203750686},  {"YD", 0.0013862370203750686},  {"UM", 0.0013470038971569063},  {"GI", 0.0013273873355478252},  {"YS", 0.0013273873355478252},  {"NY", 0.0013208484816781315},  {"RG", 0.0013012319200690504},
{"SD", 0.0012750765045902754},  {"FE", 0.0012750765045902754},  {"OC", 0.0012685376507205817},  {"MP", 0.0012685376507205817},  {"PA", 0.001261998796850888},  {"SW", 0.001261998796850888},  {"RK", 0.001261998796850888},  {"DL", 0.0012554599429811942},
{"LT", 0.0012096879658933383},  {"KN", 0.0011966102581539508},  {"TM", 0.0011900714042842571},  {"OH", 0.0011769936965448696},  {"CK", 0.0011639159888054821},  {"DW", 0.0011377605733267073},  {"WN", 0.001098527450108545},  {"AK", 0.001098527450108545},
{"TG", 0.0010919885962388512},  {"PT", 0.0010919885962388512},  {"YA", 0.0010919885962388512},  {"NB", 0.0010854497423691575},  {"RM", 0.0010854497423691575},  {"GU", 0.0010789108884994639},  {"LF", 0.0010789108884994639},  {"GS", 0.00107237203462977},
{"DX", 0.00107237203462977},  {"TB", 0.0010658331807600764},  {"DD", 0.0010462166191509952},  {"XD", 0.001026600057541914},  {"AF", 0.001026600057541914},  {"DB", 0.0010135223498025266},  {"YI", 0.0010004446420631393},  {"IK", 0.0010004446420631393},
{"SN", 0.0009939057881934454},  {"NL", 0.0009873669343237518},  {"DR", 0.0009677503727146706},  {"DU", 0.0009677503727146706},  {"PI", 0.0009612115188449769},  {"IV", 0.0009546726649752831},  {"EK", 0.0009546726649752831},  {"CR", 0.0009481338111055894},
{"OB", 0.0009481338111055894},  {"SP", 0.0009415949572358957},  {"FR", 0.0009415949572358957},  {"RN", 0.0009415949572358957},  {"EQ", 0.000935056103366202},  {"BL", 0.0009219783956268146},  {"RU", 0.0009219783956268146},  {"YE", 0.0009089006878874271},
{"YW", 0.0009023618340177334},  {"MU", 0.0008892841262783459},  {"GC", 0.0008892841262783459},  {"UD", 0.0008762064185389585},  {"OA", 0.0008696675646692648},  {"DY", 0.0008696675646692648},  {"RH", 0.000863128710799571},  {"TF", 0.0008565898569298774},
{"FU", 0.0008565898569298774},  {"SL", 0.0008108178798420213},  {"IU", 0.0007977401721026338},  {"FC", 0.0007912013182329402},  {"NH", 0.0007519681950147778},  {"OV", 0.0007454293411450841},  {"LS", 0.0007388904872753904},  {"TN", 0.0007323516334056966}};


struct g2_freq digrams[256] ={{"TH", 0.027898111245884594},  {"HE", 0.027149410029980742},  {"CO", 0.021074794924525034},  {"OM", 0.019871641878880415},  {"MA", 0.019750672686791145},  {"ER", 0.01579138372408562},  {"IN", 0.015621372967635837},  {"OT", 0.013957229217002383},
{"DO", 0.013597591078358612},  {"AN", 0.012685417981253045},  {"OU", 0.01210999695942301},  {"AT", 0.011485534373232461},  {"ED", 0.011299176428662506},  {"ND", 0.010949346602890837},  {"IT", 0.01040988939492518},  {"RE", 0.010046981818657373},
{"ON", 0.00980831287210287},  {"AS", 0.009559835612676263},  {"TO", 0.009546757862180127},  {"EN", 0.009177311410664251},  {"HA", 0.00902364784233464},  {"NG", 0.008807864959148377},  {"ES", 0.008752284519539794},  {"XM", 0.008641123640322629},
{"MX", 0.00863458476507456},  {"MM", 0.008516885010609324},  {"EA", 0.00840572413139216},  {"NT", 0.008376299192775851},  {"AL", 0.008222635624446238},  {"TI", 0.008180132935333793},  {"TE", 0.007653753477864273},  {"ST", 0.007222187711491746},
{"SE", 0.007169876709507197},  {"EC", 0.007143721208514924},  {"ET", 0.0070979490817784435},  {"AI", 0.006944285513448832},  {"LI", 0.006862549572847975},  {"SH", 0.006780813632247117},  {"SA", 0.006640227814413643},  {"HI", 0.006411367180731243},
{"OR", 0.006411367180731243},  {"IC", 0.00621847036091322},  {"AR", 0.005773826844044556},  {"TA", 0.005629971588587047},  {"ID", 0.00559400777472267},  {"IS", 0.0055057329588737445},  {"LE", 0.005495924646001641},  {"VE", 0.0054011109549046464},
{"CE", 0.00538803320440851},  {"YO", 0.005198405822214521},  {"UT", 0.005162442008350144},  {"OF", 0.005139555944981904},  {"ME", 0.005028395065764738},  {"DT", 0.005005509002396498},  {"WA", 0.004622984800384486},  {"NO", 0.0046099070498883486},
{"EL", 0.0045281711092874915},  {"BE", 0.004502015608295217},  {"TS", 0.004495476733047148},  {"RO", 0.004479129544926977},  {"EM", 0.004462782356806806},  {"NE", 0.004354890915213674},  {"HO", 0.004296041037981057},  {"SO", 0.004158724657771617},
{"OW", 0.004139108032027411},  {"EW", 0.003982175026073765},  {"DI", 0.003956019525081491},  {"XT", 0.0038939002102248393},  {"DE", 0.003769661580511536},  {"WH", 0.0037238894537750564},  {"LO", 0.003697733952782782},  {"DA", 0.0036944645151587474},
{"LY", 0.003622536887429993},  {"TX", 0.0036061896993098215},  {"RI", 0.0036029202616857875},  {"AD", 0.003583303635941582},  {"TT", 0.0035800341983175474},  {"NA", 0.003540800946829136},  {"RA", 0.0034917593824686215},  {"RS", 0.0034427178181081074},
{"UR", 0.003334826376514976},  {"EX", 0.0033086708755227013},  {"GH", 0.003272707061658324},  {"US", 0.003262898748786221},  {"WI", 0.0032530904359141183},  {"CH", 0.00324655156066605},  {"TC", 0.0031778933705613297},  {"TW", 0.0031386601190729183},
{"SI", 0.0031059657428325754},  {"RT", 0.0030961574299604726},  {"HT", 0.003034038115103821},  {"KE", 0.002988265988367341},  {"NC", 0.0029849965507433066},  {"NI", 0.0029588410497510325},  {"FO", 0.002932685548758758},  {"AY", 0.002850949608157901},
{"WE", 0.002798638606173352},  {"LD", 0.0027920997309252835},  {"LA", 0.0027692136675570435},  {"UL", 0.002765944229933009},  {"TL", 0.0026809388517081176},  {"EE", 0.002674399976460049},  {"SC", 0.0026711305388360147},  {"XE", 0.002661322225963912},
{"XL", 0.0026220889744755006},  {"LX", 0.002618819536851466},  {"LL", 0.002605741786355329},  {"WO", 0.0025926640358591917},  {"NS", 0.002569777972490952},  {"UN", 0.0025272752833785063},  {"IL", 0.0025272752833785063},  {"EI", 0.002514197532882369},
{"AB", 0.002491311469514129},  {"EO", 0.0024749642813939573},  {"RY", 0.0024390004675295804},  {"CA", 0.0024226532794094087},  {"OL", 0.0024226532794094087},  {"DS", 0.0024193838417853747},  {"EP", 0.002412844966537306},  {"MI", 0.0024063060912892374},
{"FT", 0.0023736117150488945},  {"XA", 0.00237034227742486},  {"AX", 0.002353995089304689},  {"AW", 0.00234745621405662},  {"GA", 0.002337647901184517},  {"EF", 0.0023311090259364488},  {"GE", 0.0023180312754403115},  {"IE", 0.0022853368991999686},
{"IM", 0.00223302589721542},  {"AA", 0.002223217584343317},  {"TR", 0.0021741760199828026},  {"PE", 0.0021741760199828026},  {"AC", 0.0021480205189905286},  {"OS", 0.002144751081366494},  {"KI", 0.0021382122061184257},  {"MO", 0.0021349427684943913},
{"EY", 0.0021022483922540484},  {"DC", 0.0020924400793819456},  {"GO", 0.0020859012041338767},  {"EH", 0.00207282345363774},  {"IO", 0.002066284578389671},  {"EG", 0.0020041652635330197},  {"RD", 0.0019976263882849512},  {"AM", 0.001994356950660917},
{"TY", 0.0019812792001647796},  {"TD", 0.0019714708872926767},  {"QU", 0.001915890447684094},  {"RC", 0.0019126210100600595},  {"OK", 0.0018995432595639225},  {"OX", 0.0018766571961956824},  {"XO", 0.0018537711328274423},  {"IG", 0.0018439628199553395},
{"AG", 0.001840693382331305},  {"FI", 0.0018178073189630652},  {"YC", 0.0017687657546025509},  {"OO", 0.0017295325031141393},  {"EV", 0.0017262630654901051},  {"GT", 0.0017131853149939679},  {"BU", 0.0017131853149939679},  {"OI", 0.0016968381268737964},
{"CT", 0.0016935686892497622},  {"PR", 0.0016314493743931106},  {"XS", 0.001602024435776802},  {"SX", 0.0015987549981527677},  {"EB", 0.0015824078100325962},  {"UG", 0.0015824078100325962},  {"GR", 0.0015693300595364592},  {"SS", 0.001556252309040322},
{"AV", 0.0015529828714162877},  {"AP", 0.0015431745585441849},  {"BO", 0.0015170190575519106},  {"UP", 0.0015072107446798075},  {"TU", 0.0015039413070557733},  {"IF", 0.0014974024318077047},  {"UC", 0.0014941329941836705},  {"FA", 0.0014745163684394646},
{"IR", 0.0014712469308154304},  {"UE", 0.0014647080555673618},  {"YT", 0.0014483608674471903},  {"YD", 0.0014450914298231561},  {"PL", 0.0014418219921991217},  {"UM", 0.001422205366454916},  {"PO", 0.0014123970535828132},  {"SU", 0.0013927804278386076},
{"DH", 0.001386241552590539},  {"OP", 0.0013568166139742302},  {"OD", 0.0013568166139742302},  {"OC", 0.0013437388634780932},  {"DN", 0.0013437388634780932},  {"PA", 0.0013339305506059904},  {"SW", 0.0012881584238695102},  {"YS", 0.001275080673373373},
{"FE", 0.0012685417981253045},  {"RG", 0.0012652723605012701},  {"GI", 0.0012554640476291673},  {"KN", 0.0012489251723810987},  {"NY", 0.0012260391090128588},  {"RK", 0.0012096919208926873},  {"OH", 0.0011933447327725159},  {"AK", 0.0011835364199004128},
{"SD", 0.0011802669822763786},  {"YA", 0.0011769975446523444},  {"LT", 0.0011704586694042758},  {"TM", 0.0011704586694042758},  {"DL", 0.0011541114812841043},  {"CK", 0.001141033730787967},  {"NB", 0.001137764293163933},  {"PT", 0.001137764293163933},
{"GU", 0.0011312254179158643},  {"MP", 0.00112795598029183},  {"RM", 0.0011083393545476242},  {"LF", 0.0010854532911793843},  {"AF", 0.0010756449783072813},  {"TB", 0.001072375540683247},  {"DW", 0.0010658366654351784},  {"DB", 0.0010625672278111442},
{"XD", 0.0010625672278111442},  {"TG", 0.001049489477315007},  {"YI", 0.0010429506020669384},  {"DR", 0.0010233339763227327},  {"DX", 0.0010200645386986985},  {"SP", 0.001016795101074664},  {"WN", 0.0010069867882025612},  {"CR", 0.0010004479129544926},
{"DD", 0.0009939090377064242},  {"FR", 0.0009906396000823898},  {"MU", 0.000980831287210287},  {"PI", 0.0009710229743381841},  {"IV", 0.0009677535367141498},  {"NL", 0.0009677535367141498},  {"GS", 0.0009677535367141498},  {"OA", 0.0009612146614660812},
{"UD", 0.0009415980357218755},  {"SN", 0.0009317897228497726},  {"RH", 0.0009317897228497726},  {"OB", 0.0009187119723536354},  {"EK", 0.0009187119723536354},  {"DU", 0.0009121730971055669},  {"GC", 0.000902364784233464},  {"RN", 0.0008990953466094297},
{"BL", 0.0008860175961132925},  {"TF", 0.000879478720865224},  {"FU", 0.0008664009703690869},  {"EQ", 0.0008565926574969839},  {"YW", 0.0008533232198729496},  {"IK", 0.0008533232198729496},  {"OV", 0.0008467843446248811},  {"YE", 0.000833706594128744},
{"RU", 0.0007912039050162981},  {"GL", 0.0007879344673922639},  {"AH", 0.0007879344673922639},  {"DY", 0.0007846650297682296},  {"NK", 0.0007683178416480581},  {"SL", 0.0007683178416480581},  {"LC", 0.0007487012159038524},  {"LS", 0.0007290845901596466}};


struct g3_freq trigrams[256] ={{"COM", 0.017749834892860177},  {"THE", 0.017504626269363308},  {"DOT", 0.009621986386017224},  {"MXM", 0.008634612995403156},  {"OMX", 0.008572493477450614},  {"XMA", 0.008565954580824032},  {"OMM", 0.008448254441545533},  {"MMA", 0.008438446096605658},
{"ING", 0.007503383879004257},  {"AND", 0.007228750220687761},  {"HER", 0.005793462411152742},  {"YOU", 0.004515108120655721},  {"SHE", 0.004423563567883555},  {"ECO", 0.003975649148962604},  {"HAT", 0.003671590455826484},  {"ICE", 0.003651973765946734},
{"TXT", 0.0036062014895606514},  {"THA", 0.003498309695222028},  {"ALI", 0.003423112384016321},  {"DTH", 0.0033413761761840305},  {"AID", 0.003305412244737823},  {"ERE", 0.0032825261065447817},  {"THI", 0.0031550176223264087},  {"LIC", 0.0030765508628074097},
{"SAI", 0.0030634730695542436},  {"EDO", 0.0028117255494307892},  {"ENT", 0.002759414376418123},  {"OTH", 0.002687486513525708},  {"ATH", 0.0026547920303927916},  {"LXL", 0.0026188280989465837},  {"EXE", 0.002615558650633292},  {"VER", 0.0026122892023200004},
{"ETH", 0.002579594719187084},  {"MAS", 0.0024847807181016274},  {"WAS", 0.002478241821475044},  {"MAT", 0.0024161223035225037},  {"ITH", 0.002354002785569963},  {"TCO", 0.002344194440630088},  {"TER", 0.002278805474364256},  {"NTH", 0.002268997129424381},
{"AXA", 0.002259188784484506},  {"GHT", 0.002249380439544631},  {"HES", 0.002242841542918048},  {"MAX", 0.0022395720946047563},  {"MAA", 0.002197069266531965},  {"HIS", 0.002148027541832591},  {"FOR", 0.002089177472193342},  {"OTI", 0.002072830230626884},
{"HIN", 0.002056482989060426},  {"OUT", 0.002030327402554093},  {"SCO", 0.0020074412643610517},  {"WIT", 0.001997632919421177},  {"ERS", 0.001978016229541427},  {"REA", 0.0019747467812281354},  {"MAI", 0.0018733938835160955},  {"HEW", 0.0018701244352028038},
{"DCO", 0.0018505077453230543},  {"OUL", 0.0018505077453230543},  {"OXO", 0.0018243521588167213},  {"INT", 0.00180146602062368},  {"ASA", 0.001775310434117347},  {"XTH", 0.001732807606044556},  {"TIN", 0.0017066520195382232},  {"TTH", 0.0016837658813451818},
{"FTH", 0.0016706880880920153},  {"ULD", 0.0016543408465255574},  {"ESA", 0.0016510713982122657},  {"ONE", 0.0016445325015856825},  {"KIN", 0.0016379936049590992},  {"NCO", 0.0016085685701394746},  {"TDO", 0.0016085685701394746},  {"OFT", 0.001605299121826183},
{"NDT", 0.001605299121826183},  {"SXS", 0.0015987602251996},  {"NOT", 0.0015791435353198502},  {"EVE", 0.0015660657420666837},  {"HEM", 0.001562796293753392},  {"ATI", 0.0015562573971268088},  {"TAL", 0.0015562573971268088},   {"BUT", 0.001546449052186934},
{"HOU", 0.0015431796038736423},  {"XAN", 0.0015366407072470592},  {"ION", 0.0015333712589337676},  {"HEN", 0.0015268323623071844},  {"THO", 0.0015104851207407262},  {"AAN", 0.001494137879174268},  {"YCO", 0.00147779063760781},  {"ONT", 0.0014745211892945185},
{"OME", 0.0014647128443546437},  {"NDO", 0.001461443396041352},  {"OUR", 0.0014549044994147688},  {"EDT", 0.0014516350511014772},  {"EAR", 0.0014385572578483107},  {"IDT", 0.001435287809535019},  {"OTA", 0.0014287489129084358},  {"IGH", 0.0014124016713419776},
{"UGH", 0.0014091322230286862},  {"AST", 0.0013960544297755197},  {"EWA", 0.0013862460848356449},  {"TED", 0.0013829766365223532},  {"TLE", 0.0013829766365223532},  {"STH", 0.0013633599466426035},  {"MAW", 0.0013633599466426035},  {"AIN", 0.0013633599466426035},
{"LON", 0.0013568210500160202},  {"RCO", 0.001350282153389437},  {"TIT", 0.0013404738084495623},  {"OUG", 0.0013372043601362707},  {"UND", 0.0013208571185698125},  {"ECT", 0.001317587670256521},  {"HEC", 0.0013012404286900628},  {"NGT", 0.0013012404286900628},
{"TEN", 0.0012783542904970216},  {"ERY", 0.0012718153938704384},  {"ITE", 0.0012685459455571467},  {"TIO", 0.0012620070489305635},  {"HED", 0.0012620070489305635},  {"NOW", 0.0012620070489305635},  {"EDI", 0.0012587376006172719},  {"AVE", 0.0012521987039906886},
{"ETO", 0.0012456598073641053},  {"HET", 0.0012423903590508137},  {"HEA", 0.0012423903590508137},  {"NDS", 0.0012358514624242305},  {"ITS", 0.0012325820141109388},  {"NTO", 0.0012293125657976472},  {"DON", 0.0012195042208577726},  {"RTH", 0.001216234772544481},
{"QUE", 0.001216234772544481},  {"ARE", 0.0012129653242311893},  {"OUS", 0.0012129653242311893},  {"DTO", 0.0012031569792913144},  {"INA", 0.0011933486343514395},  {"TON", 0.0011868097377248563},  {"DIN", 0.001180270841098273},  {"RSE", 0.001180270841098273},
{"HAD", 0.0011671930478451065},  {"EST", 0.0011573847029052319},  {"DAL", 0.0011573847029052319},  {"ALX", 0.0011541152545919402},  {"ERA", 0.001147576357965357},  {"ALL", 0.001147576357965357},  {"STO", 0.0011443069096520654},  {"EAT", 0.001137768013025482},
{"OUN", 0.001137768013025482},  {"ASH", 0.001137768013025482},  {"USE", 0.0011246902197723156},  {"ATE", 0.001111612426519149},  {"EDA", 0.001111612426519149},  {"EMI", 0.001111612426519149},  {"UTE", 0.0011083429782058575}, {"RES", 0.0011050735298925658},
{"HAN", 0.0011050735298925658},  {"RED", 0.0010985346332659826},  {"LIT", 0.0010919957366393996},  {"HEY", 0.001088726288326108},  {"TOT", 0.0010854568400128163},  {"TAN", 0.0010756484950729414},  {"ERC", 0.0010723790467596498},  {"COU", 0.0010658401501330665},
{"ISH", 0.001062570701819775},  {"PRO", 0.0010560318051931917},  {"EAD", 0.0010560318051931917},  {"WHI", 0.0010560318051931917},  {"ICO", 0.0010462234602533168},  {"TOF", 0.001036415115313442},  {"ERT", 0.0010298762186868589},  {"NIN", 0.0010266067703735672},
{"ONA", 0.001020067873746984},  {"DXD", 0.001020067873746984},  {"ORT", 0.0010135289771204007},  {"EOF", 0.0010135289771204007},  {"OLO", 0.0010102595288071091},  {"EAN", 0.0010069900804938175},  {"WHA", 0.0010037206321805259},  {"ANT", 0.0010004511838672342},
{"LED", 0.000984103942300776},  {"IND", 0.0009808344939874845},  {"UST", 0.0009742955973609013},  {"NGA", 0.0009742955973609013},  {"KED", 0.0009710261490476097},  {"HEL", 0.0009677567007343181},  {"HEH", 0.0009644872524210264},  {"SDO", 0.0009644872524210264},
{"WOR", 0.0009579483557944432},  {"COL", 0.0009546789074811517},  {"NDA", 0.0009481400108545684},  {"EAS", 0.0009481400108545684},  {"IST", 0.0009481400108545684},  {"TYO", 0.0009448705625412768},  {"EHA", 0.0009416011142279852},  {"WHE", 0.0009383316659146935},
{"OWN", 0.0009317927692881103},  {"CEC", 0.0009317927692881103},  {"STA", 0.0009252538726615271},  {"MAY", 0.0009187149760349439},  {"HTH", 0.0009187149760349439},  {"TIM", 0.0009187149760349439},  {"NDE", 0.0009154455277216523},  {"OTT", 0.0009154455277216523},
{"SEM", 0.000908906631095069},  {"YDO", 0.000908906631095069},  {"YTH", 0.000908906631095069},  {"MIC", 0.0009056371827817774},  {"SEL", 0.0009056371827817774},  {"ANY", 0.0008990982861551941},  {"TSH", 0.0008990982861551941},  {"DER", 0.0008892899412153194},
{"ORE", 0.0008892899412153194},  {"TOH", 0.0008827510445887361},  {"HEP", 0.0008794815962754445},  {"BER", 0.0008794815962754445},  {"ATS", 0.0008729426996488612},  {"AWH", 0.0008631343547089864},  {"EFO", 0.0008565954580824032},  {"EQU", 0.0008565954580824032},
{"TUR", 0.0008435176648292367},  {"MAB", 0.0008435176648292367},  {"AME", 0.0008435176648292367},  {"WAY", 0.0008337093198893618},  {"UCH", 0.0008337093198893618},  {"KNO", 0.0008337093198893618},  {"OTX", 0.0008304398715760703},  {"ELF", 0.0008304398715760703},
{"ENC", 0.000823900974949487},  {"OTS", 0.000823900974949487},  {"NAN", 0.0008206315266361954},  {"ENS", 0.0008173620783229038},  {"NGS", 0.0008173620783229038},  {"OTW", 0.0008140926300096122},  {"EEN", 0.0008140926300096122},  {"IVE", 0.0008108231816963205},
{"CES", 0.0008042842850697373},  {"GCO", 0.0008010148367564457},  {"ITW", 0.0008010148367564457},  {"ECA", 0.0007977453884431541},  {"NBE", 0.0007944759401298625},  {"AYO", 0.0007944759401298625},  {"UTI", 0.0007944759401298625},  {"EMA", 0.0007944759401298625},
{"ARD", 0.0007944759401298625},  {"HEF", 0.0007944759401298625},  {"TWA", 0.0007944759401298625},  {"ONS", 0.0007912064918165709},  {"ONC", 0.0007912064918165709},  {"NGC", 0.0007879370435032793},  {"ESE", 0.0007846675951899876},  {"EAL", 0.000781398146876696},
{"HOW", 0.000781398146876696},  {"WER", 0.000781398146876696},  {"OOK", 0.0007748592502501128},  {"SOM", 0.0007748592502501128},  {"LEA", 0.0007715898019368211},  {"NCE", 0.0007715898019368211},  {"EME", 0.0007715898019368211},  {"SOF", 0.0007683203536235296}};
