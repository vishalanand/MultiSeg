set -x
#parFastText 40
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/sw.word.space.sw-en.align \
    -src-train ../segmentations/morph_map_final/sw.word.sw \
    -tgt-train ../segmentations/morph_map_final/sw.word.en \
    -output ../word-embeddings/SW/sw_word_bitext_40 \
    -efsunDictionary ../data/en_sw.txt \
    -efsunDictionaryUnq ../data/en_sw_unq.txt \
    -src-lang sw \
    -tgt-lang en \
    -dim 40 ; } > ../word-embeddings/SW/sw_word_bitext_40_out.txt 2> ../word-embeddings/SW/sw_word_bitext_40_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/sw.stem.space.sw-en.align \
    -src-train ../segmentations/morph_map_final/sw.word.sw \
    -tgt-train ../segmentations/morph_map_final/sw.word.en \
    -output ../word-embeddings/SW/sw_stem_bitext_40 \
    -efsunDictionary ../data/en_sw.txt \
    -efsunDictionaryUnq ../data/en_sw_unq.txt \
    -src-lang sw \
    -tgt-lang en \
    -dim 40 ; } > ../word-embeddings/SW/sw_stem_bitext_40_out.txt 2> ../word-embeddings/SW/sw_stem_bitext_40_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/sw.word.space.sw-en.align \
    -src-train ../segmentations/morph_map_final/sw.word.sw \
    -tgt-train ../segmentations/morph_map_final/sw.word.en \
    -output ../word-embeddings/SW/sw_word_morph_3_bitext_40 \
    -efsunDictionary ../data/en_sw.txt \
    -efsunDictionaryUnq ../data/en_sw_unq.txt \
    -src-lang sw \
    -tgt-lang en \
    -dim 40 \
    -src_map_f ../segmentations/morph_map_final/sw.morph_3_map.sw \
    -tgt_map_f ../segmentations/morph_map_final/sw.morph_3_map.en \
    -use_morphology ; } > ../word-embeddings/SW/sw_word_morph_3_bitext_40_out.txt 2> ../word-embeddings/SW/sw_word_morph_3_bitext_40_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/sw.stem.space.sw-en.align \
    -src-train ../segmentations/morph_map_final/sw.word.sw \
    -tgt-train ../segmentations/morph_map_final/sw.word.en \
    -output ../word-embeddings/SW/sw_stem_morph_3_bitext_40 \
    -efsunDictionary ../data/en_sw.txt \
    -efsunDictionaryUnq ../data/en_sw_unq.txt \
    -src-lang sw \
    -tgt-lang en \
    -dim 40 \
    -src_map_f ../segmentations/morph_map_final/sw.morph_3_map.sw \
    -tgt_map_f ../segmentations/morph_map_final/sw.morph_3_map.en \
    -use_morphology ; } > ../word-embeddings/SW/sw_stem_morph_3_bitext_40_out.txt 2> ../word-embeddings/SW/sw_stem_morph_3_bitext_40_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/sw.word.space.sw-en.align \
    -src-train ../segmentations/morph_map_final/sw.word.sw \
    -tgt-train ../segmentations/morph_map_final/sw.word.en \
    -output ../word-embeddings/SW/sw_word_morph_3p_bitext_40 \
    -efsunDictionary ../data/en_sw.txt \
    -efsunDictionaryUnq ../data/en_sw_unq.txt \
    -src-lang sw \
    -tgt-lang en \
    -dim 40 \
    -src_map_f ../segmentations/morph_map_final/sw.morph_3p_map.sw \
    -tgt_map_f ../segmentations/morph_map_final/sw.morph_3p_map.en \
    -use_morphology ; } > ../word-embeddings/SW/sw_word_morph_3p_bitext_40_out.txt 2> ../word-embeddings/SW/sw_word_morph_3p_bitext_40_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/sw.stem.space.sw-en.align \
    -src-train ../segmentations/morph_map_final/sw.word.sw \
    -tgt-train ../segmentations/morph_map_final/sw.word.en \
    -output ../word-embeddings/SW/sw_stem_morph_3p_bitext_40 \
    -efsunDictionary ../data/en_sw.txt \
    -efsunDictionaryUnq ../data/en_sw_unq.txt \
    -src-lang sw \
    -tgt-lang en \
    -dim 40 \
    -src_map_f ../segmentations/morph_map_final/sw.morph_3p_map.sw \
    -tgt_map_f ../segmentations/morph_map_final/sw.morph_3p_map.en \
    -use_morphology ; } > ../word-embeddings/SW/sw_stem_morph_3p_bitext_40_out.txt 2> ../word-embeddings/SW/sw_stem_morph_3p_bitext_40_cerr.txt





#parFastText 300
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/sw.word.space.sw-en.align \
    -src-train ../segmentations/morph_map_final/sw.word.sw \
    -tgt-train ../segmentations/morph_map_final/sw.word.en \
    -output ../word-embeddings/SW/sw_word_bitext_300 \
    -efsunDictionary ../data/en_sw.txt \
    -efsunDictionaryUnq ../data/en_sw_unq.txt \
    -src-lang sw \
    -tgt-lang en \
    -dim 300 ; } > ../word-embeddings/SW/sw_word_bitext_300_out.txt 2> ../word-embeddings/SW/sw_word_bitext_300_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/sw.stem.space.sw-en.align \
    -src-train ../segmentations/morph_map_final/sw.word.sw \
    -tgt-train ../segmentations/morph_map_final/sw.word.en \
    -output ../word-embeddings/SW/sw_stem_bitext_300 \
    -efsunDictionary ../data/en_sw.txt \
    -efsunDictionaryUnq ../data/en_sw_unq.txt \
    -src-lang sw \
    -tgt-lang en \
    -dim 300 ; } > ../word-embeddings/SW/sw_stem_bitext_300_out.txt 2> ../word-embeddings/SW/sw_stem_bitext_300_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/sw.word.space.sw-en.align \
    -src-train ../segmentations/morph_map_final/sw.word.sw \
    -tgt-train ../segmentations/morph_map_final/sw.word.en \
    -output ../word-embeddings/SW/sw_word_morph_3_bitext_300 \
    -efsunDictionary ../data/en_sw.txt \
    -efsunDictionaryUnq ../data/en_sw_unq.txt \
    -src-lang sw \
    -tgt-lang en \
    -dim 300 \
    -src_map_f ../segmentations/morph_map_final/sw.morph_3_map.sw \
    -tgt_map_f ../segmentations/morph_map_final/sw.morph_3_map.en \
    -use_morphology ; } > ../word-embeddings/SW/sw_word_morph_3_bitext_300_out.txt 2> ../word-embeddings/SW/sw_word_morph_3_bitext_300_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/sw.stem.space.sw-en.align \
    -src-train ../segmentations/morph_map_final/sw.word.sw \
    -tgt-train ../segmentations/morph_map_final/sw.word.en \
    -output ../word-embeddings/SW/sw_stem_morph_3_bitext_300 \
    -efsunDictionary ../data/en_sw.txt \
    -efsunDictionaryUnq ../data/en_sw_unq.txt \
    -src-lang sw \
    -tgt-lang en \
    -dim 300 \
    -src_map_f ../segmentations/morph_map_final/sw.morph_3_map.sw \
    -tgt_map_f ../segmentations/morph_map_final/sw.morph_3_map.en \
    -use_morphology ; } > ../word-embeddings/SW/sw_stem_morph_3_bitext_300_out.txt 2> ../word-embeddings/SW/sw_stem_morph_3_bitext_300_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/sw.word.space.sw-en.align \
    -src-train ../segmentations/morph_map_final/sw.word.sw \
    -tgt-train ../segmentations/morph_map_final/sw.word.en \
    -output ../word-embeddings/SW/sw_word_morph_3p_bitext_300 \
    -efsunDictionary ../data/en_sw.txt \
    -efsunDictionaryUnq ../data/en_sw_unq.txt \
    -src-lang sw \
    -tgt-lang en \
    -dim 300 \
    -src_map_f ../segmentations/morph_map_final/sw.morph_3p_map.sw \
    -tgt_map_f ../segmentations/morph_map_final/sw.morph_3p_map.en \
    -use_morphology ; } > ../word-embeddings/SW/sw_word_morph_3p_bitext_300_out.txt 2> ../word-embeddings/SW/sw_word_morph_3p_bitext_300_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/sw.stem.space.sw-en.align \
    -src-train ../segmentations/morph_map_final/sw.word.sw \
    -tgt-train ../segmentations/morph_map_final/sw.word.en \
    -output ../word-embeddings/SW/sw_stem_morph_3p_bitext_300 \
    -efsunDictionary ../data/en_sw.txt \
    -efsunDictionaryUnq ../data/en_sw_unq.txt \
    -src-lang sw \
    -tgt-lang en \
    -dim 300 \
    -src_map_f ../segmentations/morph_map_final/sw.morph_3p_map.sw \
    -tgt_map_f ../segmentations/morph_map_final/sw.morph_3p_map.en \
    -use_morphology ; } > ../word-embeddings/SW/sw_stem_morph_3p_bitext_300_out.txt 2> ../word-embeddings/SW/sw_stem_morph_3p_bitext_300_cerr.txt
set +x