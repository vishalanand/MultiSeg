/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "utils.h"

namespace parfasttext {

namespace utils {

int64_t size(std::ifstream& ifs) {
  ifs.seekg(std::streamoff(0), std::ios::end);
  return ifs.tellg();
}

void seek(std::ifstream& ifs, int64_t pos) {
  ifs.clear();
  ifs.seekg(std::streampos(pos));
}
} // namespace utils

} // namespace parfasttext
