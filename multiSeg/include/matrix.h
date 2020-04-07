/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstdint>
#include <istream>
#include <ostream>
#include <vector>
#include <assert.h>
#include <exception>
#include <random>
#include <stdexcept>
#include <iostream>

#include "constant.h"
#include "model.h"
#include "utils.h"
#include "vector.h"

namespace parfasttext {

class Vector;

class Matrix {
 protected:
  std::vector<real> data_;

 public:
        const int64_t m_;
        const int64_t n_;
  //Wrap this so as to move into protected
  //const int64_t m_;
  Matrix();
  explicit Matrix(int64_t, int64_t);
  Matrix(const Matrix&) = default;
  Matrix& operator=(const Matrix&) = delete;

  inline const real& at(int64_t i, int64_t j) const {
    return data_[i * n_ + j];
  };

  inline int64_t size(int64_t dim) const {
    assert(dim == 0 || dim == 1);
    if (dim == 0) {
      return m_;
    }
    return n_;
  }
  void zero();
  void uniform(real);
  real dotRow(const Vector&, int64_t) const;
  void addRow_bilingual_new(const Vector&, int64_t, real a, std::shared_ptr<Matrix> &, int64_t);
  void addRow(const Vector&, int64_t, real);
  void load(std::istream& in);
  void storeRow(const Vector& vec_ngram_read, const int64_t &i);
};
} // namespace parfasttext
