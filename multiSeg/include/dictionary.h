/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <istream>
#include <memory>
#include <ostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <codecvt>
#include <sstream>

#include "args.h"
#include "constant.h"

namespace parfasttext {

  enum class entry_type : int8_t { word = 0, label = 1 };

  struct entry {
    std::string word;
    int64_t count;
    entry_type type;
    std::vector<int32_t> subwords;
  };

  class Dictionary {
  protected:
    static const int32_t MAX_VOCAB_SIZE = 30000000;
    static const int32_t MAX_LINE_SIZE = 1024;

    int32_t find(const std::string&) const;
    int32_t find(const std::string&, uint32_t h) const;
    void initTableDiscard();
    void initNgrams();
    void initNgramsMorphology(std::istream& in_morph);
    void initNgramsMorphologyMultiple(std::vector<std::string> &in_morph_file);
    void initNgramsAll(std::istream& in_morph_3, std::istream& in_morph_3p, std::istream& in_morph_bpe);
    void reset(std::istream&) const;
    void pushHash(std::vector<int32_t>&, int32_t) const;

    std::shared_ptr<Args> args_;
    std::vector<int32_t> word2int_;
    std::vector<entry> words_;

    std::vector<real> pdiscard_;
    int32_t size_;
    int32_t nwords_;
    int32_t nlabels_;
    int64_t ntokens_;

    int64_t pruneidx_size_;
    std::unordered_map<int32_t, int32_t> pruneidx_;

  public:
    static const std::string EOS;
    static const std::string BOW;
    static const std::string EOW;

    explicit Dictionary(std::shared_ptr<Args>);
    int32_t nwords() const;
    int64_t ntokens() const;
    int32_t getId(const std::string&) const;
    int32_t getId(const std::string&, uint32_t h) const;
    entry_type getType(int32_t) const;
    entry_type getType(const std::string&) const;
    bool discard(int32_t, real) const;
    std::string getWord(int32_t) const;
    const std::vector<int32_t>& getSubwords(int32_t) const;
    const std::vector<int32_t> getSubwords(const std::string&) const;
    void getSubwords(
      const std::string&,
      std::vector<int32_t>&,
      std::vector<std::string>&) const;
    void computeSubwords(
      const std::string&,
      std::vector<int32_t>&,
      std::vector<std::string>* substrings = nullptr) const;
    void computeSubwordsMorphology_each(
      const std::string& word2,
      std::vector<int32_t>& ngrams,
      std::vector<std::string>* substrings = nullptr) const;
    uint32_t hash(const std::string& str) const;
    void add(const std::string&);
    bool readWord(std::istream&, std::string&) const;
    void readFromFile_Morphology(std::istream& in, std::istream& in_morph);
    void readFromFile_All(std::istream& in, std::istream& in_morph_3, std::istream& in_morph_3p, std::istream& in_morph_bpe);
    void readFromMultipleFiles_Morphology(std::vector<std::string> in_file, std::vector<std::string> in_morph, int model_id);
    void readFromFile(std::istream&);
    void readFromMultipleFiles(std::vector<std::string> in_file, int model_id);
    std::vector<int64_t> getCounts(entry_type) const;
    int32_t getAlignLine(std::istream& in,
                         std::unordered_map< int, std::vector<int> > &src_align_pairs,
                         std::unordered_map< int, std::vector<int> > &tgt_align_pairs)
    const;
    int32_t getLine(std::istream&, std::vector<int32_t>&, std::minstd_rand&, std::vector< std::pair<std::string, int> > &)
    const;
    void load(std::istream& in, int64_t cnt);
    void threshold(int64_t, int64_t);
  };

} // namespace parfasttext
