/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "matrix.h"

namespace parfasttext {

Matrix::Matrix() : Matrix(0, 0) {}

Matrix::Matrix(int64_t m, int64_t n) : data_(m * n), m_(m), n_(n) {}

void Matrix::zero() {
  std::fill(data_.begin(), data_.end(), 0.0);
}

void Matrix::uniform(real a) {
  std::minstd_rand rng(1);
  std::uniform_real_distribution<> uniform(-a, a);
  for (int64_t i = 0; i < (m_ * n_); i++) {
    data_[i] = uniform(rng);
  }
}

real Matrix::dotRow(const Vector& vec, int64_t i) const {
  assert(i >= 0);
  assert(i < m_);
  assert(vec.size() == n_);
  real d = 0.0;
  for (int64_t j = 0; j < n_; j++) {
    d += at(i, j) * vec[j];
  }
  if (std::isnan(d)) {
    throw std::runtime_error("Encountered NaN.");
  }
  return d;
}

void Matrix::addRow_bilingual_new(const Vector& vec, int64_t i, real a, std::shared_ptr<Matrix> &other_subword_matrix_, int64_t other_m_) {
  assert(i >= 0);
  //return;
  //std::cout << "Matrix add_row i: " << i << ": m: " << m_ << std::endl;
  //assert(i < m_);
  assert(i < other_m_);
  assert(vec.size() == n_);
  for (int64_t j = 0; j < n_; j++) {
    //Change the variable to update other subword model's ngrams
    //data_[i * n_ + j] += a * vec[j];

    other_subword_matrix_->data_[i * n_ + j] += a * vec[j];
  }
}

void Matrix::addRow(const Vector& vec, int64_t i, real a) {
  assert(i >= 0);
  //return;
  //std::cout << "Matrix add_row i: " << i << ": m: " << m_ << std::endl;
  assert(i < m_);
  assert(vec.size() == n_);
  for (int64_t j = 0; j < n_; j++) {
    data_[i * n_ + j] += a * vec[j];
  }
}

void Matrix::load(std::istream& in) {
  in.clear();
  in.seekg(std::streampos(0));

  int64_t cnt_cur = 0;
  in >> cnt_cur >> cnt_cur;

  std::string word_ngram;
  Vector word_ngram_vec(n_);

  for (int64_t i = 0; i < m_; i++) {
    //word_ngram_vec.zero();
    //in >> a[j].first << " " << a[j].second << std::endl;
    in >> word_ngram >> word_ngram_vec;
    //std::cerr << "Word:\t" << word_ngram << std::endl;
    //std::cerr << "Word vector:\t" << word_ngram_vec << std::endl;
    //data_[i] = word_ngram_vec;
    //std::cerr << i << "\t" << m_ << "\t" << n_ << std::endl;
    storeRow(word_ngram_vec, i);
    //std::cerr << "\t1" << std::endl;
  }
}

void Matrix::storeRow(const Vector& vec_ngram_read, const int64_t &i) {
  for (int j = 0; j < n_; ++j) {
    //std::cerr << j << std::endl;
    data_[i * n_ + j] = vec_ngram_read[j];
  }
}

} // namespace parfasttext
