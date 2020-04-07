/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "vector.h"
#include "matrix.h"

namespace parfasttext {

Vector::Vector(int64_t m) : data_(m) {}

Vector::Vector(Vector&& other) noexcept : data_(std::move(other.data_)) {}

Vector& Vector::operator=(Vector&& other) {
  data_ = std::move(other.data_);
  return *this;
}

void Vector::zero() {
  std::fill(data_.begin(), data_.end(), 0.0);
}

void Vector::mul(real a) {
  for (int64_t i = 0; i < size(); i++) {
    data_[i] *= a;
  }
}

void Vector::addRow_bilingual(const Matrix& A, int64_t i, const Matrix& other_subword_model_A) {
//void Vector::addRow_bilingual(const Matrix& A, int64_t i) {
  assert(i >= 0);
  //assert(i < A.size(0));
  assert(i < other_subword_model_A.size(0));
  assert(size() == other_subword_model_A.size(1));
  assert(size() == A.size(1));

  //A.size(0) may not be equal to other_subword_model_A.size(0)
  //A.size(1) == other_subword_model_A.size(1)
  //std::cout << "Vector add-row" << std::endl;

  for (int64_t j = 0; j < A.size(1); j++) {
    //data_[j] += A.at(i, j);
    data_[j] += other_subword_model_A.at(i, j);
  }
}

void Vector::addRow(const Matrix& A, int64_t i) {
  assert(i >= 0);
  assert(i < A.size(0));
  assert(size() == A.size(1));
  for (int64_t j = 0; j < A.size(1); j++) {
    data_[j] += A.at(i, j);
  }
}

void Vector::addRow(const Matrix& A, int64_t i, real a) {
  assert(i >= 0);
  assert(i < A.size(0));
  assert(size() == A.size(1));
  for (int64_t j = 0; j < A.size(1); j++) {
    data_[j] += a * A.at(i, j);
  }
}

std::ostream& operator<<(std::ostream& os, const Vector& v) {
  os << std::setprecision(5);
  for (int64_t j = 0; j < v.size(); j++) {
    os << v[j] << ' ';
  }
  return os;
}

std::istream& operator>>(std::istream& is, Vector& v) {
  for (int64_t j = 0; j < v.size(); j++) {
    is >> v[j];
  }
  return is;
}

//void Vector::readVector(std::istream& is, Vector& v) {
//  for (int64_t j = 0; j < v.size(); j++) {
//    is >> v[j];
//  }
//  return is;
//}

} // namespace parfasttext
