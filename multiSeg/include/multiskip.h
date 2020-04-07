/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <istream>
#include <ostream>
#include <vector>
#include <unordered_map>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "constant.h"
#include "biskip.h"
#include "language_model.h"

namespace parfasttext {

class MultiskipArgs {

public:
  MultiskipArgs();
  std::vector<LanguageModel *> languageModels;
  bool multiskip;
  bool eval_new;

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
  int model_count;
  bool bilingual_debug;

  bool call_eval_mono;
  bool call_eval_cldc;
  bool call_eval_muse;
  bool call_eval_dict_mult;
  bool call_eval_dict_unq;
  bool save_bilingual_model;

  bool use_morphology;
  char* invoke_command;

  bool shared_tgt_model;

  void parseArgs(const std::vector<std::string>& args);
  void checkFileStatus(const std::string &input_arg, const std::string &input_str);
  void printHelp();
  std::vector<BiskipArgs> biskip_models;
};
}
