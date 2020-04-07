/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits.h>
#include <numeric>
#include <string>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>

namespace parfasttext {

  class EvalParse {

  public:
    EvalParse();

    std::string create_line();
    void read_logging(char* cerr_logging_parse, char* experiment_epoch);

    std::stringstream stream_precision;
    void cldc_parse(std::string &cldc_str, std::string &parse);

  private:
    std::string wordSim_src, wordSim_tgt_ws353, wordSim_tgt_MC, wordSim_tgt_RG, wordSim_tgt_scws, wordSim_tgt_rare;

    std::string analogy_sem, analogy_syn, analogy_all;

    std::string cldc_src_en_100, cldc_src_en_200, cldc_src_en_500, cldc_src_en_1000, cldc_src_en_5000, cldc_src_en_10000;
    std::string cldc_en_src_100, cldc_en_src_200, cldc_en_src_500, cldc_en_src_1000, cldc_en_src_5000, cldc_en_src_10000;

    std::string multi_case_avg_p1, multi_case_avg_p10, multi_case_avg_cov, multi_case_avg_mutual;
    std::string multi_case_t2s_p1, multi_case_t2s_p10, multi_case_t2s_cov, multi_case_t2s_mutual;
    std::string multi_case_s2t_p1, multi_case_s2t_p10, multi_case_s2t_cov, multi_case_s2t_mutual;

    std::string unq_case_avg_p1, unq_case_avg_p10, unq_case_avg_cov, unq_case_avg_mutual;
    std::string unq_case_t2s_p1, unq_case_t2s_p10, unq_case_t2s_cov, unq_case_t2s_mutual;
    std::string unq_case_s2t_p1, unq_case_s2t_p10, unq_case_s2t_cov, unq_case_s2t_mutual;

    std::string MUSE_mono_word_sim_src_found, MUSE_mono_word_sim_src_notfound, MUSE_mono_word_sim_src_rho;
    std::string MUSE_mono_word_sim_en_found, MUSE_mono_word_sim_en_notfound, MUSE_mono_word_sim_en_rho;

    std::string MUSE_cross_word_sim_src_found, MUSE_cross_word_sim_src_notfound, MUSE_cross_word_sim_src_rho;
    std::string MUSE_cross_word_sim_en_found, MUSE_cross_word_sim_en_notfound, MUSE_cross_word_sim_en_rho;

    std::string MUSE_word_translate_num;
    std::string MUSE_word_translate_nn_1, MUSE_word_translate_nn_5, MUSE_word_translate_nn_10;
    std::string MUSE_word_translate_cslsknn10_1, MUSE_word_translate_cslsknn10_5, MUSE_word_translate_cslsknn10_10;

    std::string MUSE_sent_translate_queries;
    std::string MUSE_sent_translate_src_nn_1, MUSE_sent_translate_src_nn_5, MUSE_sent_translate_src_nn_10;
    std::string MUSE_sent_translate_en_nn_1, MUSE_sent_translate_en_nn_5, MUSE_sent_translate_en_nn_10;

    std::string MUSE_sent_translate_src_cslsknn10_1, MUSE_sent_translate_src_cslsknn10_5, MUSE_sent_translate_src_cslsknn10_10;
    std::string MUSE_sent_translate_en_cslsknn10_1, MUSE_sent_translate_en_cslsknn10_5, MUSE_sent_translate_en_cslsknn10_10;

    std::string MUSE_dist_mean_cosine_nn_s2t, MUSE_dist_mean_cosine_nn_s2t_dict;
    std::string MUSE_dist_mean_cosine_nn_t2s, MUSE_dist_mean_cosine_nn_t2s_dict;
    std::string MUSE_dist_mean_cosine_cslsknn10_s2t, MUSE_dist_mean_cosine_cslsknn10_s2t_dict;
    std::string MUSE_dist_mean_cosine_cslsknn10_t2s, MUSE_dist_mean_cosine_cslsknn10_t2s_dict;

    std::string experiment_name;
    std::string experiment_epoch;
  };
}
