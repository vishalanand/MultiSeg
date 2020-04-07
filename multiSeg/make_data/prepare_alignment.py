import numpy as np
import argparse, nltk, codecs

def prepare_data(src, tgt, processed_alignment_input):
    f = codecs.open(processed_alignment_input, 'w', 'utf-8-sig')

    with open(src) as textfile1, open(tgt) as textfile2: 
        for x, y in zip(textfile1, textfile2):
            x = x.strip()
            y = y.strip()
            f.write("{} ||| {}\n".format(x, y))

    f.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--src", help="Raw bi-text data")
    parser.add_argument("--tgt", help="Raw bi-text data")
    parser.add_argument("--processed_alignment_input", help="processed alignment input")
    args = parser.parse_args()

    prepare_data(args.src, args.tgt, args.processed_alignment_input)

# python prepare_alignment.py \
# --src /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so \
# --tgt /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.en \
# --processed_alignment_input /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so-en

# python prepare_alignment.py \
# --src /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.dev.tok.so \
# --tgt /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.dev.tok.en \
# --processed_alignment_input /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.dev.tok.so-en

# time ./fast_align/build/fast_align \
#         -i /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so-en \
#         -d -o -v > /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so-en.forward.align
# time ./fast_align/build/fast_align \
#         -i /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so-en \
#         -d -o -v -r > /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so-en.reverse.align
# time ./fast_align/build/atools \
#         -i /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so-en.forward.align \
#         -j /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so-en.reverse.align \
#         -c grow-diag-final-and > /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so-en.align

# time ./fast_align/build/fast_align \
#         -i /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.dev.tok.so-en \
#         -d -o -v > /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.dev.tok.so-en.forward.align
# time ./fast_align/build/fast_align \
#         -i /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.dev.tok.so-en \
#         -d -o -v -r > /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.dev.tok.so-en.reverse.align
# time ./fast_align/build/atools \
#         -i /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.dev.tok.so-en.forward.align \
#         -j /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.dev.tok.so-en.reverse.align \
#         -c grow-diag-final-and > /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.dev.tok.so-en.align

# sed 's/-/ /g' /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so-en.align > /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.space.so-en.align
# sed 's/-/ /g' /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.dev.tok.so-en.align > /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.dev.tok.space.so-en.align


# {  ./bitext skipgram \
# -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
# -call_eval_dict_mult \
# -call_eval_dict_unq \
# -save_bilingual_model \
# -epoch 20 \
# -thread 4 \
# -align /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.space.so-en.align \
# -src-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so \
# -tgt-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.en \
# -output ./test/log/so_word_bitext_40 \
# -efsunDictionary /Users/va/smaraefsunemnlp/so/google_dict.so-en.txt \
# -efsunDictionaryUnq /Users/va/smaraefsunemnlp/so/google_dict.so-en.txt \
# -src-lang so \
# -tgt-lang en \
# -dim 40 ; }

# wc -l /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.space.so-en.align
# wc -l /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so

# {  ./bitext skipgram \
# -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
# -call_eval_dict_mult \
# -call_eval_dict_unq \
# -epoch 20 \
# -thread 4 \
# -align /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.space.so-en.align \
# -src-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so \
# -tgt-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.en \
# -output ./test/log/so_word_bitext_40 \
# -efsunDictionary /Users/va/smaraefsunemnlp/so/google_dict.so-en.txt \
# -efsunDictionaryUnq /Users/va/smaraefsunemnlp/so/google_dict.so-en.txt \
# -src-lang so \
# -tgt-lang en \
# -dim 40 ; }

# {  ./bitext skipgram \
# -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
# -call_eval_dict_mult \
# -epoch 20 \
# -thread 4 \
# -align /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.space.so-en.align \
# -src-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so \
# -tgt-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.en \
# -output ./test/log/so_word_bitext_40 \
# -efsunDictionary /Users/va/smaraefsunemnlp/so/google_dict.so-en.txt \
# -src-lang so \
# -tgt-lang en \
# -dim 40 ; }


# Morphology
# {  ./bitext skipgram \
# -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
# -call_eval_dict_mult \
# -epoch 20 \
# -thread 4 \
# -align /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.space.so-en.align \
# -src-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so \
# -tgt-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.en \
# -output ./test/log/so_word_bitext_40_morph_3p \
# -efsunDictionary /Users/va/smaraefsunemnlp/so/google_dict.so-en.txt \
# -src-lang so \
# -tgt-lang en \
# -dim 40 \
# -src_map_f /Users/va/smaraefsunemnlp/so_morph/so.morph_3p_map.so \
# -tgt_map_f /Users/va/smaraefsunemnlp/so_morph/so.morph_3p_map.en \
# -use_morphology ; }

# {  ./bitext skipgram \
# -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
# -call_eval_dict_mult \
# -epoch 20 \
# -thread 4 \
# -align /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.space.so-en.align \
# -src-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so \
# -tgt-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.en \
# -output ./test/log/so_word_bitext_40_morph_3 \
# -efsunDictionary /Users/va/smaraefsunemnlp/so/google_dict.so-en.txt \
# -src-lang so \
# -tgt-lang en \
# -dim 40 \
# -src_map_f /Users/va/smaraefsunemnlp/so_morph/so.morph_3_map.so \
# -tgt_map_f /Users/va/smaraefsunemnlp/so_morph/so.morph_3_map.en \
# -use_morphology ; }



# python3 computeWordTranslation.py \
#   --emb /Users/va/smaraefsunemnlp/parFastText/parfasttext/test/log/so_word_bitext_40.so.iter1.vec.so-en.all \
#   --dic /Users/va/smaraefsunemnlp/so/google_dict.so-en.txt \
#   --prefix so \
#   --merge_two_embeddings True \
#   --embedding_pr /Users/va/smaraefsunemnlp/parFastText/parfasttext/test/log/so_word_bitext_40.so.iter1.vec \
#   --embedding_en /Users/va/smaraefsunemnlp/parFastText/parfasttext/test/log/so_word_bitext_40.en.iter1.vec

# { make && time ./bivec \
#     -cbow 0 \
#     -window 5 \
#     -negative 30 \
#     -hs 0 \
#     -binary 0 \
#     -eval 0 \
#     -min-count 5 \
#     -sample 1e-4 \
#     -tgt-sample 1e-4 \
#     -bi-weight 1 \
#     -align /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.space.so-en.align \
#     -src-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so \
#     -tgt-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.en \
#     -output /Users/va/smaraefsunemnlp/parFastText/parfasttext/test/log/bivec/so_word_bivec_40 \
#     -size 40 \
#     -iter 20 \
#     -threads 12 \
#     -align-opt 1 \
#     -src-lang so \
#     -tgt-lang en \
#     -provided_translation_dictionary /Users/va/smaraefsunemnlp/so/google_dict.so-en.txt \
#     -call_eval_mono 0 \
#     -call_eval_cldc 0 \
#     -call_eval_muse 0 \
#     -call_eval_dict_mult 1 ; } > /Users/va/smaraefsunemnlp/parFastText/parfasttext/test/log/bivec/so_word_bivec_40_out.txt 2> /Users/va/smaraefsunemnlp/parFastText/parfasttext/test/log/bivec/so_word_bivec_40_cerr.txt

# { make && time ./bivec \
#     -cbow 0 \
#     -window 5 \
#     -negative 30 \
#     -hs 0 \
#     -binary 0 \
#     -eval 0 \
#     -min-count 5 \
#     -sample 1e-4 \
#     -tgt-sample 1e-4 \
#     -bi-weight 1 \
#     -align /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.space.so-en.align \
#     -src-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so \
#     -tgt-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.en \
#     -output /Users/va/smaraefsunemnlp/parFastText/parfasttext/test/log/bivec/so_word_bivec_300 \
#     -size 300 \
#     -iter 20 \
#     -threads 12 \
#     -align-opt 1 \
#     -src-lang so \
#     -tgt-lang en \
#     -provided_translation_dictionary /Users/va/smaraefsunemnlp/so/google_dict.so-en.txt \
#     -call_eval_mono 0 \
#     -call_eval_cldc 0 \
#     -call_eval_muse 0 \
#     -call_eval_dict_mult 1 ; } > /Users/va/smaraefsunemnlp/parFastText/parfasttext/test/log/bivec/so_word_bivec_300_out.txt 2> /Users/va/smaraefsunemnlp/parFastText/parfasttext/test/log/bivec/so_word_bivec_300_cerr.txt

# {  ./bitext skipgram \
# -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
# -call_eval_dict_mult \
# -epoch 20 \
# -thread 4 \
# -align /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.space.so-en.align \
# -src-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.so \
# -tgt-train /Users/va/smaraefsunemnlp/so/MATERIAL_BASE-1S-BUILD.train.tok.en \
# -output ./test/log/so_word_bitext_300 \
# -efsunDictionary /Users/va/smaraefsunemnlp/so/google_dict.so-en.txt \
# -src-lang so \
# -tgt-lang en \
# -dim 300 ; }

# (base) [va2361@node277 data]$ mkdir processed
# (base) [va2361@node277 data]$ mkdir -p processed/XLM_en_of_so/
# (base) [va2361@node277 data]$ OUTPATH=processed/XLM_en_of_so
# (base) [va2361@node277 data]$ cd ../
# (base) [va2361@node277 XLM]$ OUTPATH=data/processed/XLM_en_of_so
# (base) [va2361@node277 XLM]$ FASTBPE=tools/fastBPE/fast
# (base) [va2361@node277 XLM]$ $FASTBPE learnbpe ^C
# (base) [va2361@node277 XLM]$ wc -l data/*.tok.*
#     844 data/MATERIAL_BASE-1S-BUILD.dev.tok.en
#     844 data/MATERIAL_BASE-1S-BUILD.dev.tok.so
#   24000 data/MATERIAL_BASE-1S-BUILD.train.tok.en
#   24000 data/MATERIAL_BASE-1S-BUILD.train.tok.so
#   49688 total
# (base) [va2361@node277 XLM]$ $FASTBPE learnbpe 24000 data/MATERIAL_BASE-1S-BUILD.train.tok.en > $OUTPATH/codes
# $FASTBPE applybpe $OUTPATH/train-so-en.en data/MATERIAL_BASE-1S-BUILD.train.tok.en $OUTPATH/codes
# $FASTBPE applybpe $OUTPATH/dev-so-en.en data/MATERIAL_BASE-1S-BUILD.dev.tok.en $OUTPATH/codes
# cat $OUTPATH/train-so-en.en | $FASTBPE getvocab - > $OUTPATH/vocab

# python preprocess.py $OUTPATH/vocab $OUTPATH/train-so-en.en
# python preprocess.py $OUTPATH/vocab $OUTPATH/dev-so-en.en








# (base) [va2361@node277 XLM]$ mkdir -p data/processed/XLM_so_of_so/
# (base) [va2361@node277 XLM]$ OUTPATH=data/processed/XLM_so_of_so
# (base) [va2361@node277 XLM]$ FASTBPE=tools/fastBPE/fast
# (base) [va2361@node277 XLM]$ wc -l data/*.tok.*
#     844 data/MATERIAL_BASE-1S-BUILD.dev.tok.en
#     844 data/MATERIAL_BASE-1S-BUILD.dev.tok.so
#   24000 data/MATERIAL_BASE-1S-BUILD.train.tok.en
#   24000 data/MATERIAL_BASE-1S-BUILD.train.tok.so
#   49688 total
# (base) [va2361@node277 XLM]$ $FASTBPE learnbpe 24000 data/MATERIAL_BASE-1S-BUILD.train.tok.so > $OUTPATH/codes
# $FASTBPE applybpe $OUTPATH/train-so-en.so data/MATERIAL_BASE-1S-BUILD.train.tok.so $OUTPATH/codes
# $FASTBPE applybpe $OUTPATH/dev-so-en.so data/MATERIAL_BASE-1S-BUILD.dev.tok.so $OUTPATH/codes
# cat $OUTPATH/train-so-en.so | $FASTBPE getvocab - > $OUTPATH/vocab

# python preprocess.py $OUTPATH/vocab $OUTPATH/train-so-en.so
# python preprocess.py $OUTPATH/vocab $OUTPATH/dev-so-en.so





# OUTPATH=

# python train.py \
# --exp_name xlm_en_so \
# --dump_path $OUTPATH/dump_xlm_mlm \
# --data_path $OUTPATH \
# --lgs 'en-so' \
# --clm_steps '' \
# --mlm_steps 'en,so' \
# --emb_dim 1024 \
# --n_layers 12 \
# --n_heads 16 \
# --dropout 0.1 \
# --attention_dropout 0.1 \
# --gelu_activation true \
# --batch_size 32 \
# --bptt 256 \
# --optimizer adam,lr=0.0001 \
# --epoch_size 300000 \
# --max_epoch 100000 \
# --validation_metrics _valid_mlm_ppl \
# --stopping_criterion _valid_mlm_ppl,25 \
# --fp16 true

# data/processed/XLM_so_of_so/
# dev-so-en.so
# dev-so-en.so.pth
# train-so-en.so
# train-so-en.so.pth

# data/processed/XLM_en_of_so/
# dev-so-en.en
# dev-so-en.en.pth
# train-so-en.en
# train-so-en.en.pth



# cp data/processed/XLM_en_of_so/dev-so-en.en $OUTPATH/en-so.en.dev
# cp data/processed/XLM_en_of_so/dev-so-en.en.pth $OUTPATH/en-so.en.dev.pth
# cp data/processed/XLM_en_of_so/train-so-en.en $OUTPATH/en-so.en.train
# cp data/processed/XLM_en_of_so/train-so-en.en.pth $OUTPATH/en-so.en.train.pth

# cp data/processed/XLM_so_of_so/dev-so-en.so $OUTPATH/en-so.so.dev
# cp data/processed/XLM_so_of_so/dev-so-en.so.pth $OUTPATH/en-so.so.dev.pth
# cp data/processed/XLM_so_of_so/train-so-en.so $OUTPATH/en-so.so.train
# cp data/processed/XLM_so_of_so/train-so-en.so.pth $OUTPATH/en-so.so.train.pth


# OUTPATH=data/processed/en-so-xlm
# en-so.so.train
# en-so.so.dev

# en-so.en.train
# en-so.en.dev

# /rigel/dsi/users/va2361/XLM/data/processed/en-so



# cd /rigel/dsi/users/va2361/XLM
# (base) [va2361@node025 en-so]$ cp ../../../../../ek3050/vishal/so/MATERIAL_BASE-1S-BUILD.* ./input
# cd input/
# mv MATERIAL_BASE-1S-BUILD.dev.tok.en so.valid
# mv MATERIAL_BASE-1S-BUILD.dev.tok.so so.valid
# mv MATERIAL_BASE-1S-BUILD.train.tok.en en.train
# mv MATERIAL_BASE-1S-BUILD.train.tok.so so.train
# cd ../../../../
# OUTPATH=data/processed/en-so
# FASTBPE=tools/fastBPE/fast
# $FASTBPE learnbpe 24000 $OUTPATH/input/en.train > $OUTPATH/codes.en
# $FASTBPE learnbpe 24000 $OUTPATH/input/so.train > $OUTPATH/codes.so

# $FASTBPE applybpe $OUTPATH/train.en $OUTPATH/input/en.train $OUTPATH/codes.en
# $FASTBPE applybpe $OUTPATH/valid.en $OUTPATH/input/en.valid $OUTPATH/codes.en
# $FASTBPE applybpe $OUTPATH/train.so $OUTPATH/input/so.train $OUTPATH/codes.so
# $FASTBPE applybpe $OUTPATH/valid.so $OUTPATH/input/so.valid $OUTPATH/codes.so

# cat $OUTPATH/train.en | $FASTBPE getvocab - > $OUTPATH/vocab.en
# cat $OUTPATH/train.so | $FASTBPE getvocab - > $OUTPATH/vocab.so

# python preprocess.py $OUTPATH/vocab.en $OUTPATH/train.en
# python preprocess.py $OUTPATH/vocab.en $OUTPATH/valid.en
# python preprocess.py $OUTPATH/vocab.so $OUTPATH/train.so
# python preprocess.py $OUTPATH/vocab.so $OUTPATH/valid.so



# ../../../../XLM_old/data/processed/

# pair=en-so

# for lg in $(echo $pair | sed -e 's/\-/ /g'); do
#   for split in train valid; do
#     $FASTBPE applybpe $OUTPATH/$pair.$lg.$split data/wiki/para/$pair.$lg.$split $OUTPATH/codes
#     python preprocess.py $OUTPATH/vocab $OUTPATH/$pair.$lg.$split
#   done
# done

# $FASTBPE learnbpe 24000 $OUTPATH/en-so.en.train > $OUTPATH/codes.en
# $FASTBPE learnbpe 24000 $OUTPATH/en-so.so.train > $OUTPATH/codes.so
# $FASTBPE applybpe $OUTPATH/train.en $OUTPATH/en.train $OUTPATH/codes.en
# $FASTBPE applybpe $OUTPATH/valid.en $OUTPATH/en.valid $OUTPATH/codes.en

# $FASTBPE applybpe $OUTPATH/train.so $OUTPATH/so.train $OUTPATH/codes.so
# $FASTBPE applybpe $OUTPATH/valid.so $OUTPATH/so.valid $OUTPATH/codes.so

# cat $OUTPATH/train.en | $FASTBPE getvocab - > $OUTPATH/vocab.en
# cat $OUTPATH/train.so | $FASTBPE getvocab - > $OUTPATH/vocab.so



# $FASTBPE applybpe $OUTPATH/en-so.en.train data/wiki/para/en-so.en.train $OUTPATH/codes.en
# python preprocess.py $OUTPATH/vocab.en $OUTPATH/en-so.en.train

# $FASTBPE applybpe $OUTPATH/en-so.en.valid data/wiki/para/en-so.en.valid $OUTPATH/codes.en
# python preprocess.py $OUTPATH/vocab.en $OUTPATH/en-so.en.valid


# $FASTBPE applybpe $OUTPATH/en-so.so.train data/wiki/para/en-so.so.train $OUTPATH/codes.so
# python preprocess.py $OUTPATH/vocab.so $OUTPATH/en-so.so.train

# $FASTBPE applybpe $OUTPATH/en-so.so.valid data/wiki/para/en-so.so.valid $OUTPATH/codes.so
# python preprocess.py $OUTPATH/vocab.so $OUTPATH/en-so.so.valid
