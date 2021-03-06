set -x
#parFastText 40
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.space.de-en.align \
    -src-train ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.de \
    -tgt-train ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.en \
    -output ../word-embeddings/10k_europarl/10k_de_word_bitext_40 \
    -efsunDictionary ../data/en_de.txt \
    -efsunDictionaryUnq ../data/en_de_unq.txt \
    -src-lang de \
    -tgt-lang en \
    -dim 40 ; } > ../word-embeddings/10k_de_word_bitext_40_out.txt 2> ../word-embeddings/10k_de_word_bitext_40_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.stem.space.de-en.align \
    -src-train ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.de \
    -tgt-train ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.en \
    -output ../word-embeddings/10k_europarl/10k_de_stem_bitext_40 \
    -efsunDictionary ../data/en_de.txt \
    -efsunDictionaryUnq ../data/en_de_unq.txt \
    -src-lang de \
    -tgt-lang en \
    -dim 40 ; } > ../word-embeddings/10k_de_stem_bitext_40_out.txt 2> ../word-embeddings/10k_de_stem_bitext_40_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.space.de-en.align \
    -src-train ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.de \
    -tgt-train ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.en \
    -output ../word-embeddings/10k_europarl/10k_de_word_morph_3_bitext_40 \
    -efsunDictionary ../data/en_de.txt \
    -efsunDictionaryUnq ../data/en_de_unq.txt \
    -src-lang de \
    -tgt-lang en \
    -dim 40 \
    -src_map_f ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.morph_3_map.de \
    -tgt_map_f ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.morph_3_map.en \
    -use_morphology ; } > ../word-embeddings/10k_de_word_morph_3_bitext_40_out.txt 2> ../word-embeddings/10k_de_word_morph_3_bitext_40_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.stem.space.de-en.align \
    -src-train ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.de \
    -tgt-train ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.en \
    -output ../word-embeddings/10k_europarl/10k_de_stem_morph_3_bitext_40 \
    -efsunDictionary ../data/en_de.txt \
    -efsunDictionaryUnq ../data/en_de_unq.txt \
    -src-lang de \
    -tgt-lang en \
    -dim 40 \
    -src_map_f ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.morph_3_map.de \
    -tgt_map_f ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.morph_3_map.en \
    -use_morphology ; } > ../word-embeddings/10k_de_stem_morph_3_bitext_40_out.txt 2> ../word-embeddings/10k_de_stem_morph_3_bitext_40_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.space.de-en.align \
    -src-train ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.de \
    -tgt-train ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.en \
    -output ../word-embeddings/10k_europarl/10k_de_word_morph_3p_bitext_40 \
    -efsunDictionary ../data/en_de.txt \
    -efsunDictionaryUnq ../data/en_de_unq.txt \
    -src-lang de \
    -tgt-lang en \
    -dim 40 \
    -src_map_f ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.morph_3p_map.de \
    -tgt_map_f ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.morph_3p_map.en \
    -use_morphology ; } > ../word-embeddings/10k_de_word_morph_3p_bitext_40_out.txt 2> ../word-embeddings/10k_de_word_morph_3p_bitext_40_cerr.txt
{ make && time ./bitext skipgram \
    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \
    -call_eval_dict_mult \
    -call_eval_dict_unq \
    -save_bilingual_model \
    -epoch 30 \
    -thread 12 \
    -align ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.stem.space.de-en.align \
    -src-train ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.de \
    -tgt-train ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.en \
    -output ../word-embeddings/10k_europarl/10k_de_stem_morph_3p_bitext_40 \
    -efsunDictionary ../data/en_de.txt \
    -efsunDictionaryUnq ../data/en_de_unq.txt \
    -src-lang de \
    -tgt-lang en \
    -dim 40 \
    -src_map_f ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.morph_3p_map.de \
    -tgt_map_f ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.morph_3p_map.en \
    -use_morphology ; } > ../word-embeddings/10k_de_stem_morph_3p_bitext_40_out.txt 2> ../word-embeddings/10k_de_stem_morph_3p_bitext_40_cerr.txt
set +x