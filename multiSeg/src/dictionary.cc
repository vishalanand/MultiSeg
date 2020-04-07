/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "dictionary.h"
#include "vector.h"

namespace parfasttext {

const std::string Dictionary::EOS = "</s>";
const std::string Dictionary::BOW = "<";
const std::string Dictionary::EOW = ">";

Dictionary::Dictionary(std::shared_ptr<Args> args)
    : args_(args),
      word2int_(MAX_VOCAB_SIZE, -1),
      size_(0),
      nwords_(0),
      nlabels_(0),
      ntokens_(0),
      pruneidx_size_(-1) {}

int32_t Dictionary::find(const std::string& w) const {
  return find(w, hash(w));
}

int32_t Dictionary::find(const std::string& w, uint32_t h) const {
  int32_t word2intsize = word2int_.size();
  int32_t id = h % word2intsize;
  while (word2int_[id] != -1 && words_[word2int_[id]].word != w) {
    id = (id + 1) % word2intsize;
  }
  return id;
}

void Dictionary::add(const std::string& w) {
  int32_t h = find(w);
  ntokens_++;
  if (word2int_[h] == -1) {
    entry e;
    e.word = w;
    e.count = 1;
    e.type = getType(w);
    words_.push_back(e);
    word2int_[h] = size_++;
  } else {
    words_[word2int_[h]].count++;
  }
}

int32_t Dictionary::nwords() const {
  return nwords_;
}

int64_t Dictionary::ntokens() const {
  return ntokens_;
}

const std::vector<int32_t>& Dictionary::getSubwords(int32_t i) const {
  assert(i >= 0);
  //std::cerr << i << ":\t" << nwords_ << std::endl;
  assert(i < nwords_);
  return words_[i].subwords;
}

const std::vector<int32_t> Dictionary::getSubwords(
    const std::string& word) const {

  if(args_->use_morphology) {
    int32_t i = getId(word);
    if (i >= 0) {
      return getSubwords(i);
    }
    std::vector<int32_t> ngrams;
    if (word != EOS) {
      computeSubwords(BOW + word + EOW, ngrams);
    }
    return ngrams;
  }
  else {
    int32_t i = getId(word);
    if (i >= 0) {
      return getSubwords(i);
    }
    std::vector<int32_t> ngrams;
    if (word != EOS) {
      computeSubwords(BOW + word + EOW, ngrams);
    }
    return ngrams;
  }

}

void Dictionary::load(std::istream& in, int64_t cnt) {
  words_.clear();

  std::string word_ngram;
  Vector word_ngram_vec(args_->dim);

  for (size_t i = 0; i < cnt; i++) {
    in >> word_ngram;
    for (int j = 0; j < args_->dim; ++j) {
      in >> word_ngram_vec[j];
    }

    add(word_ngram);

    int32_t word_id = getId(word_ngram);
    if(word_id == -1) continue;

    words_[word_id].subwords.clear();
    words_[word_id].subwords.push_back(word_id);
  }

  threshold(1, 0);
}

void Dictionary::getSubwords(
    const std::string& word,
    std::vector<int32_t>& ngrams,
    std::vector<std::string>& substrings) const {
  int32_t i = getId(word);
  ngrams.clear();
  substrings.clear();
  if (i >= 0) {
    ngrams.push_back(i);
    substrings.push_back(words_[i].word);
  }
  if (word != EOS) {
    computeSubwords(BOW + word + EOW, ngrams, &substrings);
  }
}

bool Dictionary::discard(int32_t id, real rand) const {
  assert(id >= 0);
  if (args_->verbose > 4) {
    std::cout << "nwords_: " << nwords_ << std::endl;
  }
  assert(id < nwords_);
  if (args_->model == model_name::sup) {
    return false;
  }
  bool return_cnt = rand > pdiscard_[id];

  if (return_cnt) {
    //std::cout << "Not included" << std::endl;
    true;
  }
  else {
    //std::cout << "\t\t\t\t\t\tIncluded" << std::endl;
    true;
  }

  return return_cnt;
}

int32_t Dictionary::getId(const std::string& w) const {
  int32_t h = find(w);
  return word2int_[h];
}

entry_type Dictionary::getType(int32_t id) const {
  assert(id >= 0);
  assert(id < size_);
  return words_[id].type;
}

entry_type Dictionary::getType(const std::string& w) const {
  //return (w.find(args_->label) == 0) ? entry_type::label : entry_type::word;
  return entry_type::word;
}

std::string Dictionary::getWord(int32_t id) const {
  assert(id >= 0);
  assert(id < size_);
  return words_[id].word;
}

// The correct implementation of fnv should be:
// h = h ^ uint32_t(uint8_t(str[i]));
// Unfortunately, earlier version of fasttext used
// h = h ^ uint32_t(str[i]);
// which is undefined behavior (as char can be signed or unsigned).
// Since all fasttext models that were already released were trained
// using signed char, we fixed the hash function to make models
// compatible whatever compiler is used.
uint32_t Dictionary::hash(const std::string& str) const {
  uint32_t h = 2166136261;
  for (size_t i = 0; i < str.size(); i++) {
    h = h ^ uint32_t(int8_t(str[i]));
    h = h * 16777619;
  }
  return h;
}

void Dictionary::computeSubwords(
    const std::string& word,
    std::vector<int32_t>& ngrams,
    std::vector<std::string>* substrings) const {
  for (size_t i = 0; i < word.size(); i++) {
    std::string ngram;
    if ((word[i] & 0xC0) == 0x80) {
      continue;
    }
    for (size_t j = i, n = 1; j < word.size() && n <= args_->maxn; n++) {
      ngram.push_back(word[j++]);
      while (j < word.size() && (word[j] & 0xC0) == 0x80) {
        ngram.push_back(word[j++]);
      }
      if (n >= args_->minn && !(n == 1 && (i == 0 || j == word.size()))) {
        int32_t h = hash(ngram) % args_->bucket;
        pushHash(ngrams, h);
        if (substrings) {
          substrings->push_back(ngram);
        }
      }
    }
  }
}

void Dictionary::initNgrams() {
  for (size_t i = 0; i < size_; i++) {
    std::string word = BOW + words_[i].word + EOW;
    words_[i].subwords.clear();
    words_[i].subwords.push_back(i);
    if (words_[i].word != EOS) {
      computeSubwords(word, words_[i].subwords);
    }
  }
}

void Dictionary::initNgramsAll(std::istream& in_morph_3, std::istream& in_morph_3p, std::istream& in_morph_bpe) {
  for (size_t i = 0; i < size_; i++) {
    std::string word = BOW + words_[i].word + EOW;
    words_[i].subwords.clear();
    words_[i].subwords.push_back(i);
    if (words_[i].word != EOS) {
      computeSubwords(word, words_[i].subwords);
    }
  }

  std::string word1, word2;
  std::string   line_morph;

  while (std::getline(in_morph_3, line_morph)) {
    std::stringstream   linestream(line_morph);

    std::getline(linestream, word1, '\t');

    int32_t word_id = getId(word1);
    if(word_id == -1) continue;

    // words_[word_id].subwords.clear();
    // words_[word_id].subwords.push_back(word_id);
    //std::cerr << "\nWord\t" << word1 << "\t";

    while(std::getline(linestream, word2, '\t')) {
      //std::cerr << "subword:" << word2 << "\t";
      if (words_[word_id].word != EOS) {
        computeSubwordsMorphology_each(word2, words_[word_id].subwords);
      }
    }
  }

  while (std::getline(in_morph_3p, line_morph)) {
    std::stringstream   linestream(line_morph);

    std::getline(linestream, word1, '\t');

    int32_t word_id = getId(word1);
    if(word_id == -1) continue;

    // words_[word_id].subwords.clear();
    // words_[word_id].subwords.push_back(word_id);
    //std::cerr << "\nWord\t" << word1 << "\t";

    while(std::getline(linestream, word2, '\t')) {
      //std::cerr << "subword:" << word2 << "\t";
      if (words_[word_id].word != EOS) {
        computeSubwordsMorphology_each(word2, words_[word_id].subwords);
      }
    }
  }

  while (std::getline(in_morph_bpe, line_morph)) {
    std::stringstream   linestream(line_morph);

    std::getline(linestream, word1, '\t');

    int32_t word_id = getId(word1);
    if(word_id == -1) continue;

    // words_[word_id].subwords.clear();
    // words_[word_id].subwords.push_back(word_id);
    //std::cerr << "\nWord\t" << word1 << "\t";

    while(std::getline(linestream, word2, '\t')) {
      //std::cerr << "subword:" << word2 << "\t";
      if (words_[word_id].word != EOS) {
        computeSubwordsMorphology_each(word2, words_[word_id].subwords);
      }
    }
  }

}

void Dictionary::initNgramsMorphology(std::istream& in_morph) {

  //std::cout << "Morphology" << std::endl;
  for (size_t i = 0; i < size_; i++) {
    words_[i].subwords.clear();
  }

  std::string word1, word2;
  std::string   line_morph;

  while (std::getline(in_morph, line_morph)) {
    std::stringstream   linestream(line_morph);

    std::getline(linestream, word1, '\t');

    int32_t word_id = getId(word1);
    if(word_id == -1) continue;

    words_[word_id].subwords.clear();
    words_[word_id].subwords.push_back(word_id);
    //std::cerr << "\nWord\t" << word1 << "\t";

    while(std::getline(linestream, word2, '\t')) {
      //std::cerr << "subword:" << word2 << "\t";
      if (words_[word_id].word != EOS) {
        computeSubwordsMorphology_each(word2, words_[word_id].subwords);
      }
    }
  }
}

void Dictionary::initNgramsMorphologyMultiple(std::vector<std::string> &in_morph_file) {

  //std::cout << "Morphology" << std::endl;
  for (size_t i = 0; i < size_; i++) {
    words_[i].subwords.clear();
  }

  std::string word1, word2;
  std::string   line_morph;

  for(int i=0; i<in_morph_file.size(); i++) {
    std::string in_string = in_morph_file[i];

    std::ifstream in_morph(in_string);

    while (std::getline(in_morph, line_morph)) {
      std::stringstream   linestream(line_morph);

      std::getline(linestream, word1, '\t');

      int32_t word_id = getId(word1);
      if(word_id == -1) continue;

      words_[word_id].subwords.clear();
      words_[word_id].subwords.push_back(word_id);
      //std::cerr << "\nWord\t" << word1 << "\t";

      while(std::getline(linestream, word2, '\t')) {
        //std::cerr << "subword:" << word2 << "\t";
        if (words_[word_id].word != EOS) {
          computeSubwordsMorphology_each(word2, words_[word_id].subwords);
        }
      }
    }
  }
}

void Dictionary::computeSubwordsMorphology_each(
        const std::string& word2,
        std::vector<int32_t>& ngrams,
        std::vector<std::string>* substrings) const {
  int32_t h;

  if(word2 != EMPTY) {
    h = hash(word2) % args_->bucket;
    pushHash(ngrams, h);
    if (substrings)
      substrings->push_back(word2);
  }
}

bool Dictionary::readWord(std::istream& in, std::string& word) const {
  int c;
  std::streambuf& sb = *in.rdbuf();
  word.clear();
  while ((c = sb.sbumpc()) != EOF) {
    if (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v' ||
        c == '\f' || c == '\0') {
      if (word.empty()) {
        if (c == '\n') {
          word += EOS;
          return true;
        }
        continue;
      } else {
        if (c == '\n')
          sb.sungetc();
        return true;
      }
    }
    word.push_back(c);
  }
  // trigger eofbit
  in.get();
  return !word.empty();
}

void Dictionary::readFromFile_Morphology(std::istream& in, std::istream& in_morph) {
  std::string word;
  int64_t minThreshold = 1;
  while (readWord(in, word)) {
    add(word);
    if (ntokens_ % 1000000 == 0 && args_->verbose > 1) {
      std::cerr << "\rRead " << ntokens_ / 1000000 << "M words" << std::flush;
    }
    if (size_ > 0.75 * MAX_VOCAB_SIZE) {
      minThreshold++;
      threshold(minThreshold, minThreshold);
    }
  }
  threshold(args_->minCount, args_->minCountLabel);
  initTableDiscard();

  initNgramsMorphology(in_morph);

  if (size_ == 0) {
    throw std::invalid_argument(
            "Empty vocabulary. Try a smaller -minCount value.");
  }
}

void Dictionary::readFromFile_All(std::istream& in, std::istream& in_morph_3, std::istream& in_morph_3p, std::istream& in_morph_bpe) {
  std::string word;
  int64_t minThreshold = 1;
  while (readWord(in, word)) {
    add(word);
    if (ntokens_ % 1000000 == 0 && args_->verbose > 1) {
      std::cerr << "\rRead " << ntokens_ / 1000000 << "M words" << std::flush;
    }
    if (size_ > 0.75 * MAX_VOCAB_SIZE) {
      minThreshold++;
      threshold(minThreshold, minThreshold);
    }
  }
  threshold(args_->minCount, args_->minCountLabel);
  initTableDiscard();

  // initNgramsAll(in_morph);
  initNgramsAll(in_morph_3, in_morph_3p, in_morph_bpe);

  if (size_ == 0) {
    throw std::invalid_argument(
      "Empty vocabulary. Try a smaller -minCount value.");
  }
}

void Dictionary::readFromFile(std::istream& in) {
  std::string word;
  int64_t minThreshold = 1;
  while (readWord(in, word)) {
    add(word);
    if (ntokens_ % 1000000 == 0 && args_->verbose > 1) {
      std::cerr << "\rRead " << ntokens_ / 1000000 << "M words" << std::flush;
    }
    if (size_ > 0.75 * MAX_VOCAB_SIZE) {
      minThreshold++;
      threshold(minThreshold, minThreshold);
    }
  }
  //std::cerr << "\rRead " << ntokens_ / 1000 << "K words mono" << std::endl;

  threshold(args_->minCount, args_->minCountLabel);
  initTableDiscard();
  initNgrams();
  if (size_ == 0) {
    throw std::invalid_argument(
        "Empty vocabulary. Try a smaller -minCount value.");
  }
}

void Dictionary::readFromMultipleFiles_Morphology(std::vector<std::string> in_file, std::vector<std::string> in_morph_file, int model_id) {
  std::string word;
  int64_t minThreshold = 1;
  int64_t ntokens_cur_ = 0;

  for(int i=0; i<in_file.size(); i++) {
    std::string in_string = in_file[i];

    std::ifstream in(in_string);

    if (model_id == i) {
      ntokens_cur_ = ntokens_;
    }

    while (readWord(in, word)) {
      add(word);
      if (ntokens_ % 1000000 == 0 && args_->verbose > 1) {
        std::cerr << "\rRead " << ntokens_ / 1000000 << "M words" << std::flush;
      }
      if (size_ > 0.75 * MAX_VOCAB_SIZE) {
        minThreshold++;
        threshold(minThreshold, minThreshold);
      }
    }

    //std::cerr << "\rRead " << ntokens_ / 1000 << "K words multiple" << std::endl;
    in.close();

    if(model_id == i) {
      ntokens_cur_ = ntokens_ - ntokens_cur_;
    }
  }
  threshold(args_->minCount, args_->minCountLabel);
  initTableDiscard();

  //initNgramsMorphology(in_morph);
  initNgramsMorphologyMultiple(in_morph_file);

  if (size_ == 0) {
    throw std::invalid_argument(
      "Empty vocabulary. Try a smaller -minCount value.");
  }
}

void Dictionary::readFromMultipleFiles(std::vector<std::string> in_file, int model_id) {
  std::string word;
  int64_t minThreshold = 1;

  int64_t ntokens_cur_ = 0;

  //for(const auto &in_string: in_file) {
  for(int i=0; i<in_file.size(); i++) {
    std::string in_string = in_file[i];

    std::ifstream in(in_string);

    if(model_id == i) {
      ntokens_cur_ = ntokens_;
    }

    while (readWord(in, word)) {
      add(word);
      if (ntokens_ % 1000000 == 0 && args_->verbose > 1) {
        std::cerr << "\rRead " << ntokens_ / 1000000 << "M words" << std::flush;
      }
      if (size_ > 0.75 * MAX_VOCAB_SIZE) {
        minThreshold++;
        threshold(minThreshold, minThreshold);
      }
    }
    //std::cerr << "\rRead " << ntokens_ / 1000 << "K words multiple" << std::endl;
    in.close();

    if(model_id == i) {
      ntokens_cur_ = ntokens_ - ntokens_cur_;
    }
  }

  ntokens_ = ntokens_cur_;
  //std::cerr << "\rRead " << ntokens_ / 1000 << "K words curr" << std::endl;

  threshold(args_->minCount, args_->minCountLabel);
  initTableDiscard();
  initNgrams();
  if (size_ == 0) {
    throw std::invalid_argument(
      "Empty vocabulary. Try a smaller -minCount value.");
  }
}

void Dictionary::threshold(int64_t t, int64_t tl) {
  sort(words_.begin(), words_.end(), [](const entry& e1, const entry& e2) {
    if (e1.type != e2.type) {
      return e1.type < e2.type;
    }
    return e1.count > e2.count;
  });
  words_.erase(
      remove_if(
          words_.begin(),
          words_.end(),
          [&](const entry& e) {
            return (e.type == entry_type::word && e.count < t) ||
                (e.type == entry_type::label && e.count < tl);
          }),
      words_.end());
  words_.shrink_to_fit();
  size_ = 0;
  nwords_ = 0;
  nlabels_ = 0;
  std::fill(word2int_.begin(), word2int_.end(), -1);
  for (auto it = words_.begin(); it != words_.end(); ++it) {
    int32_t h = find(it->word);
    word2int_[h] = size_++;
    if (it->type == entry_type::word) {
      nwords_++;
    }
    if (it->type == entry_type::label) {
      nlabels_++;
    }
  }
}

void Dictionary::initTableDiscard() {
  pdiscard_.resize(size_);
  for (size_t i = 0; i < size_; i++) {
    real f = real(words_[i].count) / real(ntokens_);
    pdiscard_[i] = std::sqrt(args_->t / f) + args_->t / f;
  }
}

std::vector<int64_t> Dictionary::getCounts(entry_type type) const {
  std::vector<int64_t> counts;
  for (auto& w : words_) {
    if (w.type == type) {
      counts.push_back(w.count);
    }
  }
  return counts;
}

void Dictionary::reset(std::istream& in) const {
  if (in.eof()) {
    in.clear();
    in.seekg(std::streampos(0));
  }
}

int32_t Dictionary::getAlignLine(
        std::istream& in,
        std::unordered_map< int, std::vector<int> > &src_align_pairs,
        std::unordered_map< int, std::vector<int> > &tgt_align_pairs) const {

  int32_t ntoken_pairs = 0, token1, token2;

  reset(in);
  src_align_pairs.clear();
  tgt_align_pairs.clear();

  char ch1;
  while ((in >> token1 >> token2)) {
    //if(args_->verbose > 4) {std::cout << token1 << ":" << token2 << " ";}
    //std::cout << token1 << ":" << token2 << " ";

    //align_map[token1] = token2;
    ntoken_pairs ++;

    //src_align_pairs.emplace_back(std::make_pair(token1, token2));
    src_align_pairs[token1].emplace_back(token2);
    tgt_align_pairs[token2].emplace_back(token1);

    if(in.peek() == '\n')
      break;
  }

  return ntoken_pairs;
}

int32_t Dictionary::getLine(
    std::istream& in,
    std::vector<int32_t>& words,
    std::minstd_rand& rng,
    std::vector< std::pair<std::string, int> > &wordtoken_wid) const {
  std::uniform_real_distribution<> uniform(0, 1);
  std::string token;
  int32_t ntokens = 0;

  reset(in);
  words.clear();
  wordtoken_wid.clear();

  std::string cur_wordtoken = "";
  int32_t cur_wid = -1;

  while (readWord(in, token)) {
    int32_t h = find(token);
    int32_t wid = word2int_[h];

    if(args_->bilingual_debug)
      std::cout << token << " "; // << wid << " ";
    cur_wordtoken = token;

    if(args_->bilingual_debug) {
      if (args_->verbose > 4) {
        std::cout << "token " << token << ", word_id " << wid << std::endl;
      }
    }

    if (wid < 0) {
      if(args_->bilingual_debug)
        std::cout << "Not found" << token << std::endl;
      wordtoken_wid.emplace_back(std::make_pair(cur_wordtoken, wid));
      continue;
    }

    ntokens++;

    if (getType(wid) == entry_type::word && !discard(wid, uniform(rng))) {
      words.push_back(wid);
      wordtoken_wid.emplace_back(std::make_pair(cur_wordtoken, wid));
    }
    else {
      //std::cout << "Not included" << std::endl;
      wordtoken_wid.emplace_back(std::make_pair(cur_wordtoken, -1));
      true;
    }
    if (ntokens > MAX_LINE_SIZE || token == EOS) {
      break;
    }
  }

  return ntokens;
}

void Dictionary::pushHash(std::vector<int32_t>& hashes, int32_t id) const {
  if (pruneidx_size_ == 0 || id < 0) {
    return;
  }
  if (pruneidx_size_ > 0) {
    if (pruneidx_.count(id)) {
      id = pruneidx_.at(id);
    } else {
      return;
    }
  }
  hashes.push_back(nwords_ + id);
}

} // namespace parfasttext
