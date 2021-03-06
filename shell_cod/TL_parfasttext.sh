set -x
#parFastText 40
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/tl.word.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_word_bitext_40 \
    -efsunDictionary ../data/en_tl.txt \
    -efsunDictionaryUnq ../data/en_tl_unq.txt \
    -src-lang tl \
    -tgt-lang en \
    -dim 40 ; } > ../word-embeddings/TL/tl_word_bitext_40_out.txt 2> ../word-embeddings/TL/tl_word_bitext_40_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/tl.stem.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_stem_bitext_40 \
    -efsunDictionary ../data/en_tl.txt \
    -efsunDictionaryUnq ../data/en_tl_unq.txt \
    -src-lang tl \
    -tgt-lang en \
    -dim 40 ; } > ../word-embeddings/TL/tl_stem_bitext_40_out.txt 2> ../word-embeddings/TL/tl_stem_bitext_40_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/tl.word.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_word_morph_3_bitext_40 \
    -efsunDictionary ../data/en_tl.txt \
    -efsunDictionaryUnq ../data/en_tl_unq.txt \
    -src-lang tl \
    -tgt-lang en \
    -dim 40 \
    -src_map_f ../segmentations/morph_map_final/tl.morph_3_map.tl \
    -tgt_map_f ../segmentations/morph_map_final/tl.morph_3_map.en \
    -use_morphology ; } > ../word-embeddings/TL/tl_word_morph_3_bitext_40_out.txt 2> ../word-embeddings/TL/tl_word_morph_3_bitext_40_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/tl.stem.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_stem_morph_3_bitext_40 \
    -efsunDictionary ../data/en_tl.txt \
    -efsunDictionaryUnq ../data/en_tl_unq.txt \
    -src-lang tl \
    -tgt-lang en \
    -dim 40 \
    -src_map_f ../segmentations/morph_map_final/tl.morph_3_map.tl \
    -tgt_map_f ../segmentations/morph_map_final/tl.morph_3_map.en \
    -use_morphology ; } > ../word-embeddings/TL/tl_stem_morph_3_bitext_40_out.txt 2> ../word-embeddings/TL/tl_stem_morph_3_bitext_40_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/tl.word.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_word_morph_3p_bitext_40 \
    -efsunDictionary ../data/en_tl.txt \
    -efsunDictionaryUnq ../data/en_tl_unq.txt \
    -src-lang tl \
    -tgt-lang en \
    -dim 40 \
    -src_map_f ../segmentations/morph_map_final/tl.morph_3p_map.tl \
    -tgt_map_f ../segmentations/morph_map_final/tl.morph_3p_map.en \
    -use_morphology ; } > ../word-embeddings/TL/tl_word_morph_3p_bitext_40_out.txt 2> ../word-embeddings/TL/tl_word_morph_3p_bitext_40_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/tl.stem.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_stem_morph_3p_bitext_40 \
    -efsunDictionary ../data/en_tl.txt \
    -efsunDictionaryUnq ../data/en_tl_unq.txt \
    -src-lang tl \
    -tgt-lang en \
    -dim 40 \
    -src_map_f ../segmentations/morph_map_final/tl.morph_3p_map.tl \
    -tgt_map_f ../segmentations/morph_map_final/tl.morph_3p_map.en \
    -use_morphology ; } > ../word-embeddings/TL/tl_stem_morph_3p_bitext_40_out.txt 2> ../word-embeddings/TL/tl_stem_morph_3p_bitext_40_cerr.txt





#parFastText 300
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/tl.word.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_word_bitext_300 \
    -efsunDictionary ../data/en_tl.txt \
    -efsunDictionaryUnq ../data/en_tl_unq.txt \
    -src-lang tl \
    -tgt-lang en \
    -dim 300 ; } > ../word-embeddings/TL/tl_word_bitext_300_out.txt 2> ../word-embeddings/TL/tl_word_bitext_300_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/tl.stem.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_stem_bitext_300 \
    -efsunDictionary ../data/en_tl.txt \
    -efsunDictionaryUnq ../data/en_tl_unq.txt \
    -src-lang tl \
    -tgt-lang en \
    -dim 300 ; } > ../word-embeddings/TL/tl_stem_bitext_300_out.txt 2> ../word-embeddings/TL/tl_stem_bitext_300_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/tl.word.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_word_morph_3_bitext_300 \
    -efsunDictionary ../data/en_tl.txt \
    -efsunDictionaryUnq ../data/en_tl_unq.txt \
    -src-lang tl \
    -tgt-lang en \
    -dim 300 \
    -src_map_f ../segmentations/morph_map_final/tl.morph_3_map.tl \
    -tgt_map_f ../segmentations/morph_map_final/tl.morph_3_map.en \
    -use_morphology ; } > ../word-embeddings/TL/tl_word_morph_3_bitext_300_out.txt 2> ../word-embeddings/TL/tl_word_morph_3_bitext_300_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/tl.stem.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_stem_morph_3_bitext_300 \
    -efsunDictionary ../data/en_tl.txt \
    -efsunDictionaryUnq ../data/en_tl_unq.txt \
    -src-lang tl \
    -tgt-lang en \
    -dim 300 \
    -src_map_f ../segmentations/morph_map_final/tl.morph_3_map.tl \
    -tgt_map_f ../segmentations/morph_map_final/tl.morph_3_map.en \
    -use_morphology ; } > ../word-embeddings/TL/tl_stem_morph_3_bitext_300_out.txt 2> ../word-embeddings/TL/tl_stem_morph_3_bitext_300_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/tl.word.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_word_morph_3p_bitext_300 \
    -efsunDictionary ../data/en_tl.txt \
    -efsunDictionaryUnq ../data/en_tl_unq.txt \
    -src-lang tl \
    -tgt-lang en \
    -dim 300 \
    -src_map_f ../segmentations/morph_map_final/tl.morph_3p_map.tl \
    -tgt_map_f ../segmentations/morph_map_final/tl.morph_3p_map.en \
    -use_morphology ; } > ../word-embeddings/TL/tl_word_morph_3p_bitext_300_out.txt 2> ../word-embeddings/TL/tl_word_morph_3p_bitext_300_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/tl.stem.space.tl-en.align \
    -src-train ../segmentations/morph_map_final/tl.word.tl \
    -tgt-train ../segmentations/morph_map_final/tl.word.en \
    -output ../word-embeddings/TL/tl_stem_morph_3p_bitext_300 \
    -efsunDictionary ../data/en_tl.txt \
    -efsunDictionaryUnq ../data/en_tl_unq.txt \
    -src-lang tl \
    -tgt-lang en \
    -dim 300 \
    -src_map_f ../segmentations/morph_map_final/tl.morph_3p_map.tl \
    -tgt_map_f ../segmentations/morph_map_final/tl.morph_3p_map.en \
    -use_morphology ; } > ../word-embeddings/TL/tl_stem_morph_3p_bitext_300_out.txt 2> ../word-embeddings/TL/tl_stem_morph_3p_bitext_300_cerr.txt
set +x