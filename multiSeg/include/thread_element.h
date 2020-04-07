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
  class ThreadElement {

  public:
    int32_t thread_iter_move;
    int32_t local_lines;
    int64_t localTokenCount_src;
    int64_t localTokenCount_tgt;
    int64_t ntokens_src;
    int64_t ntokens_tgt;

    std::ifstream ifs_align, ifs_src, ifs_tgt;
    std::vector<int32_t> line_src, line_tgt, new_parallel_line_src, new_parallel_line_tgt;
    std::vector< std::pair<std::string, int> > wordtoken_wid_src, wordtoken_wid_tgt;

    Model model_src;
    Model model_tgt;

    ThreadElement() = default;

  };
} // namespace parfasttext
