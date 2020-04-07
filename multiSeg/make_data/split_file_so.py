import sys, pandas, csv, os, pickle, codecs
#from scipy import spatial
import argparse, numpy, os
from sklearn.metrics.pairwise import cosine_similarity
from scipy.spatial.distance import cdist, pdist, squareform
from timeit import default_timer as timer
from scipy.stats.mstats import rankdata

# vecmap-sup-glosbe-so-sub.vec
# vecmap-sup-so-sub.vec
# vecmap-sup-pan-so-sub.vec
# vecmap-sup-wikt-so-sub.vec
# vecmap-sup-sw-sub.vec
# vecmap-sup-tl-sub.vec
# /rigel/dsi/users/va2361/parFastText/word-embeddings/vecmap_sup
# /rigel/dsi/users/ek3050/vishal/vecmap-sup-sw-sub.vec

file_sw = "/Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok"
file_sw_sw = "{}.{}".format(file_sw, "so")
file_sw_en = "{}.{}".format(file_sw, "en")

file_tl = "/Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.dev.tok"
file_tl_tl = "{}.{}".format(file_tl, "tl")
file_tl_en = "{}.{}".format(file_tl, "en")


def split_file(file_l, file_l_l, file_l_en):
    f_l_l = codecs.open(file_l_l, 'w', 'utf-8-sig')
    f_l_en = codecs.open(file_l_en, 'w', 'utf-8-sig')

    cnt_en = 0
    cnt_l = 0
    dim = -1

    with codecs.open(file_l, 'r', 'utf-8-sig') as read_f:
        for line in read_f:    
            if(dim < 0):
                dim = len(line.split()) - 1
            if(line[:3] == "en:"):
                cnt_en = cnt_en + 1
            else:
                cnt_l = cnt_l + 1
    # print("{}_{}_{}".format(cnt_en, cnt_l, dim))

    f_l_en.write("{} {}\n".format(cnt_en, dim))
    f_l_l.write("{} {}\n".format(cnt_l, dim))

    with codecs.open(file_l, 'r', 'utf-8-sig') as read_f:
        for line in read_f:
            if(line[:3] == "en:"):
                f_l_en.write(line[3:])
            else:
                f_l_l.write(line[3:])

    f_l_l.close()
    f_l_en.close()

split_file(file_sw, file_sw_sw, file_sw_en)
split_file(file_tl, file_tl_tl, file_tl_en)
