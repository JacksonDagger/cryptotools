from string import ascii_uppercase
import matplotlib.pyplot as plt

class ct:
    A_pos = 65
    alphabet_size = 26
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
