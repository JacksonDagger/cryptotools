from string import ascii_uppercase
import matplotlib.pyplot as plt
import pandas as pd

class ct:
    A_pos = 65
    alphabet_size = 26
    books = ["alice.txt", "lg.txt"]
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
        inverse_key = "".join(list(map(lambda x: ct.char_shift(x, 13), keyword)))
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
        



