import re, collections
from collections import defaultdict
from tqdm import tqdm

#verbose = True
verbose = False
num_merges = 10000

def get_stats(vocab):
    """Compute frequencies of adjacent pairs of symbols."""
    pairs = collections.defaultdict(int)
    for word, freq in vocab.items():
        symbols = word.split()
        for i in range(len(symbols)-1):
            pairs[symbols[i],symbols[i+1]] += freq
    return pairs

def merge_vocab(pair, v_in):
    v_out = {}
    bigram = re.escape(' '.join(pair))
    p = re.compile(r'(?<!\S)' + bigram + r'(?!\S)')
    for word in v_in:
        w_out = p.sub(''.join(pair), word)
        v_out[w_out] = v_in[word]
    return v_out

def vocab_words(train_data):

    bpe_codes = {}
    bpe_codes_reverse = {}

    for i in tqdm(range(num_merges)):
        if verbose:
            print("### Iteration {}".format(i + 1))
        pairs = get_stats(train_data)
        best = max(pairs, key=pairs.get)
        train_data = merge_vocab(best, train_data)
        
        bpe_codes[best] = i
        bpe_codes_reverse[best[0] + best[1]] = best
        
        if verbose:
            print("new merge: {}".format(best))
            print("train data: {}".format(train_data))

    return bpe_codes, bpe_codes_reverse, train_data

def get_pairs(word):
    """Return set of symbol pairs in a word.
    Word is represented as a tuple of symbols (symbols being variable-length strings).
    """
    pairs = set()
    prev_char = word[0]
    for char in word[1:]:
        pairs.add((prev_char, char))
        prev_char = char
    return pairs


def encode(orig):
    """Encode word based on list of BPE merge operations, which are applied consecutively"""

    word = tuple(orig) + ('</w>',)
    if verbose:
        print("__word split into characters:__ <tt>{}</tt>".format(word))
    pairs = get_pairs(word)

    if not pairs:
        return orig

    iteration = 0
    while True:
        iteration += 1
        if verbose:
            print("__Iteration {}:__".format(iteration))
            print("bigrams in the word: {}".format(pairs))
        bigram = min(pairs, key = lambda pair: bpe_codes.get(pair, float('inf')))
        if verbose:
            print("candidate for merging: {}".format(bigram))
        if bigram not in bpe_codes:
            if verbose:
                print("__Candidate not in BPE merges, algorithm stops.__")
            break
        first, second = bigram
        new_word = []
        i = 0
        while i < len(word):
            try:
                j = word.index(first, i)
                new_word.extend(word[i:j])
                i = j
            except:
                new_word.extend(word[i:])
                break

            if word[i] == first and i < len(word)-1 and word[i+1] == second:
                new_word.append(first+second)
                i += 2
            else:
                new_word.append(word[i])
                i += 1
        new_word = tuple(new_word)
        word = new_word
        if verbose:
            print("word after merging: {}".format(word))
        if len(word) == 1:
            break
        else:
            pairs = get_pairs(word)

    # don't print end-of-word symbols
    if word[-1] == '</w>':
        word = word[:-1]
    elif word[-1].endswith('</w>'):
        word = word[:-1] + (word[-1].replace('</w>',''),)
   
    return word

#bpes = ["/Users/va/smaraefsunemnlp/so_morph/so.word.en", "/Users/va/smaraefsunemnlp/so_morph/so.word.so", "/Users/va/smaraefsunemnlp/so_morph/bpe.test"]
bpes = [
    # "/Users/va/Smara_MATERIAL/parFastText/segmentations/morph_map_final/SW/sw.word.en", 
    # "/Users/va/Smara_MATERIAL/parFastText/segmentations/morph_map_final/SW/sw.word.sw", 
    # "/Users/va/Smara_MATERIAL/parFastText/segmentations/morph_map_final/TL/tl.word.en",
    # "/Users/va/Smara_MATERIAL/parFastText/segmentations/morph_map_final/TL/tl.word.tl",
    # "/Users/va/Smara_MATERIAL/parFastText/segmentations/morph_map_final/Europarl/mini/100k/100k_europarl.word.de",
    "/Users/va/Smara_MATERIAL/parFastText/segmentations/morph_map_final/Europarl/mini/100k/100k_europarl.word.en"]
#bpes = ["/Users/va/smaraefsunemnlp/so_morph/bpe.test"]
for bpe in bpes:
    freq_count = {}
    freq_count = collections.defaultdict(int)
    fw = open(bpe + "." + str(num_merges) + ".map_bpe","w")
    with open(bpe, 'r') as file:
        for line in file:
            split_line = line.split()
            for word in split_line:
                freq_count[" ".join(word) + ' </w>'] += 1
    train_data = dict(freq_count)
    #train_data = {'l o w </w>': 5, 'l o w e r </w>': 2, 'n e w e s t </w>': 6, 'w i d e s t </w>': 3}
    #print("train data: {}".format(train_data))
    bpe_codes, bpe_codes_reverse, train_data = vocab_words(train_data)
    # print("train data: {}".format(train_data))
    # print("word after merging: {}".format(encode("lowest")))
    for word in freq_count:
        word = word.replace(' ', '')[:-4]
        #print("{}\t{}".format(word, "\t".join(encode(word))))
        fw.write("{}\t{}\n".format(word, "\t".join(encode(word))))
    fw.close()
