/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "evalparse.h"

namespace parfasttext {

EvalParse::EvalParse() {
  wordSim_src = "-";
  wordSim_tgt_ws353 = "-";
  wordSim_tgt_MC = "-";
  wordSim_tgt_RG = "-";
  wordSim_tgt_scws = "-";
  wordSim_tgt_rare = "-";

  analogy_sem = "-";
  analogy_syn = "-";
  analogy_all = "-";

  cldc_src_en_100 = "-";
  cldc_src_en_200 = "-";
  cldc_src_en_500 = "-";
  cldc_src_en_1000 = "-";
  cldc_src_en_5000 = "-";
  cldc_src_en_10000 = "-";

  cldc_en_src_100 = "-";
  cldc_en_src_200 = "-";
  cldc_en_src_500 = "-";
  cldc_en_src_1000 = "-";
  cldc_en_src_5000 = "-";
  cldc_en_src_10000 = "-";

  multi_case_avg_p1 = "-";
  multi_case_avg_p10 = "-";
  multi_case_avg_cov = "-";
  multi_case_avg_mutual = "-";

  multi_case_t2s_p1 = "-";
  multi_case_t2s_p10 = "-";
  multi_case_t2s_cov = "-";
  multi_case_t2s_mutual = "-";

  multi_case_s2t_p1 = "-";
  multi_case_s2t_p10 = "-";
  multi_case_s2t_cov = "-";
  multi_case_s2t_mutual = "-";

  unq_case_avg_p1 = "-";
  unq_case_avg_p10 = "-";
  unq_case_avg_cov = "-";
  unq_case_avg_mutual = "-";

  unq_case_t2s_p1 = "-";
  unq_case_t2s_p10 = "-";
  unq_case_t2s_cov = "-";
  unq_case_t2s_mutual = "-";

  unq_case_s2t_p1 = "-";
  unq_case_s2t_p10 = "-";
  unq_case_s2t_cov = "-";
  unq_case_s2t_mutual = "-";

  MUSE_mono_word_sim_src_found = "-";
  MUSE_mono_word_sim_src_notfound = "-";
  MUSE_mono_word_sim_src_rho = "-";

  MUSE_mono_word_sim_en_found = "-";
  MUSE_mono_word_sim_en_notfound = "-";
  MUSE_mono_word_sim_en_rho = "-";

  MUSE_cross_word_sim_src_found = "-";
  MUSE_cross_word_sim_src_notfound = "-";
  MUSE_cross_word_sim_src_rho = "-";

  MUSE_cross_word_sim_en_found = "-";
  MUSE_cross_word_sim_en_notfound = "-";
  MUSE_cross_word_sim_en_rho = "-";

  MUSE_word_translate_num = "-";

  MUSE_word_translate_nn_1 = "-";
  MUSE_word_translate_nn_5 = "-";
  MUSE_word_translate_nn_10 = "-";

  MUSE_word_translate_cslsknn10_1 = "-";
  MUSE_word_translate_cslsknn10_5 = "-";
  MUSE_word_translate_cslsknn10_10 = "-";

  MUSE_sent_translate_queries = "-";

  MUSE_sent_translate_src_nn_1 = "-";
  MUSE_sent_translate_src_nn_5 = "-";
  MUSE_sent_translate_src_nn_10 = "-";

  MUSE_sent_translate_en_nn_1 = "-";
  MUSE_sent_translate_en_nn_5 = "-";
  MUSE_sent_translate_en_nn_10 = "-";

  MUSE_sent_translate_src_cslsknn10_1 = "-";
  MUSE_sent_translate_src_cslsknn10_5 = "-";
  MUSE_sent_translate_src_cslsknn10_10 = "-";

  MUSE_sent_translate_en_cslsknn10_1 = "-";
  MUSE_sent_translate_en_cslsknn10_5 = "-";
  MUSE_sent_translate_en_cslsknn10_10 = "-";

  MUSE_dist_mean_cosine_nn_s2t = "-";
  MUSE_dist_mean_cosine_nn_s2t_dict = "-";

  MUSE_dist_mean_cosine_nn_t2s = "-";
  MUSE_dist_mean_cosine_nn_t2s_dict = "-";

  MUSE_dist_mean_cosine_cslsknn10_s2t = "-";
  MUSE_dist_mean_cosine_cslsknn10_s2t_dict = "-";

  MUSE_dist_mean_cosine_cslsknn10_t2s = "-";
  MUSE_dist_mean_cosine_cslsknn10_t2s_dict = "-";
};

std::string EvalParse::create_line() {
  return \
    experiment_name + "\t" +  \
    experiment_epoch + "\t" +  \
    multi_case_avg_p1 + "\t" +  \
    multi_case_avg_p10 + "\t" +  \
    multi_case_avg_cov + "\t" +  \
    multi_case_avg_mutual + "\t" +  \
    multi_case_t2s_p1 + "\t" +  \
    multi_case_t2s_p10 + "\t" +  \
    multi_case_t2s_cov + "\t" +  \
    multi_case_t2s_mutual + "\t" +  \
    multi_case_s2t_p1 + "\t" +  \
    multi_case_s2t_p10 + "\t" +  \
    multi_case_s2t_cov + "\t" +  \
    multi_case_s2t_mutual + "\t" +  \

    wordSim_src + "\t" +  \
    wordSim_tgt_ws353 + "\t" +  \
    wordSim_tgt_MC + "\t" +  \
    wordSim_tgt_RG + "\t" +  \
    wordSim_tgt_scws + "\t" +  \
    wordSim_tgt_rare + "\t" +  \

    analogy_sem + "\t" +  \
    analogy_syn + "\t" +  \
    analogy_all + "\t" +  \

    cldc_src_en_100 + "\t" +  \
    cldc_src_en_200 + "\t" +  \
    cldc_src_en_500 + "\t" +  \
    cldc_src_en_1000 + "\t" +  \
    cldc_src_en_5000 + "\t" +  \
    cldc_src_en_10000 + "\t" +  \
    cldc_en_src_100 + "\t" +  \
    cldc_en_src_200 + "\t" +  \
    cldc_en_src_500 + "\t" +  \
    cldc_en_src_1000 + "\t" +  \
    cldc_en_src_5000 + "\t" +  \
    cldc_en_src_10000 + "\t" +  \

    MUSE_mono_word_sim_src_found + "\t" +  \
    MUSE_mono_word_sim_src_notfound + "\t" +  \
    MUSE_mono_word_sim_src_rho + "\t" +  \
    MUSE_mono_word_sim_en_found + "\t" +  \
    MUSE_mono_word_sim_en_notfound + "\t" +  \
    MUSE_mono_word_sim_en_rho + "\t" +  \
    MUSE_cross_word_sim_src_found + "\t" +  \
    MUSE_cross_word_sim_src_notfound + "\t" +  \
    MUSE_cross_word_sim_src_rho + "\t" +  \
    MUSE_cross_word_sim_en_found + "\t" +  \
    MUSE_cross_word_sim_en_notfound + "\t" +  \
    MUSE_cross_word_sim_en_rho + "\t" +  \
    MUSE_word_translate_num + "\t" +  \
    MUSE_word_translate_nn_1 + "\t" +  \
    MUSE_word_translate_nn_5 + "\t" +  \
    MUSE_word_translate_nn_10 + "\t" +  \
    MUSE_word_translate_cslsknn10_1 + "\t" +  \
    MUSE_word_translate_cslsknn10_5 + "\t" +  \
    MUSE_word_translate_cslsknn10_10 + "\t" +  \
    MUSE_sent_translate_queries + "\t" +  \
    MUSE_sent_translate_src_nn_1 + "\t" +  \
    MUSE_sent_translate_src_nn_5 + "\t" +  \
    MUSE_sent_translate_src_nn_10 + "\t" +  \
    MUSE_sent_translate_en_nn_1 + "\t" +  \
    MUSE_sent_translate_en_nn_5 + "\t" +  \
    MUSE_sent_translate_en_nn_10 + "\t" +  \
    MUSE_sent_translate_src_cslsknn10_1 + "\t" +  \
    MUSE_sent_translate_src_cslsknn10_5 + "\t" +  \
    MUSE_sent_translate_src_cslsknn10_10 + "\t" +  \
    MUSE_sent_translate_en_cslsknn10_1 + "\t" +  \
    MUSE_sent_translate_en_cslsknn10_5 + "\t" +  \
    MUSE_sent_translate_en_cslsknn10_10 + "\t" +  \
    MUSE_dist_mean_cosine_nn_s2t + "\t" +  \
    MUSE_dist_mean_cosine_nn_s2t_dict + "\t" +  \
    MUSE_dist_mean_cosine_nn_t2s + "\t" +  \
    MUSE_dist_mean_cosine_nn_t2s_dict + "\t" +  \
    MUSE_dist_mean_cosine_cslsknn10_s2t + "\t" +  \
    MUSE_dist_mean_cosine_cslsknn10_s2t_dict + "\t" +  \
    MUSE_dist_mean_cosine_cslsknn10_t2s + "\t" +  \
    MUSE_dist_mean_cosine_cslsknn10_t2s_dict + "\t" +  \

    unq_case_avg_p1 + "\t" +  \
    unq_case_avg_p10 + "\t" +  \
    unq_case_avg_cov + "\t" +  \
    unq_case_avg_mutual + "\t" +  \
    unq_case_t2s_p1 + "\t" +  \
    unq_case_t2s_p10 + "\t" +  \
    unq_case_t2s_cov + "\t" +  \
    unq_case_t2s_mutual + "\t" +  \
    unq_case_s2t_p1 + "\t" +  \
    unq_case_s2t_p10 + "\t" +  \
    unq_case_s2t_cov + "\t" +  \
    unq_case_s2t_mutual;
}

void EvalParse::cldc_parse(std::string &cldc_str, std::string &parse) {
  stream_precision.str("");
  stream_precision << std::fixed << std::setprecision(5) << std::stof(cldc_str.substr(strlen("Accuracy: ")));
  parse = stream_precision.str();
}

void invoke_load_line(std::vector<std::string> &log_arr, std::ifstream &myfile, std::string &line, std::string &token) {
  log_arr.clear();
  getline (myfile, line);
  std::istringstream iss(line);
  while(std::getline(iss, token, '\t')) {
    log_arr.push_back(token);
  }
}

void EvalParse::read_logging(char* cerr_logging_parse, char* logging_epoch) {
  std::string line, token;
  std::vector<std::string> log_arr;

  std::ifstream myfile (cerr_logging_parse);
  if (!myfile.is_open()) {
    throw std::invalid_argument(std::string(cerr_logging_parse) + " cannot be opened for training!");
  }

  experiment_name = std::string(cerr_logging_parse).substr(std::string(cerr_logging_parse).find_last_of('/') + 1);
  experiment_epoch = logging_epoch;
  if (myfile.is_open()) {
    while (! myfile.eof() ) {
      invoke_load_line(log_arr, myfile, line, token);

      if (line.rfind("Iteration:\tmulti\tcase\tAVG", 0) == 0) {
        multi_case_avg_p1 = log_arr[5];
        multi_case_avg_p10 = log_arr[7];
        multi_case_avg_cov = log_arr[9];
        multi_case_avg_mutual = log_arr[11];

        invoke_load_line(log_arr, myfile, line, token);

        multi_case_t2s_p1 = log_arr[5];
        multi_case_t2s_p10 = log_arr[7];
        multi_case_t2s_cov = log_arr[9];
        multi_case_t2s_mutual = log_arr[11];

        invoke_load_line(log_arr, myfile, line, token);

        multi_case_s2t_p1 = log_arr[5];
        multi_case_s2t_p10 = log_arr[7];
        multi_case_s2t_cov = log_arr[9];
        multi_case_s2t_mutual = log_arr[11];
      }
      else if (line.rfind("Iteration:\tunq\tcase\tAVG", 0) == 0) {
        unq_case_avg_p1 = log_arr[5];
        unq_case_avg_p10 = log_arr[7];
        unq_case_avg_cov = log_arr[9];
        unq_case_avg_mutual = log_arr[11];

        invoke_load_line(log_arr, myfile, line, token);

        unq_case_t2s_p1 = log_arr[5];
        unq_case_t2s_p10 = log_arr[7];
        unq_case_t2s_cov = log_arr[9];
        unq_case_t2s_mutual = log_arr[11];

        invoke_load_line(log_arr, myfile, line, token);

        unq_case_s2t_p1 = log_arr[5];
        unq_case_s2t_p10 = log_arr[7];
        unq_case_s2t_cov = log_arr[9];
        unq_case_s2t_mutual = log_arr[11];
      }
      else if (line.rfind("Iteration:\tMUSE\tmono_word_sim\t", 0) == 0) {
        MUSE_mono_word_sim_src_found = log_arr[6];
        MUSE_mono_word_sim_src_notfound = log_arr[8];
        MUSE_mono_word_sim_src_rho = log_arr[10];

        invoke_load_line(log_arr, myfile, line, token);

        MUSE_mono_word_sim_en_found = log_arr[6];
        MUSE_mono_word_sim_en_notfound = log_arr[8];
        MUSE_mono_word_sim_en_rho = log_arr[10];
      }
      else if (line.rfind("Iteration:\tMUSE\tcross_word_sim\t", 0) == 0) {
        MUSE_cross_word_sim_src_found = log_arr[7];
        MUSE_cross_word_sim_src_notfound = log_arr[9];
        MUSE_cross_word_sim_src_rho = log_arr[11];

        invoke_load_line(log_arr, myfile, line, token);

        MUSE_cross_word_sim_en_found = log_arr[7];
        MUSE_cross_word_sim_en_notfound = log_arr[9];
        MUSE_cross_word_sim_en_rho = log_arr[11];
      }
      else if (line.rfind("Iteration:\tMUSE\tword_translate\t", 0) == 0) {
        MUSE_word_translate_num = log_arr[3].substr(0, log_arr[3].find(" source words"));
        MUSE_word_translate_nn_1 = log_arr[3].substr(log_arr[3].find("k = 1: ") + strlen("k = 1: "));

        invoke_load_line(log_arr, myfile, line, token);
        MUSE_word_translate_nn_5 = log_arr[3].substr(log_arr[3].find("k = 5: ") + strlen("k = 5: "));

        invoke_load_line(log_arr, myfile, line, token);
        MUSE_word_translate_nn_10 = log_arr[3].substr(log_arr[3].find("k = 10: ") + strlen("k = 10: "));

        invoke_load_line(log_arr, myfile, line, token);
        MUSE_word_translate_cslsknn10_1 = log_arr[3].substr(log_arr[3].find("k = 1: ") + strlen("k = 1: "));

        invoke_load_line(log_arr, myfile, line, token);
        MUSE_word_translate_cslsknn10_5 = log_arr[3].substr(log_arr[3].find("k = 5: ") + strlen("k = 5: "));

        invoke_load_line(log_arr, myfile, line, token);
        MUSE_word_translate_cslsknn10_10 = log_arr[3].substr(log_arr[3].find("k = 10: ") + strlen("k = 10: "));
      }
      else if (line.rfind("Iteration:\tMUSE\tsent_translate\t", 0) == 0) {
        MUSE_sent_translate_queries = log_arr[3].substr(0, log_arr[3].find(" queries "));
        MUSE_sent_translate_src_nn_1 = log_arr[3].substr(log_arr[3].find("nn - Precision at k = 1: ") + strlen("nn - Precision at k = 1: "));
        invoke_load_line(log_arr, myfile, line, token);
        MUSE_sent_translate_src_nn_5 = log_arr[3].substr(log_arr[3].find("nn - Precision at k = 5: ") + strlen("nn - Precision at k = 5: "));
        invoke_load_line(log_arr, myfile, line, token);
        MUSE_sent_translate_src_nn_10 = log_arr[3].substr(log_arr[3].find("nn - Precision at k = 10: ") + strlen("nn - Precision at k = 10: "));

        invoke_load_line(log_arr, myfile, line, token);
        MUSE_sent_translate_en_nn_1 = log_arr[3].substr(log_arr[3].find("nn - Precision at k = 1: ") + strlen("nn - Precision at k = 1: "));
        invoke_load_line(log_arr, myfile, line, token);
        MUSE_sent_translate_en_nn_5 = log_arr[3].substr(log_arr[3].find("nn - Precision at k = 5: ") + strlen("nn - Precision at k = 5: "));
        invoke_load_line(log_arr, myfile, line, token);
        MUSE_sent_translate_en_nn_10 = log_arr[3].substr(log_arr[3].find("nn - Precision at k = 10: ") + strlen("nn - Precision at k = 10: "));

        invoke_load_line(log_arr, myfile, line, token);
        MUSE_sent_translate_src_cslsknn10_1 = log_arr[3].substr(log_arr[3].find("csls_knn_10 - Precision at k = 1: ") + strlen("csls_knn_10 - Precision at k = 1: "));
        invoke_load_line(log_arr, myfile, line, token);
        MUSE_sent_translate_src_cslsknn10_5 = log_arr[3].substr(log_arr[3].find("csls_knn_10 - Precision at k = 5: ") + strlen("csls_knn_10 - Precision at k = 5: "));
        invoke_load_line(log_arr, myfile, line, token);
        MUSE_sent_translate_src_cslsknn10_10 = log_arr[3].substr(log_arr[3].find("csls_knn_10 - Precision at k = 10: ") + strlen("csls_knn_10 - Precision at k = 10: "));
        invoke_load_line(log_arr, myfile, line, token);
        MUSE_sent_translate_en_cslsknn10_1 = log_arr[3].substr(log_arr[3].find("csls_knn_10 - Precision at k = 1: ") + strlen("csls_knn_10 - Precision at k = 1: "));
        invoke_load_line(log_arr, myfile, line, token);
        MUSE_sent_translate_en_cslsknn10_5 = log_arr[3].substr(log_arr[3].find("csls_knn_10 - Precision at k = 5: ") + strlen("csls_knn_10 - Precision at k = 5: "));
        invoke_load_line(log_arr, myfile, line, token);
        MUSE_sent_translate_en_cslsknn10_10 = log_arr[3].substr(log_arr[3].find("csls_knn_10 - Precision at k = 10: ") + strlen("csls_knn_10 - Precision at k = 10: "));
      }
      else if (line.rfind("Iteration:\tMUSE\tdist_mean_cosine\t", 0) == 0) {
        MUSE_dist_mean_cosine_nn_s2t = log_arr[3].substr(log_arr[3].find("max size): ") + strlen("max size): "));
        MUSE_dist_mean_cosine_nn_s2t_dict = log_arr[4].substr(log_arr[4].find("train_dictionary_size: ") + strlen("train_dictionary_size: "));

        invoke_load_line(log_arr, myfile, line, token);
        MUSE_dist_mean_cosine_nn_t2s = log_arr[3].substr(log_arr[3].find("max size): ") + strlen("max size): "));
        MUSE_dist_mean_cosine_nn_t2s_dict = log_arr[4].substr(log_arr[4].find("train_dictionary_size: ") + strlen("train_dictionary_size: "));

        invoke_load_line(log_arr, myfile, line, token);
        MUSE_dist_mean_cosine_cslsknn10_s2t = log_arr[3].substr(log_arr[3].find("max size): ") + strlen("max size): "));
        MUSE_dist_mean_cosine_cslsknn10_s2t_dict = log_arr[4].substr(log_arr[4].find("train_dictionary_size: ") + strlen("train_dictionary_size: "));

        invoke_load_line(log_arr, myfile, line, token);
        MUSE_dist_mean_cosine_cslsknn10_t2s = log_arr[3].substr(log_arr[3].find("max size): ") + strlen("max size): "));
        MUSE_dist_mean_cosine_cslsknn10_t2s_dict = log_arr[4].substr(log_arr[4].find("train_dictionary_size: ") + strlen("train_dictionary_size: "));
      }
      else if (line.rfind("Iteration:\twordSim\t", 0) == 0) {
        wordSim_src = log_arr[5];

        invoke_load_line(log_arr, myfile, line, token);

        wordSim_tgt_ws353 = log_arr[5];
        wordSim_tgt_MC = log_arr[7];
        wordSim_tgt_RG = log_arr[9];
        wordSim_tgt_scws = log_arr[11];
        wordSim_tgt_rare = log_arr[13];
      }
      else if (line.rfind("Iteration:\tanalogy\t", 0) == 0) {
        analogy_sem = log_arr[4];
        analogy_syn = log_arr[6];
        analogy_all = log_arr[8];
      }
      else if (line.rfind("Iteration:\tcldc\t", 0) == 0) {
        cldc_parse(log_arr[4], cldc_src_en_100);

        invoke_load_line(log_arr, myfile, line, token);
        cldc_parse(log_arr[4], cldc_src_en_200);

        invoke_load_line(log_arr, myfile, line, token);
        cldc_parse(log_arr[4], cldc_src_en_500);

        invoke_load_line(log_arr, myfile, line, token);
        cldc_parse(log_arr[4], cldc_src_en_1000);

        invoke_load_line(log_arr, myfile, line, token);
        cldc_parse(log_arr[4], cldc_src_en_5000);

        invoke_load_line(log_arr, myfile, line, token);
        cldc_parse(log_arr[4], cldc_src_en_10000);

        invoke_load_line(log_arr, myfile, line, token);
        cldc_parse(log_arr[4], cldc_en_src_100);

        invoke_load_line(log_arr, myfile, line, token);
        cldc_parse(log_arr[4], cldc_en_src_200);

        invoke_load_line(log_arr, myfile, line, token);
        cldc_parse(log_arr[4], cldc_en_src_500);

        invoke_load_line(log_arr, myfile, line, token);
        cldc_parse(log_arr[4], cldc_en_src_1000);

        invoke_load_line(log_arr, myfile, line, token);
        cldc_parse(log_arr[4], cldc_en_src_5000);

        invoke_load_line(log_arr, myfile, line, token);
        cldc_parse(log_arr[4], cldc_en_src_10000);
      }
    }
    myfile.close();
  }
}
}
