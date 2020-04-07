/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <assert.h>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <utility>
#include <vector>

#include "constant.h"

namespace parfasttext {

class Matrix;

class Vector {
 protected:
  std::vector<real> data_;

 public:
  explicit Vector(int64_t);
  Vector(const Vector&) = delete;
  Vector(Vector&&) noexcept;
  Vector& operator=(const Vector&) = delete;
  Vector& operator=(Vector&&);

  inline real& operator[](int64_t i) {
    return data_[i];
  }
  inline const real& operator[](int64_t i) const {
    return data_[i];
  }

  inline int64_t size() const {
    return data_.size();
  }
  void zero();
  void mul(real);
  void addRow_bilingual(const Matrix&, int64_t, const Matrix& other_subword_model);
  void addRow(const Matrix&, int64_t);
  void addRow(const Matrix&, int64_t, real);
};

std::ostream& operator<<(std::ostream&, const Vector&);
std::istream& operator>>(std::istream&, Vector& v);

} // namespace parfasttext
