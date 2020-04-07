set -x
#Bivec
{ make && time ./bivec \
    -cbow 0 -window 5 -negative 30 -hs 0 -binary 0 -eval 0 -min-count 5 -sample 1e-4 -tgt-sample 1e-4 -bi-weight 1 \
    -call_eval_mono 0 -call_eval_cldc 0 -call_eval_muse 0 \
    -align ../segmentations/morph_map_final/tl.word.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_word_bivec_40 \
    -size 40 \
    -iter 30 \
    -threads 12 \
    -align-opt 1 \
    -src-lang tl \
    -tgt-lang en \
    -provided_translation_dictionary ../data/en_tl.txt \
    -provided_translation_dictionary_unq ../data/en_tl_unq.txt \
    -call_eval_dict_mult 1 \
    -call_eval_dict_unq 1 ; } > ../word-embeddings/TL/tl_word_bivec_40_out.txt 2> ../word-embeddings/TL/tl_word_bivec_40_cerr.txt
{ make && time ./bivec \
    -cbow 0 -window 5 -negative 30 -hs 0 -binary 0 -eval 0 -min-count 5 -sample 1e-4 -tgt-sample 1e-4 -bi-weight 1 \
    -call_eval_mono 0 -call_eval_cldc 0 -call_eval_muse 0 \
    -align ../segmentations/morph_map_final/tl.stem.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_stem_bivec_40 \
    -size 40 \
    -iter 30 \
    -threads 12 \
    -align-opt 1 \
    -src-lang tl \
    -tgt-lang en \
    -provided_translation_dictionary ../data/en_tl.txt \
    -provided_translation_dictionary_unq ../data/en_tl_unq.txt \
    -call_eval_dict_mult 1 \
    -call_eval_dict_unq 1 ; } > ../word-embeddings/TL/tl_stem_bivec_40_out.txt 2> ../word-embeddings/TL/tl_stem_bivec_40_cerr.txt
{ make && time ./bivec \
    -cbow 0 -window 5 -negative 30 -hs 0 -binary 0 -eval 0 -min-count 5 -sample 1e-4 -tgt-sample 1e-4 -bi-weight 1 \
    -call_eval_mono 0 -call_eval_cldc 0 -call_eval_muse 0 \
    -align ../segmentations/morph_map_final/tl.word.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_word_bivec_300 \
    -size 300 \
    -iter 30 \
    -threads 12 \
    -align-opt 1 \
    -src-lang tl \
    -tgt-lang en \
    -provided_translation_dictionary ../data/en_tl.txt \
    -provided_translation_dictionary_unq ../data/en_tl_unq.txt \
    -call_eval_dict_mult 1 \
    -call_eval_dict_unq 1 ; } > ../word-embeddings/TL/tl_word_bivec_300_out.txt 2> ../word-embeddings/TL/tl_word_bivec_300_cerr.txt
{ make && time ./bivec \
    -cbow 0 -window 5 -negative 30 -hs 0 -binary 0 -eval 0 -min-count 5 -sample 1e-4 -tgt-sample 1e-4 -bi-weight 1 \
    -call_eval_mono 0 -call_eval_cldc 0 -call_eval_muse 0 \
    -align ../segmentations/morph_map_final/tl.stem.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_stem_bivec_300 \
    -size 300 \
    -iter 30 \
    -threads 12 \
    -align-opt 1 \
    -src-lang tl \
    -tgt-lang en \
    -provided_translation_dictionary ../data/en_tl.txt \
    -provided_translation_dictionary_unq ../data/en_tl_unq.txt \
    -call_eval_dict_mult 1 \
    -call_eval_dict_unq 1 ; } > ../word-embeddings/TL/tl_stem_bivec_300_out.txt 2> ../word-embeddings/TL/tl_stem_bivec_300_cerr.txt
set +x