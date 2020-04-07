/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "model.h"
#include "utils.h"

namespace parfasttext {

constexpr int64_t SIGMOID_TABLE_SIZE = 512;
constexpr int64_t MAX_SIGMOID = 8;
constexpr int64_t LOG_TABLE_SIZE = 512;

//Allows for dummy default to enable later initialization

Model::Model()
  : hidden_(1),
    output_(1),
    grad_(1),
    rng(1) {};

Model::Model(int dim, int64_t wo_size, int32_t seed)
  : hidden_(dim),
    output_(wo_size),
    grad_(dim),
    rng(seed) {};

Model::Model(
    std::shared_ptr<Matrix> wi,
    std::shared_ptr<Matrix> wo,
    std::shared_ptr<Args> args,
    int32_t seed)
    : hidden_(args->dim),
      output_(wo->size(0)),
      grad_(args->dim),
      rng(seed) {
  initialize(wi, wo, args, seed);
}

void Model::initialize(
  std::shared_ptr<Matrix> wi,
  std::shared_ptr<Matrix> wo,
  std::shared_ptr<Args> args,
  int32_t seed) {

  output_ = Vector(wo->size(0));
  grad_ = Vector(args->dim);
  rng = std::minstd_rand(seed);
  hidden_ = Vector(args->dim);

  wi_ = wi;
  wo_ = wo;
  args_ = args;
  osz_ = wo->size(0);
  hsz_ = args->dim;
  negpos = 0;
  loss_ = 0.0;
  nexamples_ = 1;
  t_sigmoid_.reserve(SIGMOID_TABLE_SIZE + 1);
  t_log_.reserve(LOG_TABLE_SIZE + 1);
  initSigmoid();
  initLog();
}

real Model::binaryLogistic(int32_t target, bool label, real lr) {
  real score = sigmoid(wo_->dotRow(hidden_, target));
  real alpha = lr * (real(label) - score);
  grad_.addRow(*wo_, target, alpha);
  wo_->addRow(hidden_, target, alpha);
  if (label) {
    return -log(score);
  } else {
    return -log(1.0 - score);
  }
}

real Model::negativeSampling(int32_t target, real lr) {
  real loss = 0.0;
  grad_.zero();

  for (int32_t n = 0; n <= args_->neg; n++) {
    if (n == 0) {
      loss += binaryLogistic(target, true, lr);
    } else {
      loss += binaryLogistic(getNegative(target), false, lr);
    }
  }
  return loss;
}

void Model::computeHidden_bilingual(const std::vector<int32_t>& input, Vector& hidden, Model &other_subword_model)
    const {
  assert(hidden.size() == hsz_);
  hidden.zero();
  for (auto it = input.cbegin(); it != input.cend(); ++it) {
    //std::cout << "Here: " << (*wi_).size(0) << ": " << (*it) << std::endl;
    hidden.addRow_bilingual(*wi_, *it, *other_subword_model.wi_);
  }
  hidden.mul(1.0 / input.size());

}

void Model::computeHidden(const std::vector<int32_t>& input, Vector& hidden)
    const {
  assert(hidden.size() == hsz_);
  hidden.zero();
  for (auto it = input.cbegin(); it != input.cend(); ++it) {
    //std::cout << "Here: " << (*wi_).size(0) << ": " << (*it) << std::endl;
    hidden.addRow(*wi_, *it);
  }
  hidden.mul(1.0 / input.size());
}

real Model::computeLoss(
    const std::vector<int32_t>& targets,
    int32_t targetIndex,
    real lr) {
  return negativeSampling(targets[targetIndex], lr);
}

void Model::update_bilingual(
        const std::vector<int32_t>& input,
        const std::vector<int32_t>& targets,
        int32_t targetIndex,
        real lr,
        Model &other_subword_model) {

  if(args_->bilingual_debug) {
    std::cout << std::endl << "\nInputs [ngrams other] (" << input.size() << "): ";

    for (int i = 0; i < input.size(); ++i)
      std::cout << input[i] << " ";
    std::cout << std::endl << "Targets [line_from] (" << targets.size() << "): ";

    for (int i = 0; i < targets.size(); ++i)
      std::cout << targets[i] << " ";
    std::cout << std::endl;
  }

  //std::cout << "[update] input.size(): " << input.size() << "\ttargets.size(): " << targets.size() << "\tosz_: " << osz_ << std::endl;
  if (input.size() == 0) {
    return;
  }

  computeHidden_bilingual(input, hidden_, other_subword_model);

  //Only this executes
  assert(targetIndex >= 0);
  assert(targetIndex < osz_);
  if(args_->bilingual_debug)
    std::cout << "Here of2" << std::endl;
  //loss_ += computeLoss_bilingual(targets, targetIndex, lr, other_subword_model);
  loss_ += computeLoss(targets, targetIndex, lr);

  nexamples_ += 1;

  if (args_->model == model_name::sup) {
    grad_.mul(1.0 / input.size());
  }

  for (auto it = input.cbegin(); it != input.cend(); ++it) {
    wi_->addRow_bilingual_new(grad_, *it, 1.0, other_subword_model.wi_, other_subword_model.wi_->m_);
  }
}

void Model::update(
    const std::vector<int32_t>& input,
    const std::vector<int32_t>& targets,
    int32_t targetIndex,
    real lr) {
  if (input.size() == 0) {
    return;
  }
  computeHidden(input, hidden_);

  //Only this executes
  assert(targetIndex >= 0);
  assert(targetIndex < osz_);
  loss_ += computeLoss(targets, targetIndex, lr);

  nexamples_ += 1;

  if (args_->model == model_name::sup) {
    grad_.mul(1.0 / input.size());
  }

  for (auto it = input.cbegin(); it != input.cend(); ++it) {
    wi_->addRow(grad_, *it, 1.0);
  }
}

void Model::setTargetCounts(const std::vector<int64_t>& counts) {
  assert(counts.size() == osz_);
  initTableNegatives(counts);
}

void Model::initTableNegatives(const std::vector<int64_t>& counts) {
  real z = 0.0;
  for (size_t i = 0; i < counts.size(); i++) {
    z += pow(counts[i], 0.5);
  }
  for (size_t i = 0; i < counts.size(); i++) {
    real c = pow(counts[i], 0.5);
    for (size_t j = 0; j < c * NEGATIVE_TABLE_SIZE / z; j++) {
      negatives_.push_back(i);
    }
  }
  std::shuffle(negatives_.begin(), negatives_.end(), rng);
}

int32_t Model::getNegative(int32_t target) {
  int32_t negative;
  do {
    negative = negatives_[negpos];
    negpos = (negpos + 1) % negatives_.size();
  } while (target == negative);
  return negative;
}

real Model::getLoss() const {
  return loss_ / nexamples_;
}

void Model::initSigmoid() {
  for (int i = 0; i < SIGMOID_TABLE_SIZE + 1; i++) {
    real x = real(i * 2 * MAX_SIGMOID) / SIGMOID_TABLE_SIZE - MAX_SIGMOID;
    t_sigmoid_.push_back(1.0 / (1.0 + std::exp(-x)));
  }
}

void Model::initLog() {
  for (int i = 0; i < LOG_TABLE_SIZE + 1; i++) {
    real x = (real(i) + 1e-5) / LOG_TABLE_SIZE;
    t_log_.push_back(std::log(x));
  }
}

real Model::log(real x) const {
  if (x > 1.0) {
    return 0.0;
  }
  int64_t i = int64_t(x * LOG_TABLE_SIZE);
  return t_log_[i];
}

real Model::sigmoid(real x) const {
  if (x < -MAX_SIGMOID) {
    return 0.0;
  } else if (x > MAX_SIGMOID) {
    return 1.0;
  } else {
    int64_t i =
        int64_t((x + MAX_SIGMOID) * SIGMOID_TABLE_SIZE / MAX_SIGMOID / 2);
    return t_sigmoid_[i];
  }
}

}
