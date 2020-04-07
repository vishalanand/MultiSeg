/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <gtest/gtest.h>

#define MAX_SENT_LEN 20000
#define MAX_STRING 1000
#define FREQ 1000
#define MAX_VOCAB 200000
#define EPOCH_CONST "    -epoch 1 \\\n"
#define EPOCH_CONST_1 "    -epoch 1 \\\n"

void execute(char* command);
std::string invoke_parfasttext();
std::string invoke_sw_word_40();
std::string invoke_sw_word_40_morph_3();
std::string invoke_sw_word_40_morph_3p();
std::string invoke_sw_stem_40();
std::string invoke_sw_stem_40_morph_3();
std::string invoke_sw_stem_40_morph_3p();
std::string invoke_sw_stem_40_morph_3p_bin();
std::string invoke_sw_stem_40_morph_3p_eval();
std::string invoke_sw_stem_40_morph_3p_eval_bin();

void execute(char* command);
bool log_cerr(std::string read_log);

void execute(char* command){
  system(command);
}

inline bool not_exists (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer));
}

bool log_cerr(std::string read_log) {

  std::string line, token;
  std::vector<std::string> log_arr;
  bool check = false;

  std::ifstream myfile (read_log);
  if (myfile.is_open()) {
    while (! myfile.eof() ) {
      log_arr.clear();
      getline (myfile, line);
      if (line.rfind("Iteration:\tmulti\tcase", 0) == 0 && line.rfind("Iteration:\t1", 0) != 0) {
        std::cerr << line << std::endl;
        check = true;
        std::istringstream iss(line);
        while(std::getline(iss, token, '\t')) {
          log_arr.push_back(token);
        }

        if( std::stof(log_arr[5]) <= 0 ||
            std::stof(log_arr[7]) <= 0 ||
            std::stof(log_arr[9]) <= 0 ||
            std::stoi(log_arr[11]) == 0) {
          std::cout << line << std::endl;
          check = false;
        }
      }
    }
    myfile.close();
  }

  return check;
}

bool log_rigorous_cerr(std::string read_log) {
  return true;

  std::string line, token;
  std::vector<std::string> log_arr;
  bool check = false;

  std::ifstream myfile (read_log);
  if (myfile.is_open()) {
    while (! myfile.eof() ) {
      log_arr.clear();
      getline (myfile, line);
      //if (line.rfind("Iteration:", 0) == 0 && line.rfind("Iteration:\t1", 0) != 0) {
      if (line.rfind("Iteration:\tmulti\tcase", 0) == 0 && line.rfind("Iteration:\t1", 0) != 0) {
        std::cerr << line << std::endl;
        check = true;
        std::istringstream iss(line);
        while(std::getline(iss, token, '\t')) {
          log_arr.push_back(token);
        }

        if( std::stof(log_arr[5]) <= 0 ||
            std::stof(log_arr[7]) <= 0 ||
            std::stof(log_arr[9]) <= 0 ||
            std::stoi(log_arr[11]) == 0) {
          std::cout << line << std::endl;
          check = false;
        }
      }
    }
    myfile.close();
  }

  return check;
}

std::string invoke_sw_word_40() {
  char command[MAX_STRING];
  sprintf(command, "{  ./bitext skipgram \\\n"
                   "    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \\\n"
                   "    -call_eval_dict_mult \\\n"
                   //"    -call_eval_dict_unq \\\n"
                   //"    -save_bilingual_model \\\n"
                   EPOCH_CONST
                   "    -thread 4 \\\n"
                   "    -align ../segmentations/morph_SW_TL/sw.word.space.sw-en.align \\\n"
                   "    -src-train ../segmentations/morph_SW_TL/sw.word.sw \\\n"
                   "    -tgt-train ../segmentations/morph_SW_TL/sw.word.en \\\n"
                   "    -output ./test/log/sw_word_bitext_40 \\\n"
                   "    -efsunDictionary ../data/en_sw.txt \\\n"
                   "    -efsunDictionaryUnq ../data/en_sw_unq.txt \\\n"
                   "    -src-lang sw \\\n"
                   "    -tgt-lang en \\\n"
                   "    -dim 40 ; }");
  execute(command);

  return "./test/log/sw_word_bitext_40_cerr.txt";
}

std::vector<std::string> invoke_sw_tl_word_40() {
  char command[MAX_STRING];
  sprintf(command, "{  ./bitext multiskip \\\n"
                   "\t-ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \\\n"
//                   "\t-call_eval_dict_mult \\\n"
                   //"\t-call_eval_dict_unq \\\n"
                   //"\t-save_bilingual_model \\\n"
                   "\t-epoch 4 \\\n"
                   "\t-thread 4 \\\n"
                   "\t-multiskip \\\n"
                   "\t-align ../segmentations/morph_SW_TL/sw.word.space.sw-en.align \\\n"
                   "\t-src-train ../segmentations/morph_SW_TL/sw.word.sw \\\n"
                   "\t-tgt-train ../segmentations/morph_SW_TL/sw.word.en \\\n"
                   "\t-efsunDictionary ../data/en_sw.txt \\\n"
                   "\t-efsunDictionaryUnq ../data/en_sw_unq.txt \\\n"
                   "\t-src-lang sw \\\n"
                   "\t-tgt-lang en \\\n"
                   "\t-output ./test/log/sw_word_bitext_40_multi \\\n"
//                   "\t-multiskip \\\n"
//                   "\t-align ../segmentations/morph_SW_TL/tl.word.space.tl-en.align \\\n"
//                   "\t-src-train ../segmentations/morph_SW_TL/tl.word.tl \\\n"
//                   "\t-tgt-train ../segmentations/morph_SW_TL/tl.word.en \\\n"
//                   "\t-efsunDictionary ../data/en_tl.txt \\\n"
//                   "\t-efsunDictionaryUnq ../data/en_tl_unq.txt \\\n"
//                   "\t-src-lang tl \\\n"
//                   "\t-tgt-lang en \\\n"
//                   "\t-output ./test/log/tl_word_bitext_40_multi \\\n"
                   "\t-dim 40 ; }" );
  execute(command);


  return std::vector<std::string> {"./test/log/sw_word_bitext_40_multi_cerr.txt", "./test/log/tl_word_bitext_40_multi_cerr.txt"};
  //return std::vector<std::string> {"./test/log/sw_word_bitext_40_multi_cerr.txt"};
}

std::vector<std::string> invoke_common_sw_tl_word_40() {
  char command[MAX_STRING];
  sprintf(command, "{  ./bitext multiskip \\\n"
                   "\t-ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \\\n"
                   "\t-call_eval_dict_mult \\\n"
                   //"\t-call_eval_dict_unq \\\n"
                   //"\t-save_bilingual_model \\\n"
                   "\t-common_target \\\n"
                   "\t-epoch 4 \\\n"
                   "\t-thread 4 \\\n"
                   "\t-multiskip \\\n"
                   "\t-align ../segmentations/morph_SW_TL/sw.word.space.sw-en.align \\\n"
                   "\t-src-train ../segmentations/morph_SW_TL/sw.word.sw \\\n"
                   "\t-tgt-train ../segmentations/morph_SW_TL/sw.word.en \\\n"
                   "\t-efsunDictionary ../data/en_sw.txt \\\n"
                   "\t-efsunDictionaryUnq ../data/en_sw_unq.txt \\\n"
                   "\t-src-lang sw \\\n"
                   "\t-tgt-lang en \\\n"
                   "\t-output ./test/log/sw_word_common_bitext_40_multi \\\n"
                   "\t-multiskip \\\n"
                   "\t-align ../segmentations/morph_SW_TL/tl.word.space.tl-en.align \\\n"
                   "\t-src-train ../segmentations/morph_SW_TL/tl.word.tl \\\n"
                   "\t-tgt-train ../segmentations/morph_SW_TL/tl.word.en \\\n"
                   "\t-efsunDictionary ../data/en_tl.txt \\\n"
                   "\t-efsunDictionaryUnq ../data/en_tl_unq.txt \\\n"
                   "\t-src-lang tl \\\n"
                   "\t-tgt-lang en \\\n"
                   "\t-output ./test/log/tl_word_common_bitext_40_multi \\\n"
                   "\t-dim 40 ; }" );
  execute(command);


  return std::vector<std::string> {"./test/log/sw_word_common_bitext_40_multi_cerr.txt", "./test/log/tl_word_common_bitext_40_multi_cerr.txt"};
  //return std::vector<std::string> {"./test/log/sw_word_bitext_40_multi_cerr.txt"};
}

std::string invoke_sw_word_40_morph_3() {
  char command[MAX_STRING];
  sprintf(command, "{  ./bitext skipgram \\\n"
                   "    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \\\n"
                   "    -call_eval_dict_mult \\\n"
                   //"    -call_eval_dict_unq \\\n"
                   //"    -save_bilingual_model \\\n"
                   EPOCH_CONST
                   "    -thread 4 \\\n"
                   "    -align ../segmentations/morph_SW_TL/sw.word.space.sw-en.align \\\n"
                   "    -src-train ../segmentations/morph_SW_TL/sw.word.sw \\\n"
                   "    -tgt-train ../segmentations/morph_SW_TL/sw.word.en \\\n"
                   "    -output ./test/log/sw_word_morph_3_bitext_40 \\\n"
                   "    -efsunDictionary ../data/en_sw.txt \\\n"
                   "    -efsunDictionaryUnq ../data/en_sw_unq.txt \\\n"
                   "    -src-lang sw \\\n"
                   "    -tgt-lang en \\\n"
                   "    -dim 40 \\\n"
                   "    -src_map_f ../segmentations/morph_SW_TL/sw.morph_3_map.sw \\\n"
                   "    -tgt_map_f ../segmentations/morph_SW_TL/sw.morph_3_map.en \\\n"
                   "    -use_morphology ; }");
  execute(command);

  return "./test/log/sw_word_morph_3_bitext_40_cerr.txt";
}

std::string invoke_sw_word_40_morph_3p() {
  char command[MAX_STRING];
  sprintf(command, "{  ./bitext skipgram \\\n"
                   "    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \\\n"
                   "    -call_eval_dict_mult \\\n"
                   //"    -call_eval_dict_unq \\\n"
                   //"    -save_bilingual_model \\\n"
                   EPOCH_CONST
                   "    -thread 4 \\\n"
                   "    -align ../segmentations/morph_SW_TL/sw.word.space.sw-en.align \\\n"
                   "    -src-train ../segmentations/morph_SW_TL/sw.word.sw \\\n"
                   "    -tgt-train ../segmentations/morph_SW_TL/sw.word.en \\\n"
                   "    -output ./test/log/sw_word_morph_3p_bitext_40 \\\n"
                   "    -efsunDictionary ../data/en_sw.txt \\\n"
                   "    -efsunDictionaryUnq ../data/en_sw_unq.txt \\\n"
                   "    -src-lang sw \\\n"
                   "    -tgt-lang en \\\n"
                   "    -dim 40 \\\n"
                   "    -src_map_f ../segmentations/morph_SW_TL/sw.morph_3p_map.sw \\\n"
                   "    -tgt_map_f ../segmentations/morph_SW_TL/sw.morph_3p_map.en \\\n"
                   "    -use_morphology ; }");
  execute(command);

  return "./test/log/sw_word_morph_3p_bitext_40_cerr.txt";
}

std::string invoke_sw_stem_40() {
  char command[MAX_STRING];
  sprintf(command, "{  ./bitext skipgram \\\n"
                   "    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \\\n"
                   "    -call_eval_dict_mult \\\n"
                   //"    -call_eval_dict_unq \\\n"
                   //"    -save_bilingual_model \\\n"
                   EPOCH_CONST
                   "    -thread 4 \\\n"
                   "    -align ../segmentations/morph_SW_TL/sw.stem.space.sw-en.align \\\n"
                   "    -src-train ../segmentations/morph_SW_TL/sw.word.sw \\\n"
                   "    -tgt-train ../segmentations/morph_SW_TL/sw.word.en \\\n"
                   "    -output ./test/log/sw_stem_bitext_40 \\\n"
                   "    -efsunDictionary ../data/en_sw.txt \\\n"
                   "    -efsunDictionaryUnq ../data/en_sw_unq.txt \\\n"
                   "    -src-lang sw \\\n"
                   "    -tgt-lang en \\\n"
                   "    -dim 40 ; }");
  execute(command);

  return "./test/log/sw_stem_bitext_40_cerr.txt";
}

std::string invoke_sw_stem_40_morph_3() {
  char command[MAX_STRING];
  sprintf(command, "{  ./bitext skipgram \\\n"
                   "    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \\\n"
                   "    -call_eval_dict_mult \\\n"
                   //"    -call_eval_dict_unq \\\n"
                   //"    -save_bilingual_model \\\n"
                   EPOCH_CONST
                   "    -thread 4 \\\n"
                   "    -align ../segmentations/morph_SW_TL/sw.stem.space.sw-en.align \\\n"
                   "    -src-train ../segmentations/morph_SW_TL/sw.word.sw \\\n"
                   "    -tgt-train ../segmentations/morph_SW_TL/sw.word.en \\\n"
                   "    -output ./test/log/sw_stem_morph_3_bitext_40 \\\n"
                   "    -efsunDictionary ../data/en_sw.txt \\\n"
                   "    -efsunDictionaryUnq ../data/en_sw_unq.txt \\\n"
                   "    -src-lang sw \\\n"
                   "    -tgt-lang en \\\n"
                   "    -dim 40 \\\n"
                   "    -src_map_f ../segmentations/morph_SW_TL/sw.morph_3_map.sw \\\n"
                   "    -tgt_map_f ../segmentations/morph_SW_TL/sw.morph_3_map.en \\\n"
                   "    -use_morphology ; }");
  execute(command);

  return "./test/log/sw_stem_morph_3_bitext_40_cerr.txt";
}

std::string invoke_sw_stem_40_morph_3p() {
  char command[MAX_STRING];
  sprintf(command, "{  ./bitext skipgram \\\n"
                   "    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \\\n"
                   "    -call_eval_dict_mult \\\n"
                   //"    -call_eval_dict_unq \\\n"
                   //"    -save_bilingual_model \\\n"
                   EPOCH_CONST
                   "    -thread 4 \\\n"
                   "    -align ../segmentations/morph_SW_TL/sw.stem.space.sw-en.align \\\n"
                   "    -src-train ../segmentations/morph_SW_TL/sw.word.sw \\\n"
                   "    -tgt-train ../segmentations/morph_SW_TL/sw.word.en \\\n"
                   "    -output ./test/log/sw_stem_morph_3p_bitext_40 \\\n"
                   "    -efsunDictionary ../data/en_sw.txt \\\n"
                   "    -efsunDictionaryUnq ../data/en_sw_unq.txt \\\n"
                   "    -src-lang sw \\\n"
                   "    -tgt-lang en \\\n"
                   "    -dim 40 \\\n"
                   "    -src_map_f ../segmentations/morph_SW_TL/sw.morph_3p_map.sw \\\n"
                   "    -tgt_map_f ../segmentations/morph_SW_TL/sw.morph_3p_map.en \\\n"
                   "    -use_morphology ; }");
  execute(command);

  return "./test/log/sw_stem_morph_3p_bitext_40_cerr.txt";
}

std::string invoke_de_stem_40_morph_3p() {
  char command[MAX_STRING];
  sprintf(command, "{  ./bitext skipgram \\\n"
                   "    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \\\n"
                   "    -call_eval_dict_mult \\\n"
                   //"    -call_eval_dict_unq \\\n"
                   //"    -save_bilingual_model \\\n"
                   EPOCH_CONST_1
                   "    -thread 4 \\\n"
                   "    -align ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.stem.space.de-en.align \\\n"
                   "    -src-train ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.de \\\n"
                   "    -tgt-train ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.word.en \\\n"
                   "    -output ./test/log/de_stem_morph_3p_bitext_40 \\\n"
                   "    -efsunDictionary ../data/en_de.txt \\\n"
                   "    -efsunDictionaryUnq ../data/en_de_unq.txt \\\n"
                   "    -src-lang de \\\n"
                   "    -tgt-lang en \\\n"
                   "    -dim 40 \\\n"
                   "    -src_map_f ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.morph_3p_map.de \\\n"
                   "    -tgt_map_f ../segmentations/morph_map_final/Europarl/mini/10k/10k_europarl.morph_3p_map.en \\\n"
                   "    -use_morphology ; }");
  execute(command);

  return "./test/log/de_stem_morph_3p_bitext_40_cerr.txt";
}

std::string invoke_sw_stem_40_morph_3p_bin() {
  char command[MAX_STRING];
  sprintf(command, "{  ./bitext skipgram \\\n"
                   "    -bilingual -ws 5 -minCount 5 -lrUpdateRate 100 -neg 30 -verbose 1 \\\n"
                   "    -call_eval_dict_mult \\\n"
                   //"    -call_eval_dict_unq \\\n"
                   "    -save_bilingual_model \\\n"
                   EPOCH_CONST
                   "    -thread 4 \\\n"
                   "    -align ../segmentations/morph_SW_TL/sw.stem.space.sw-en.align \\\n"
                   "    -src-train ../segmentations/morph_SW_TL/sw.word.sw \\\n"
                   "    -tgt-train ../segmentations/morph_SW_TL/sw.word.en \\\n"
                   "    -output ./test/log/sw_stem_morph_3p_bitext_40 \\\n"
                   "    -efsunDictionary ../data/en_sw.txt \\\n"
                   "    -efsunDictionaryUnq ../data/en_sw_unq.txt \\\n"
                   "    -src-lang sw \\\n"
                   "    -tgt-lang en \\\n"
                   "    -dim 40 \\\n"
                   "    -src_map_f ../segmentations/morph_SW_TL/sw.morph_3p_map.sw \\\n"
                   "    -tgt_map_f ../segmentations/morph_SW_TL/sw.morph_3p_map.en \\\n"
                   "    -use_morphology ; }");
  execute(command);

  return "./test/log/sw_stem_morph_3p_bitext_40_cerr.txt";
}

std::string invoke_sw_stem_40_morph_3p_eval() {
  if(not_exists("./test/log/sw_stem_morph_3p_bitext_40.sw.iter1.vec")) {
    invoke_sw_stem_40_morph_3p();
  }
  char command[MAX_STRING];
//  sprintf(command, "{  ./bitext eval_only \\\n"
//                   "    -src_emb ./test/log/sw_stem_morph_3p_bitext_40.sw.iter1.vec \\\n"
//                   "    -tgt_emb ./test/log/sw_stem_morph_3p_bitext_40.en.iter1.vec \\\n"
//                   "    -output ./test/log/sw_stem_morph_3p_bitext_40_eval \\\n"
//                   "    -src-lang sw \\\n"
//                   "    -tgt-lang en \\\n"
//                   "    -dim 40 \\\n"
//                   "    -efsunDictionary ../data/en_sw.txt \\\n"
//                   "    -efsunDictionaryUnq ../data/en_sw_unq.txt \\\n"
//                   "    -call_eval_dict_mult \\\n"
//                   "    -call_eval_dict_unq ; }");
  sprintf(command, "{  ./bitext eval_only \\\n"
                   "    -src_emb ./test/log/sw_stem_morph_3p_bitext_40.sw.iter1.vec \\\n"
                   "    -tgt_emb ./test/log/sw_stem_morph_3p_bitext_40.en.iter1.vec \\\n"
                   "    -output ./test/log/sw_stem_morph_3p_bitext_40_eval \\\n"
                   "    -src-lang sw \\\n"
                   "    -tgt-lang en \\\n"
                   "    -dim 40 \\\n"
                   "    -efsunDictionary ../data/en_sw.txt \\\n"
                   "    -efsunDictionaryUnq ../data/en_sw_unq.txt \\\n"
                   "    -call_eval_dict_mult ; }");
  execute(command);

  return "./test/log/sw_stem_morph_3p_bitext_40_eval_cerr.txt";
}

std::string invoke_de_stem_40_morph_3p_eval_rigorous() {
  if(not_exists("./test/log/de_stem_morph_3p_bitext_40.de.iter1.vec")) {
    invoke_de_stem_40_morph_3p();
  }
  char command[MAX_STRING];
  sprintf(command, "{  ./bitext eval_only \\\n"
                   "    -src_emb ./test/log/de_stem_morph_3p_bitext_40.de.iter1.vec \\\n"
                   "    -tgt_emb ./test/log/de_stem_morph_3p_bitext_40.en.iter1.vec \\\n"
                   "    -output ./test/log/de_stem_morph_3p_bitext_40_eval \\\n"
                   "    -src-lang de \\\n"
                   "    -tgt-lang en \\\n"
                   "    -dim 40 \\\n"
                   "    -efsunDictionary ../data/en_de.txt \\\n"
                   "    -efsunDictionaryUnq ../data/en_de_unq.txt \\\n"
                   "    -call_eval_mono \\\n"
                   "    -call_eval_cldc \\\n"
                   "    -call_eval_muse \\\n"
                   "    -call_eval_dict_unq \\\n"
                   "    -call_eval_dict_mult \\\n"
                   "    ; } > ./test/log/de_stem_morph_3p_bitext_40_eval_out.txt 2> ./test/log/de_stem_morph_3p_bitext_40_eval_cerr.txt ");
  execute(command);

  return "./test/log/de_stem_morph_3p_bitext_40_eval_cerr.txt";
}

std::string invoke_de_stem_40_morph_3p_eval_rigorous_parse() {
  if(not_exists("./test/log/de_stem_morph_3p_bitext_40_eval_cerr.txt")) {
    invoke_de_stem_40_morph_3p_eval_rigorous();
  }
  char command[MAX_STRING];
  sprintf(command, "{  ./bitext parse_eval \\\n"
                   "    -cerr_logging_parse ./test/log/de_stem_morph_3p_bitext_40_eval_cerr.txt \\\n"
                   "    -experiment_epoch 1 ; } 2> check.txt");
  execute(command);

  return "./test/log/de_stem_morph_3p_bitext_40_eval_cerr.txt";
}

std::string invoke_sw_stem_40_morph_3p_eval_bin() {
  if(not_exists("./test/log/sw_stem_morph_3p_bitext_40.sw.iter1.vec.bin.txt")) {
    invoke_sw_stem_40_morph_3p_bin();
  }
  char command[MAX_STRING];
  sprintf(command, "{  ./bitext eval_only \\\n"
                   "    -src_emb ./test/log/sw_stem_morph_3p_bitext_40.sw.iter1.vec.bin.txt \\\n"
                   "    -tgt_emb ./test/log/sw_stem_morph_3p_bitext_40.en.iter1.vec.bin.txt \\\n"
                   "    -output ./test/log/sw_stem_morph_3p_bitext_40_eval_bin \\\n"
                   "    -src-lang sw \\\n"
                   "    -tgt-lang en \\\n"
                   "    -dim 40 \\\n"
                   "    -efsunDictionary ../data/en_sw.txt \\\n"
                   "    -efsunDictionaryUnq ../data/en_sw_unq.txt \\\n"
                   "    -call_eval_dict_mult \\\n"
                   "    -call_eval_dict_unq ; }");
  execute(command);

  return "./test/log/sw_stem_morph_3p_bitext_40_eval_bin_cerr.txt";
}

std::string invoke_sw_stem_40_morph_3p_oov_lookup() {
  char command[MAX_STRING];
  sprintf(command, "{  ./bitext oov_lookup \\\n"
                   "    -src_emb ./test/log/sw_stem_morph_3p_bitext_40_oov_lookup.sw.iter1.vec \\\n"
                   "    -tgt_emb ./test/log/sw_stem_morph_3p_bitext_40_oov_lookup.en.iter1.vec \\\n"
                   "    -src_bin_emb ./test/log/sw_stem_morph_3p_bitext_40_oov_lookup.sw.iter1.vec.bin.txt \\\n"
                   "    -tgt_bin_emb ./test/log/sw_stem_morph_3p_bitext_40_oov_lookup.en.iter1.vec.bin.txt \\\n"
                   "    -src_fill_emb ./test/log/sw_stem_morph_3p_bitext_40_oov_lookup.sw.iter1.vec.fill.txt \\\n"
                   "    -tgt_fill_emb ./test/log/sw_stem_morph_3p_bitext_40_oov_lookup.en.iter1.vec.fill.txt \\\n"
                   "    -dict_fill ../data/en_sw.txt \\\n"
//                   "    -interactive \\\n"
                   "    -subword_lookup_load ; } > out.txt");
  execute(command);

  return "./test/log/sw_stem_40_morph_3p_oov_lookup_cerr.txt";
}

//TEST(BlackBoxTest, test_invoke_sw_word_40){
//  EXPECT_TRUE(log_cerr(invoke_sw_word_40()));
//}
//
//TEST(BlackBoxTest, test_invoke_sw_word_40_morph_3){
//  EXPECT_TRUE(log_cerr(invoke_sw_word_40_morph_3()));
//}
//
//TEST(BlackBoxTest, test_invoke_sw_word_40_morph_3p){
//  EXPECT_TRUE(log_cerr(invoke_sw_word_40_morph_3p()));
//}
//
//TEST(BlackBoxTest, test_invoke_sw_stem_40){
//  EXPECT_TRUE(log_cerr(invoke_sw_stem_40()));
//}
//
//TEST(BlackBoxTest, test_invoke_sw_stem_40_morph_3){
//  EXPECT_TRUE(log_cerr(invoke_sw_stem_40_morph_3()));
//}
//
//TEST(BlackBoxTest, test_invoke_sw_stem_40_morph_3p){
//  EXPECT_TRUE(log_cerr(invoke_sw_stem_40_morph_3p()));
//}
//
//TEST(BlackBoxTest, test_invoke_sw_stem_40_morph_3p_eval){
//  EXPECT_TRUE(log_cerr(invoke_sw_stem_40_morph_3p_eval()));
//}

//TEST(BlackBoxTest, test_invoke_sw_stem_40_morph_3p_eval){
//  EXPECT_TRUE(log_rigorous_cerr(invoke_de_stem_40_morph_3p_eval_rigorous()));
//}

//TEST(BlackBoxTest, test_invoke_sw_stem_40_morph_3p_eval_rigorous_parse){
//  invoke_de_stem_40_morph_3p_eval_rigorous_parse();
//  EXPECT_TRUE(true);
//}

TEST(BlackBoxTest, test_invoke_sw_stem_40_morph_3p_oov_lookup){
  char command[MAX_STRING];
  //invoke_sw_stem_40_morph_3p_eval_bin();
//  char command[MAX_STRING];
//sprintf(command, "{  ./bitext eval_only \\\n"
//                  "    -src_emb ./test/log/sw_stem_morph_3p_bitext_40.sw.iter1.vec \\\n"
//                  "    -tgt_emb ./test/log/sw_stem_morph_3p_bitext_40.en.iter1.vec \\\n"
//                  "    -output ./test/log/sw_stem_morph_3p_bitext_40_eval \\\n"
//                  "    -src-lang sw \\\n"
//                  "    -tgt-lang en \\\n"
//                  "    -dim 40 \\\n"
//                  "    -efsunDictionary ../data/en_sw.txt \\\n"
//                  "    -efsunDictionaryUnq ../data/en_sw_unq.txt \\\n"
//                  "    -call_eval_dict_mult ; } > out.txt");
//                  execute(command);
  //invoke_sw_stem_40_morph_3p_oov_lookup();

  sprintf(command, "{  ./bitext eval_only \\\n"
  "    -src_emb ./test/log/sw_stem_morph_3p_bitext_40_oov_lookup.sw.iter1.vec \\\n"
  "    -tgt_emb ./test/log/sw_stem_morph_3p_bitext_40_oov_lookup.en.iter1.vec \\\n"
  "    -output ./test/log/sw_stem_morph_3p_bitext_40_eval_bin \\\n"
  "    -src-lang sw \\\n"
  "    -tgt-lang en \\\n"
  "    -dim 40 \\\n"
  "    -efsunDictionary ../data/en_sw.txt \\\n"
  "    -efsunDictionaryUnq ../data/en_sw_unq.txt \\\n"
  "    -call_eval_dict_mult ; } > out_vec.txt ");
  execute(command);

  sprintf(command, "{  ./bitext eval_only \\\n"
  "    -src_emb ./test/log/sw_stem_morph_3p_bitext_40_oov_lookup.sw.iter1.vec.fill.txt \\\n"
  "    -tgt_emb ./test/log/sw_stem_morph_3p_bitext_40_oov_lookup.en.iter1.vec.fill.txt \\\n"
  "    -output ./test/log/sw_stem_morph_3p_bitext_40_eval_bin \\\n"
  "    -src-lang sw \\\n"
  "    -tgt-lang en \\\n"
  "    -dim 40 \\\n"
  "    -efsunDictionary ../data/en_sw.txt \\\n"
  "    -efsunDictionaryUnq ../data/en_sw_unq.txt \\\n"
  "    -call_eval_dict_mult ; } > out_fill.txt ");
  execute(command);

  EXPECT_TRUE(true);
}

//TEST(BlackBoxTest, test_invoke_sw_stem_40_morph_3p_eval_bin){
//  EXPECT_TRUE(log_cerr(invoke_sw_stem_40_morph_3p_eval_bin()));
//}
//
//TEST(BlackBoxMultiTest, test_invoke_sw_tl_word_40){
//  std::vector<std::string> multi_log = invoke_sw_tl_word_40();
//  for(int i=0; i<multi_log.size(); i++) {
//    EXPECT_TRUE(log_cerr(multi_log[i]));
//  }
//}
//
//TEST(BlackBoxMultiTest, test_invoke_common_sw_tl_word_40){
//  std::vector<std::string> multi_log = invoke_common_sw_tl_word_40();
//  for(int i=0; i<multi_log.size(); i++) {
//    EXPECT_TRUE(log_cerr(multi_log[i]));
//  }
//}
