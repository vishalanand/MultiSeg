/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#define MAX_STRING 1000
//#define FREQ_MASTER_CONST 5000
//#define FREQ_THREAD_CONST 1000
#define FREQ_MASTER_CONST 4000
#define FREQ_THREAD_CONST 5000
#define MAX_SENT_LEN 20000
#define MAX_VOCAB 200000
#define EMPTY "__EMP__"

namespace parfasttext {
  typedef float real;
  enum class model_name : int {cbow = 1, sg, sup};
}
