/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "args.h"

namespace parfasttext {

Args::Args() {
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
  bilingual = false;
  multiskip = false;
  eval_new = false;
  provided_translation_dictionary = "";
  provided_translation_dictionary_unq = "";
  use_morphology = false;

  call_eval_mono = false;
  call_eval_cldc = false;
  call_eval_muse = false;
  call_eval_dict_mult = false;
  call_eval_dict_unq = false;
  save_bilingual_model = false;

  src_emb = "";
  tgt_emb = "";

  interactive = false;
  subword_lookup_load = false;
}

void Args::checkFileStatus(const std::string &input_arg, const std::string &input_str) {
  std::ifstream ifs(input_str.c_str());
  if (!ifs.is_open()) {
    throw std::invalid_argument("Missing file for: \"" + input_arg + "\":\t" + input_str);
  }

  ifs.close();
}

void Args::parseArgs(const std::vector<std::string>& args) {
  std::string command(args[1]);
  if(command == "eval_only") {
    for (int ai = 2; ai < args.size(); ai += 2) {
      if (args[ai][0] != '-') {
        std::cerr << "Provided argument without a dash! Usage:" << std::endl;
        exit(EXIT_FAILURE);
      }
      try {
        if (args[ai] == "-src_emb") {
          src_emb = std::string(args.at(ai + 1));
          realpath(src_emb.c_str(), actual_path);
          src_emb = actual_path;
          checkFileStatus(args[ai], actual_path);
        } else if (args[ai] == "-tgt_emb") {
          tgt_emb = std::string(args.at(ai + 1));
          realpath(tgt_emb.c_str(), actual_path);
          tgt_emb = actual_path;
          checkFileStatus(args[ai], actual_path);
        } else if (args[ai] == "-src-lang") {
          src_lang = std::string(args.at(ai + 1));
        } else if (args[ai] == "-tgt-lang") {
          tgt_lang = std::string(args.at(ai + 1));
        } else if (args[ai] == "-output") {
          output = std::string(args.at(ai + 1));
          realpath(output.c_str(), actual_path);
          output = actual_path;
        } else if (args[ai] == "-efsunDictionary") {
          provided_translation_dictionary = std::string(args.at(ai + 1));
          realpath(provided_translation_dictionary.c_str(), actual_path);
          provided_translation_dictionary = actual_path;
          checkFileStatus(args[ai], actual_path);
        } else if (args[ai] == "-efsunDictionaryUnq") {
          provided_translation_dictionary_unq = std::string(args.at(ai + 1));
          realpath(provided_translation_dictionary_unq.c_str(), actual_path);
          provided_translation_dictionary_unq = actual_path;
          checkFileStatus(args[ai], actual_path);
        } else if (args[ai] == "-dim") {
          dim = std::stoi(args.at(ai + 1));
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
    return;
  }
  else if(command == "oov_lookup") {
    for (int ai = 2; ai < args.size(); ai += 2) {
      if (args[ai][0] != '-') {
        std::cerr << "Provided argument without a dash! Usage:" << std::endl;
        exit(EXIT_FAILURE);
      }
      try {
        if (args[ai] == "-src_emb") {
          src_emb = std::string(args.at(ai + 1));
          realpath(src_emb.c_str(), actual_path);
          src_emb = actual_path;
          checkFileStatus(args[ai], actual_path);
        } else if (args[ai] == "-tgt_emb") {
          tgt_emb = std::string(args.at(ai + 1));
          realpath(tgt_emb.c_str(), actual_path);
          tgt_emb = actual_path;
          checkFileStatus(args[ai], actual_path);
        } else if (args[ai] == "-output") {
          output = std::string(args.at(ai + 1));
          realpath(output.c_str(), actual_path);
          output = actual_path;
        } else if (args[ai] == "-interactive") {
          interactive = true;
          ai--;
        } else if (args[ai] == "-subword_lookup_load") {
          subword_lookup_load = true;
          ai--;
        } else if (args[ai] == "-src_bin_emb") {
          src_bin_emb = std::string(args.at(ai + 1));
          realpath(src_bin_emb.c_str(), actual_path);
          src_bin_emb = actual_path;
        } else if (args[ai] == "-tgt_bin_emb") {
          tgt_bin_emb = std::string(args.at(ai + 1));
          realpath(tgt_bin_emb.c_str(), actual_path);
          tgt_bin_emb = actual_path;
        } else if (args[ai] == "-src_fill_emb") {
          src_fill_emb = std::string(args.at(ai + 1));
          realpath(src_fill_emb.c_str(), actual_path);
          src_fill_emb = actual_path;
        } else if (args[ai] == "-tgt_fill_emb") {
          tgt_fill_emb = std::string(args.at(ai + 1));
          realpath(tgt_fill_emb.c_str(), actual_path);
          tgt_fill_emb = actual_path;
        } else if (args[ai] == "-dict_fill") {
          dictionary_fill = std::string(args.at(ai + 1));
          realpath(dictionary_fill.c_str(), actual_path);
          dictionary_fill = actual_path;
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
    return;
  }
  else if(command == "parse_eval") {
    for (int ai = 2; ai < args.size(); ai += 2) {
      if (args[ai][0] != '-') {
        std::cerr << "Provided argument without a dash! Usage:" << std::endl;
        exit(EXIT_FAILURE);
      }
      try {
        if (args[ai] == "-cerr_logging_parse") {
          cerr_logging_parse = std::string(args.at(ai + 1));
          realpath(cerr_logging_parse.c_str(), actual_path);
          cerr_logging_parse = actual_path;
          checkFileStatus(args[ai], actual_path);
        } else if (args[ai] == "-experiment_epoch") {
          experiment_epoch = std::string(args.at(ai + 1));
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
    return;
  }

  for (int ai = 2; ai < args.size(); ai += 2) {
    if (args[ai][0] != '-') {
      std::cerr << "Provided argument without a dash! Usage:" << std::endl;
      exit(EXIT_FAILURE);
    }
    try {
      if (args[ai] == "-efsunDictionary") {
        provided_translation_dictionary = std::string(args.at(ai + 1));
        realpath(provided_translation_dictionary.c_str(), actual_path);
        provided_translation_dictionary = actual_path;
        checkFileStatus(args[ai], actual_path);
      } else if (args[ai] == "-efsunDictionaryUnq") {
        provided_translation_dictionary_unq = std::string(args.at(ai + 1));
        realpath(provided_translation_dictionary_unq.c_str(), actual_path);
        provided_translation_dictionary_unq = actual_path;
        checkFileStatus(args[ai], actual_path);
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
      } else if (args[ai] == "-bilingual") {
        bilingual = true;
        ai--;
      } else if (args[ai] == "-eval_new") {
        eval_new = true;
        ai--;
      } else if (args[ai] == "-bilingual_debug") {
        bilingual_debug = true;
        ai--;
      } else if (args[ai] == "-src_map_f") {
        src_map_f = std::string(args.at(ai + 1));
        realpath(src_map_f.c_str(), actual_path);
        src_map_f = actual_path;
        checkFileStatus(args[ai], actual_path);
      } else if (args[ai] == "-tgt_map_f") {
        tgt_map_f = std::string(args.at(ai + 1));
        realpath(tgt_map_f.c_str(), actual_path);
        tgt_map_f = actual_path;
        checkFileStatus(args[ai], actual_path);
      } else if (args[ai] == "-src_map_f_all") {
        src_map_f_all = std::string(args.at(ai + 1));
        realpath(src_map_f_all.c_str(), actual_path);
        src_map_f_all = actual_path;
        checkFileStatus(args[ai], actual_path);
      } else if (args[ai] == "-tgt_map_f_all") {
        tgt_map_f_all = std::string(args.at(ai + 1));
        realpath(tgt_map_f_all.c_str(), actual_path);
        tgt_map_f_all = actual_path;
        checkFileStatus(args[ai], actual_path);
      } else if (args[ai] == "-src_map_f_bpe") {
        src_map_f_bpe = std::string(args.at(ai + 1));
        realpath(src_map_f_bpe.c_str(), actual_path);
        src_map_f_bpe = actual_path;
        checkFileStatus(args[ai], actual_path);
      } else if (args[ai] == "-tgt_map_f_bpe") {
        tgt_map_f_bpe = std::string(args.at(ai + 1));
        realpath(tgt_map_f_bpe.c_str(), actual_path);
        tgt_map_f_bpe = actual_path;
        checkFileStatus(args[ai], actual_path);
      } else if (args[ai] == "-use_morphology") {
        use_morphology = true;
        ai--;
      } else if (args[ai] == "-align") {
        align = std::string(args.at(ai + 1));
        realpath(align.c_str(), actual_path);
        align = actual_path;
        checkFileStatus(args[ai], actual_path);
      } else if (args[ai] == "-src-train") {
        src_train = std::string(args.at(ai + 1));
        realpath(src_train.c_str(), actual_path);
        src_train = actual_path;
        checkFileStatus(args[ai], actual_path);
      } else if (args[ai] == "-tgt-train") {
        tgt_train = std::string(args.at(ai + 1));
        realpath(tgt_train.c_str(), actual_path);
        tgt_train = actual_path;
        checkFileStatus(args[ai], actual_path);
      } else if (args[ai] == "-src-lang") {
        src_lang = std::string(args.at(ai + 1));
      } else if (args[ai] == "-tgt-lang") {
        tgt_lang = std::string(args.at(ai + 1));
      } else if (args[ai] == "-output") {
        output = std::string(args.at(ai + 1));
        realpath(output.c_str(), actual_path);
        output = actual_path;
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

  if (wordNgrams <= 1 && maxn == 0) {
    bucket = 0;
  }
}

void Args::printHelp() {
  std::cout << "Incorrect command" << std::endl;
}

} // namespace parfasttext
