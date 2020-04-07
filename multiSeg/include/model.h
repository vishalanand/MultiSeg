/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

#include "args.h"
#include "matrix.h"
#include "constant.h"
#include "vector.h"

namespace parfasttext {

class Model {
 protected:
  std::shared_ptr<Matrix> wi_;
  std::shared_ptr<Matrix> wo_;
  std::shared_ptr<Args> args_;
  Vector hidden_;
  Vector output_;
  Vector grad_;
  int32_t hsz_;
  int32_t osz_;
  real loss_;
  int64_t nexamples_;
  std::vector<real> t_sigmoid_;
  std::vector<real> t_log_;
  std::vector<int32_t> negatives_;
  size_t negpos;

  int32_t getNegative(int32_t target);
  void initSigmoid();
  void initLog();

  static const int32_t NEGATIVE_TABLE_SIZE = 10000000;

 public:
  Model();
  Model(int dim, int64_t wo_size, int32_t seed);
  Model(
      std::shared_ptr<Matrix>,
      std::shared_ptr<Matrix>,
      std::shared_ptr<Args>,
      int32_t);
  void initialize(
    std::shared_ptr<Matrix> wi,
    std::shared_ptr<Matrix> wo,
    std::shared_ptr<Args> args,
    int32_t seed);

  real binaryLogistic(int32_t, bool, real);
  real negativeSampling(int32_t, real);

  void update_bilingual(
      const std::vector<int32_t>& input,
      const std::vector<int32_t>& targets,
      int32_t targetIndex,
      real lr,
      Model &other_subword_model);
  void update(
      const std::vector<int32_t>&,
      const std::vector<int32_t>&,
      int32_t,
      real);
  real computeLoss(const std::vector<int32_t>&, int32_t, real);
  void computeHidden_bilingual(const std::vector<int32_t>& input, Vector& hidden, Model &other_subword_model) const;
  void computeHidden(const std::vector<int32_t>&, Vector&) const;

  void setTargetCounts(const std::vector<int64_t>&);
  void initTableNegatives(const std::vector<int64_t>&);
  real getLoss() const;
  real sigmoid(real) const;
  real log(real) const;

  std::minstd_rand rng;
};

}
