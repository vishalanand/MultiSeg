/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "multiskip.h"

namespace parfasttext {

MultiskipArgs::MultiskipArgs() {
  lr = 0.05;
  dim = 100;
  ws = 5;
  epoch = 5;
  minCount = 5;
  minCountLabel = 0;
  neg = 5;
  wordNgrams = 1;
  model = model_name::sg;
  bucket = 2000000;
  minn = 3;
  maxn = 6;
  thread = 12;
  lrUpdateRate = 100;
  t = 1e-4;
  verbose = 2;
  bilingual_debug = false;
  multiskip = false;
  eval_new = false;
  use_morphology = false;

  call_eval_mono = false;
  call_eval_cldc = false;
  call_eval_muse = false;
  call_eval_dict_mult = false;
  call_eval_dict_unq = false;
  save_bilingual_model = false;
  model_count = 0;

  std::vector<BiskipArgs> biskip_models;
  shared_tgt_model = false;
}

void MultiskipArgs::checkFileStatus(const std::string &input_arg, const std::string &input_str) {
  std::ifstream ifs(input_str.c_str());
  if (!ifs.is_open()) {
    throw std::invalid_argument("Missing file for: \"" + input_arg + "\":\t" + input_str);
  }

  ifs.close();
}

void MultiskipArgs::parseArgs(const std::vector<std::string>& args) {
  std::string command(args[1]);
  if(command == "multiskip") {
    for (int ai = 2; ai < args.size(); ai += 2) {
      if (args[ai][0] != '-') {
        std::cerr << "Provided argument without a dash! Usage: MULT" << std::endl;
        exit(EXIT_FAILURE);
      }
      try {
        if (args[ai] == "-multiskip") {
          model_count++;
          multiskip = true;
          ai--;

          BiskipArgs biskipArgs;
          biskip_models.push_back(biskipArgs);
          biskip_models[biskip_models.size() - 1].bilingual = true;

          for (ai = ai + 2; ai < args.size(); ai += 2) {
            if (args[ai][0] != '-') {
              std::cerr << "Provided argument without a dash! Usage:" << std::endl;
              exit(EXIT_FAILURE);
            }
            if (args[ai] == "-src_map_f") {
              biskip_models[biskip_models.size() - 1].src_map_f = std::string(args.at(ai + 1));
              realpath(biskip_models[biskip_models.size() - 1].src_map_f.c_str(), actual_path);
              biskip_models[biskip_models.size() - 1].src_map_f = actual_path;
              checkFileStatus(args[ai], actual_path);
            } else if (args[ai] == "-tgt_map_f") {
              biskip_models[biskip_models.size() - 1].tgt_map_f = std::string(args.at(ai + 1));
              realpath(biskip_models[biskip_models.size() - 1].tgt_map_f.c_str(), actual_path);
              biskip_models[biskip_models.size() - 1].tgt_map_f = actual_path;
              checkFileStatus(args[ai], actual_path);
            } else if (args[ai] == "-src-train") {
              biskip_models[biskip_models.size() - 1].src_train = std::string(args.at(ai + 1));
              realpath(biskip_models[biskip_models.size() - 1].src_train.c_str(), actual_path);
              biskip_models[biskip_models.size() - 1].src_train = actual_path;
              checkFileStatus(args[ai], actual_path);
            } else if (args[ai] == "-tgt-train") {
              biskip_models[biskip_models.size() - 1].tgt_train = std::string(args.at(ai + 1));
              realpath(biskip_models[biskip_models.size() - 1].tgt_train.c_str(), actual_path);
              biskip_models[biskip_models.size() - 1].tgt_train = actual_path;
              checkFileStatus(args[ai], actual_path);
            } else if (args[ai] == "-src-lang") {
              biskip_models[biskip_models.size() - 1].src_lang = std::string(args.at(ai + 1));
            } else if (args[ai] == "-tgt-lang") {
              biskip_models[biskip_models.size() - 1].tgt_lang = std::string(args.at(ai + 1));
            } else if (args[ai] == "-align") {
              biskip_models[biskip_models.size() - 1].align = std::string(args.at(ai + 1));
              realpath(biskip_models[biskip_models.size() - 1].align.c_str(), actual_path);
              biskip_models[biskip_models.size() - 1].align = actual_path;
              checkFileStatus(args[ai], actual_path);
            } else if (args[ai] == "-efsunDictionary") {
              biskip_models[biskip_models.size() - 1].provided_translation_dictionary = std::string(args.at(ai + 1));
              realpath(biskip_models[biskip_models.size() - 1].provided_translation_dictionary.c_str(), actual_path);
              biskip_models[biskip_models.size() - 1].provided_translation_dictionary = actual_path;
              checkFileStatus(args[ai], actual_path);
            } else if (args[ai] == "-output") {
              biskip_models[biskip_models.size() - 1].output = std::string(args.at(ai + 1));
              realpath(biskip_models[biskip_models.size() - 1].output.c_str(), actual_path);
              biskip_models[biskip_models.size() - 1].output= actual_path;
            } else if (args[ai] == "-efsunDictionaryUnq") {
              biskip_models[biskip_models.size() - 1].provided_translation_dictionary_unq = std::string(args.at(ai + 1));
              realpath(biskip_models[biskip_models.size() - 1].provided_translation_dictionary_unq.c_str(), actual_path);
              biskip_models[biskip_models.size() - 1].provided_translation_dictionary_unq = actual_path;
              checkFileStatus(args[ai], actual_path);
            } else {
              ai = ai - 2;
              break;
            }
          }
          if(biskip_models[biskip_models.size() - 1].tgt_lang != "en") {
            std::cerr << "Target should be english for multiskip variation" << std::endl;
            exit(EXIT_FAILURE);
          }
        } else if (args[ai] == "-dim") {
          dim = std::stoi(args.at(ai + 1));
        } else if (args[ai] == "-common_target") {
          shared_tgt_model = true;
          ai--;
        } else if (args[ai] == "-call_eval_mono") {
          call_eval_mono = true;
          ai--;
        } else if (args[ai] == "-call_eval_cldc") {
          call_eval_cldc = true;
          ai--;
        } else if (args[ai] == "-call_eval_muse") {
          call_eval_muse = true;
          ai--;
        } else if (args[ai] == "-call_eval_dict_mult") {
          call_eval_dict_mult = true;
          ai--;
        } else if (args[ai] == "-call_eval_dict_unq") {
          call_eval_dict_unq = true;
          ai--;
        } else if (args[ai] == "-lr") {
          lr = std::stof(args.at(ai + 1));
        } else if (args[ai] == "-lrUpdateRate") {
          lrUpdateRate = std::stoi(args.at(ai + 1));
        } else if (args[ai] == "-dim") {
          dim = std::stoi(args.at(ai + 1));
        } else if (args[ai] == "-ws") {
          ws = std::stoi(args.at(ai + 1));
        } else if (args[ai] == "-epoch") {
          epoch = std::stoi(args.at(ai + 1));
        } else if (args[ai] == "-minCount") {
          minCount = std::stoi(args.at(ai + 1));
        } else if (args[ai] == "-neg") {
          neg = std::stoi(args.at(ai + 1));
        } else if (args[ai] == "-bucket") {
          bucket = std::stoi(args.at(ai + 1));
        } else if (args[ai] == "-minn") {
          minn = std::stoi(args.at(ai + 1));
        } else if (args[ai] == "-maxn") {
          maxn = std::stoi(args.at(ai + 1));
        } else if (args[ai] == "-thread") {
          thread = std::stoi(args.at(ai + 1));
        } else if (args[ai] == "-t") {
          t = std::stof(args.at(ai + 1));
        } else if (args[ai] == "-verbose") {
          verbose = std::stoi(args.at(ai + 1));
        } else if (args[ai] == "-eval_new") {
          eval_new = true;
          ai--;
        } else if (args[ai] == "-bilingual_debug") {
          bilingual_debug = true;
          ai--;
        } else if (args[ai] == "-use_morphology") {
          use_morphology = true;
          ai--;
        } else if (args[ai] == "-call_eval_mono") {
          call_eval_mono = true;
          ai--;
        } else if (args[ai] == "-call_eval_cldc") {
          call_eval_cldc = true;
          ai--;
        } else if (args[ai] == "-call_eval_muse") {
          call_eval_muse = true;
          ai--;
        } else if (args[ai] == "-call_eval_dict_mult") {
          call_eval_dict_mult = true;
          ai--;
        } else if (args[ai] == "-call_eval_dict_unq") {
          call_eval_dict_unq = true;
          ai--;
        } else if (args[ai] == "-save_bilingual_model") {
          save_bilingual_model = true;
          ai--;
        } else {
          std::cerr << "Unknown argument: " << args[ai] << std::endl;
          printHelp();
          exit(EXIT_FAILURE);
        }
      } catch (const std::out_of_range &e) {
        std::cerr << args[ai] << " is missing an argument" << std::endl;
        printHelp();
        exit(EXIT_FAILURE);
      } catch (const std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
        printHelp();
        exit(EXIT_FAILURE);
      }
    }
  }

  if (wordNgrams <= 1 && maxn == 0) {
    bucket = 0;
  }
}

void MultiskipArgs::printHelp() {
  std::cout << "Incorrect command" << std::endl;
}

} // namespace parfasttext
