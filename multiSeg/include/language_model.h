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
#include <thread>
#include <tuple>

#include "args.h"
#include "dictionary.h"
#include "matrix.h"
#include "model.h"
#include "constant.h"
#include "utils.h"
#include "vector.h"
#include "thread_element.h"

namespace parfasttext {
  class LanguageModel {

  public:
    std::shared_ptr<Args> args_src_, args_tgt_;
    std::shared_ptr<Dictionary> dict_src_, dict_tgt_;
    std::shared_ptr<Dictionary> global_dict_tgt_;
    std::shared_ptr<Matrix> input_src_, output_src_;
    std::shared_ptr<Matrix> input_tgt_, output_tgt_;
    std::atomic<int64_t> tokenCount_src_{}, tokenCount_tgt_{};
    std::atomic<real> loss_src_{}, loss_tgt_{};
    std::atomic<int64_t> semaphore;
    std::vector<bool> semaphore_thread;
    std::atomic_bool pause_print, finish_train;
    std::chrono::steady_clock::time_point start_;
    long long *line_blocks_src, *line_blocks_tgt, *line_blocks_align;
    int *line_parts_src, *line_parts_tgt, *line_parts_align;
    long long num_lines_src, num_lines_tgt, num_lines_align;
    int32_t iter_all;
    double last_t_;
    Args args_var_l;
    std::vector<std::thread> threads;
    std::atomic<bool> signal_come_back_to_main;
    std::atomic<bool> visited_thread;
    std::atomic<int> last_eval_lines;
    std::vector<ThreadElement> thread_elements;

    LanguageModel(){
      pause_print = false;
      semaphore = 0;
      finish_train = false;
      last_t_ = 0;
      tokenCount_src_ = 0;
      tokenCount_tgt_ = 0;
      loss_src_ = -1;
      loss_tgt_ = -1;
      iter_all = 1;
      signal_come_back_to_main.store(false);
      finish_train = false;
      visited_thread.store(false);
      last_eval_lines.store(-1);
    }

  };
} // namespace parfasttext
