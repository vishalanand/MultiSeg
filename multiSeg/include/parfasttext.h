/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <time.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <queue>
#include <set>
#include <tuple>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unistd.h>
#include <limits.h>

#include "args.h"
#include "dictionary.h"
#include "evalparse.h"
#include "matrix.h"
#include "model.h"
#include "constant.h"
#include "utils.h"
#include "vector.h"
#include "multiskip.h"
#include "language_model.h"

namespace parfasttext {

class ParFastText {
 protected:

  LanguageModel *languageModel;
  LanguageModel l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12;

  char stdout_char[MAX_STRING], stderr_char[MAX_STRING];
  std::atomic_int completed_model;
  std::shared_ptr<int> FREQ_MASTER;
  std::shared_ptr<int> FREQ_THREAD;
  bool shared_model;

  void startBilingualThreads();
  void invoke_call_eval(
    std::string srcFile,
    std::string tgtFile,
    std::string word_binary_type);

  void addInputVector(
    Vector& vec,
    int32_t ind,
    std::shared_ptr<Matrix> &input_p_) const;

  void bilingualTrainThread(int32_t);
  void eval_mono(
    char* emb_file,
    char* lang);

  void cldc(
    char* filename_src,
    char* filename_tgt,
    char* src_lang_char,
    char* tgt_lang_char);

  void printInfo(
    real,
    real,
    std::ostream&);

  void ComputeBlockStartPoints(
    char* file_name,
    int num_blocks,
    long long **blocks,
    long long *num_lines,
    int **line_parts);

  void validity(
    std::string src_file,
    std::string tgt_file,
    std::string align_file);

 public:
  ParFastText();

  void set_std_output_cerr(const std::string std_output);
  void set_std_output_cerr_write(const std::string std_output);

  void getWordVector(
        Vector& vec,
        const std::string& word,
        std::shared_ptr<Dictionary> &dict_p,
        std::shared_ptr<Matrix> &input_p_) const;

  void museEval(
        char* filename_src,
        char* filename_tgt,
        char* src_lang_char,
        char* tgt_lang_char,
        int dimension);
  void loadModel(const std::string& filename_src, const std::string& filename_tgt, LanguageModel &lm_oov);
  //void getWordVector(Vector& vec, const std::string& word, std::shared_ptr<Dictionary> &dict_p_oov, std::shared_ptr<Matrix> &input_p) const;
  bool checkModel(std::ifstream &filename);
  void oov_lookup(
    char* filename_src_char,
    char* filename_tgt_char,
    char* filename_src_bin_char,
    char* filename_tgt_bin_char,
    char* filename_src_fill_char,
    char* filename_tgt_fill_char,
    bool interactive = false,
    bool subword_lookup_load = false,
    char* dictionary_fill = nullptr);

  void call_parse_eval(char* cerr_logging_parse, char* experiment_epoch);

  void execute(char* command);

  void saveBilingualVectors(
    const std::string& filename_src,
    const std::string& filename_tgt);

  void dictionary_word_translate(
        char* filename_src,
        char* filename_tgt,
        char* src_lang_char,
        char* tgt_lang_char,
        char* provided_translation_dictionary_char);

  void call_eval(
          char* filename_src_char,
          char* filename_tgt_char,
          char* src_lang_char,
          char* tgt_lang_char,
          char* provided_translation_dictionary_char,
          char* provided_translation_dictionary_unq_char,
          int dimension,
          bool call_eval_mono,
          bool call_eval_cldc,
          bool call_eval_muse,
          bool call_eval_dict_mult,
          bool call_eval_dict_unq);

  void saveBilingualModel(
    const std::string& filename_src,
    const std::string& filename_tgt);

  std::vector<std::pair<std::string, Vector>> getNgramVectorsNew(
                                                const std::string& word,
                                                std::shared_ptr<Args> &args_p,
                                                std::shared_ptr<Dictionary> &dict_p,
                                                std::shared_ptr<Matrix> &input_p) const;
  bool getNgramVectorsNewConstruct(
          const std::string& word,
          const std::shared_ptr<Args> &args_p,
          const std::shared_ptr<Dictionary> &dict_p,
          const std::shared_ptr<Matrix> &input_p,
          Vector &vec) const;

  void bilingual_train(const Args& args);
  void bilingual_train_all(const Args& args);

  void multi_bilingual_train(MultiskipArgs& args);

  void insert_model(MultiskipArgs& multiskipargs);

  void construct_from_multiskip(
        const MultiskipArgs& multiskipargs,
        int index,
        Args &args_var);

  void call_pair_train(
          Model& model_from,
          Model& model_to,
          real lr_from,
          real lr_to,
          const std::vector<int32_t>& line_from,
          const std::vector<int32_t>& new_parallel_line_to,
          std::shared_ptr<Dictionary> &dict_p_from_,
          std::shared_ptr<Dictionary> &dict_p_to_,
          std::unordered_map< int, std::vector<int> > &align_pairs_wid);

  void bilingualSkipgram(
        Model& model,
        real lr,
        const std::vector<int32_t>& line,
        std::shared_ptr<Dictionary> &dict_p_);

};
} // namespace parfasttext