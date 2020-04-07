/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <istream>
#include <ostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "constant.h"

namespace parfasttext {

class Args {

 public:
  Args();

  bool bilingual;
  bool multiskip;
  bool eval_new;
  std::string provided_translation_dictionary;
  std::string provided_translation_dictionary_unq;
  std::string output;
  std::string dictionary_fill;
  std::string src_bin_emb;
  std::string tgt_bin_emb;
  std::string src_fill_emb;
  std::string tgt_fill_emb;
  char* invoke_command;

  double lr;
  char actual_path[MAX_STRING];
  int lrUpdateRate;
  int dim;
  int ws;
  int epoch;
  int minCount;
  int minCountLabel;
  int neg;
  int wordNgrams;
  model_name model;
  int bucket;
  int minn;
  int maxn;
  int thread;
  double t;
  int verbose;

  std::string align;
  bool bilingual_debug;
  std::string src_train, src_lang;
  std::string tgt_train, tgt_lang;

  bool call_eval_mono;
  bool call_eval_cldc;
  bool call_eval_muse;
  bool call_eval_dict_mult;
  bool call_eval_dict_unq;
  bool save_bilingual_model;
  std::string src_map_f, src_map_f_all, src_map_f_bpe;
  std::string tgt_map_f, tgt_map_f_all, tgt_map_f_bpe;

  bool use_morphology;

  std::string src_emb;
  std::string tgt_emb;
  std::string cerr_logging_parse;
  std::string experiment_epoch;

  bool interactive;
  bool subword_lookup_load;

  void parseArgs(const std::vector<std::string>& args);
  void checkFileStatus(const std::string &input_arg, const std::string &input_str);
  void printHelp();
};
} // namespace parfasttext
