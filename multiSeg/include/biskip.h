/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <string>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>

namespace parfasttext {
class BiskipArgs {

public:
  BiskipArgs();

  std::string provided_translation_dictionary;
  std::string provided_translation_dictionary_unq;

  std::string align;
  std::string src_train, src_lang;
  std::string tgt_train, tgt_lang;
  std::string src_map_f, tgt_map_f;
  std::string output;
  bool bilingual;
};
} // namespace parfasttext