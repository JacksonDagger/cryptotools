from string import ascii_uppercase
import matplotlib.pyplot as plt
import pandas as pd

class ct:
    A_pos = 65
    alphabet_size = 26
    books = ["alice.txt", "lg.txt"]

    crc32lut = [
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
    ]
    """ paste in Python shell to reload
from importlib import reload
import tools
tools = reload(tools)
from tools import ct
    """
    """converts a single uppercase character to its position in the alphabet (ex. char_pos('A')=1)"""
    def char_pos(char):
        pos = ord(char) - ct.A_pos
        if pos < 0 or pos >= 26:
            raise ValueError("not a capital character")
        return pos
    
    """converts a positon in the alphabet to a sibngle uppercase character (ex. cpos_char(1)='A')"""
    def pos_char(pos):
        if pos < 0 or pos >= 26:
            raise ValueError("not a capital character")
        char = chr(pos + ct.A_pos)
        return char
    
    """shifts an uppercase letter [shift] letters up in the alphabet (ex. char_shift(Y, 3)='B')"""
    def char_shift(char, shift):
        if char == ' ':
            return ' '
        pos = ct.char_pos(char)
        return chr((pos+shift)%26 + ct.A_pos)

    """encrypts a given uppercase plain-text with the given shift with the Caesar cipher"""
    def caesar_encrypt(plaintext, shift):
        if type(plaintext) != str:
            raise TypeError("text must be a string")
        ciphertext = "".join(list(map(lambda x: ct.char_shift(x, shift), plaintext)))
        return ciphertext

    """decrypts a given uppercase cipher-text with the given shift with the Caesar cipher"""
    def caesar_decrypt(ciphertext, shift):
        return ct.caesar_encrypt(ciphertext, (-1)*shift)

    """attempts and prints all 26 possible Caeser shifts for the ciphertext"""
    def caesar_all_tries(ciphertext):
        if type(ciphertext) != str:
            raise TypeError("text must be a string")
        for i in range(26):
            print("Shift " + str(i) +" : " + ct.caesar_decrypt(ciphertext, i))
    
    """creates a dictionary of character frequencies"""
    def freq_dict(ciphertext):
        if type(ciphertext) != str:
            raise TypeError("text must be a string")
        freqs = {"total": len(ciphertext)}
        for c in ascii_uppercase:
            freqs[c] = ciphertext.count(c)
        return freqs
    
    """uses pyplot to show a frequency chart for a given cipher-text"""
    def freq_chart(ciphertext, title = "Ciphertext frequency chart", show = True, figure = -1):
        freqs = ct.freq_dict(ciphertext)
        chars = list(freqs.keys())[1:]
        char_freqs = list(freqs.values())[1:]

        if figure > -1:
            plt.figure(figure)
        plt.bar(chars, char_freqs)
        plt.xlabel("Characters")
        plt.ylabel("Frequency")
        plt.title(title)
        if show:
            plt.show()
    
    """uses pyplot to show multiple frequency charts for a given cipher-text anf guessed poly key length"""
    def poly_freq_chart(ciphertext, keylen):
        plt.clf()
        for i in range(keylen):
            partstring = "".join([ciphertext[j] for j in range(i, len(ciphertext), keylen)])
            title = "Polyalphabetic frequency analysis: length = " + str(keylen) + " offset = " + str(i)
            fignum = keylen**2 + i
            ct.freq_chart(partstring, title = title, show = False, figure = fignum)
        plt.show()
    
    """encrypts a given uppercase plain-text with a polyalphabetic/Vigniere cipher"""
    def poly_encrypt(plaintext, keyword):
        if type(plaintext) != str:
            raise TypeError("text must be a string")
        shifts = list(map(lambda x: ord(x) - ct.A_pos, plaintext))
        cipherlist = [ct.char_shift(plaintext[i], shifts[i%len(shifts)]) for i in range (len(plaintext))]
        ciphertext = "".join(cipherlist)
        return ciphertext
    
    """decrypts a given uppercase cipher-text with a polyalphabetic/Vigniere cipher"""
    def poly_decrypt(ciphertext, keyword):
        inverse_key = "".join(list(map(lambda x: ct.pos_char(26-ct.char_pos(x)), keyword)))
        return ct.poly_encrypt(ciphertext, inverse_key)

    """
    encrypts a given uppercase plain-text with a key representing the mappings of the letters of the alphabet in a substitution cipher
    the key is a string where the character in each position is mapped to by the character in that potition in the standard alphabet
    underscores indicate no-mapping and nothing is done
    """
    def substitution_encrypt(plaintext, key):
        if len(key) != ct.alphabet_size:
            raise ValueError("substitution keys must be " + str(ct.alphabet_size) + " chars long")
        key = list(key)
        cipher_list = [key[ct.char_pos(c)] for c in plaintext]
        ciphertext = "".join(cipher_list)

        if '_' in key: # incomplete key
            part_cipher_list = [key[ct.char_pos(c)] if key[ct.char_pos(c)] != '_' else c.lower() for c in plaintext]
            part_ciphertext = "".join(part_cipher_list)
            print(part_ciphertext)
        return ciphertext
    
    """
    decrypts a given uppercase cipher-text with a key in the same format as substitution encrypt
    """
    def substitution_decrypt(ciphertext, key):
        inverse_key = [ct.pos_char(key.find(ct.pos_char(i))) if key.find(ct.pos_char(i)) != -1 else '_' for i in range(len(key))]
        return ct.substitution_encrypt(ciphertext, inverse_key)

    """ratio of double letter pairs starting on an even index (for detecting Playfair)"""
    def double_ratio(ciphertext):
        total = ct.total_doubles(ciphertext)
        if total == 0:
            return 1
        return 100*ct.inline_doubles(ciphertext)/total

    """number of double letter pairs starting on an even index"""
    def inline_doubles(text):
        if len(text) < 2:
            return 0
        doubles = 0
        for i in range(0, len(text)-1, 2):
            doubles += 1 if text[i] == text[i+1] else 0
        return doubles
    
    """total number of double letter pairs in text"""
    def total_doubles(text):
        if len(text) < 2:
            return 0
        doubles = 0
        for i in range(len(text)-1):
            doubles += 1 if text[i] == text[i+1] else 0
        return doubles
    
    """returns a sorted list of tuples representing digrams and the number of occurences"""
    def digram_freq(text):
        return ct.multigram_freq(text, 2)

    """returns a sorted list of tuples representing trigrams and the number of occurences"""
    def trigram_freq(text):
        return ct.multigram_freq(text, 3)

    """returns a sorted list of tuples representing n-grams and the number of occurences"""
    def multigram_freq(text, gram_len, sig = 1):
        g_dict = {}
        for i in range(len(text) + 1 -  gram_len):
            gram = text[i:i + gram_len]
            if gram in g_dict:
                g_dict[gram] += 1
            else:
                g_dict[gram] = 1
        ret_list = list(filter(lambda x: x[1] >= sig, g_dict.items()))
        return sorted(ret_list, key=lambda x: x[1], reverse=True) # sort by frequency

    """returns inline digram frequencies"""
    def inline_digram_freq(text, sig=1):
        g_dict = {}
        for i in range(0, len(text) - 1, 2):
            gram = text[i:i + 2]
            if gram in g_dict:
                g_dict[gram] += 1
            else:
                g_dict[gram] = 1
        ret_list = list(filter(lambda x: x[1] >= sig, g_dict.items()))
        return sorted(ret_list, key=lambda x: x[1], reverse=True) # sort by frequency

    def inverted_inline_digrams(text, sig=1):
        matches = []
        digrams = ct.inline_digram_freq(text, sig=sig)
        for i, (digram, freq) in enumerate(digrams[:-1]):
            for match, mfreq in digrams[i:]:
                if digram[0] == match[1] and digram[1] == match[0]:
                    matches.append((digram, freq, mfreq))
                    break
        return matches
    def char_mappings(c):
        if c == 'J':
            return 'I'
        elif ord(c) >= 65 and ord(c) < 91:
            return c
        elif c == ' ':
            return ""
        elif c == '.':
            return "DOT"
        elif c == ',':
            return "COMMA"
        elif c == ';':
            return "SEMICOLON"
        return ""
    def preprocess_text(text):
        text = text.upper()
        text = list(text)
        text = "".join(list(map(ct.char_mappings, text)))
        text = list(text)
        for i in range(len(text)-1, 0, -1):
            if  i % 2 and text[i] == text[i-1]:
                text.insert(i, 'X')
        if len(text)%2:
            text.append('X')
        return "".join(text)
    def df_sort_print(df):
        df=df.sort_values(by=["gram"])
        i = 0
        pslist=[]
        while i < 256:
            pslist.append("{\""+df["gram"][i]+"\", "+str(df["frequency"][i])+"}, ")
            if i%8 == 7:
                print(" ".join(pslist))
                pslist=[]
            i+=1
    def generate_playfair_freq_tables():
        texts=[]
        for b in ct.books:
            f = open(b, "r", encoding="utf8")
            texts.append(f.read())
            f.close()
        text = "".join(texts)
        text=ct.preprocess_text(text)
        id_2_freqs=pd.DataFrame(ct.inline_digram_freq(text), columns=["gram", "occurences"])
        g2_freqs=pd.DataFrame(ct.digram_freq(text), columns=["gram", "occurences"])
        g3_freqs=pd.DataFrame(ct.trigram_freq(text), columns=["gram", "occurences"])
        g4_freqs=pd.DataFrame(ct.multigram_freq(text, 4), columns=["gram", "occurences"])
        g5_freqs=pd.DataFrame(ct.multigram_freq(text, 5), columns=["gram", "occurences"])
        
        id_2_freqs["frequency"] = id_2_freqs["occurences"]/id_2_freqs["occurences"].sum()
        id_2_freqs = id_2_freqs.head(256)
        g2_freqs["frequency"] = g2_freqs["occurences"]/g2_freqs["occurences"].sum()
        g2_freqs = g2_freqs.head(256)
        g3_freqs["frequency"] = g3_freqs["occurences"]/g3_freqs["occurences"].sum()
        g3_freqs = g3_freqs.head(256)
        g4_freqs["frequency"] = g4_freqs["occurences"]/g4_freqs["occurences"].sum()
        g4_freqs = g4_freqs.head(256)
        g5_freqs["frequency"] = g5_freqs["occurences"]/g5_freqs["occurences"].sum()
        g5_freqs = g5_freqs.head(256)
        ct.df_sort_print(id_2_freqs)
        print("\n")
        ct.df_sort_print(g2_freqs)
        print("\n")
        ct.df_sort_print(g3_freqs)

    def reverse_lut(lu_table):
        reverse_lu_table = [0] * 256
        for i, entry in enumerate(lu_table):
            rev_index = (entry & 0xFF000000) >> 24
            reverse_lu_table[rev_index] = i
        for i, entry in enumerate(reverse_lu_table[1:]):
            if entry == 0:
                print("got a 0 at " + str(i))
        for i in range(0, 255, 8):
            print(str(reverse_lu_table[i:i+8])[1:-1])
