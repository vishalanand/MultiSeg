/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "parfasttext.h"

namespace parfasttext {

ParFastText::ParFastText() {
  completed_model.store(0);
  FREQ_MASTER = std::make_shared<int> (FREQ_MASTER_CONST);
  FREQ_THREAD = std::make_shared<int> (FREQ_THREAD_CONST);
  shared_model = false;
};

void ParFastText::addInputVector(Vector& vec, int32_t ind, std::shared_ptr<Matrix> &input_p_) const {
  vec.addRow(*input_p_, ind);
}

void ParFastText::getWordVector(
                    Vector& vec,
                    const std::string& word,
                    std::shared_ptr<Dictionary> &dict_p,
                    std::shared_ptr<Matrix> &input_p_) const {
  //std::cerr << "Here" << std::endl;

  const std::vector<int32_t>& ngrams = dict_p->getSubwords(word);
  vec.zero();

  for (int i = 0; i < ngrams.size(); i++) {
    addInputVector(vec, ngrams[i], input_p_);
  }

  if (ngrams.size() > 0) {
    vec.mul(1.0 / ngrams.size());
  }
}


void ParFastText::saveBilingualVectors(const std::string& filename_src, const std::string& filename_tgt) {

  std::ofstream ofs_src(filename_src);
  std::ofstream ofs_tgt(filename_tgt);

  if (!ofs_src.is_open() || !ofs_tgt.is_open()) {
    throw std::invalid_argument(filename_src + " or " + filename_tgt + " cannot be opened for saving vectors!");
  }

  ofs_src << languageModel->dict_src_->nwords() << " " << languageModel->args_src_->dim << std::endl;
  if(shared_model) {
    ofs_tgt << languageModel->global_dict_tgt_->nwords() << " " << languageModel->args_tgt_->dim << std::endl;
  }
  else {
    ofs_tgt << languageModel->dict_tgt_->nwords() << " " << languageModel->args_tgt_->dim << std::endl;
  }

  Vector vec(languageModel->args_src_->dim);

  for (int32_t i = 0; i < languageModel->dict_src_->nwords(); i++) {
    std::string word = languageModel->dict_src_->getWord(i);
    getWordVector(vec, word, languageModel->dict_src_, languageModel->input_src_);
    ofs_src << word << " " << vec << std::endl;
  }

  if(shared_model == false) {
    for (int32_t i = 0; i < languageModel->dict_tgt_->nwords(); i++) {
      std::string word = languageModel->dict_tgt_->getWord(i);
      getWordVector(vec, word, languageModel->dict_tgt_, languageModel->input_tgt_);
      ofs_tgt << word << " " << vec << std::endl;
    }
  }
  else {
    for (int32_t i = 0; i < languageModel->global_dict_tgt_->nwords(); i++) {
      std::string word = languageModel->global_dict_tgt_->getWord(i);
      getWordVector(vec, word, languageModel->global_dict_tgt_, languageModel->input_tgt_);
      ofs_tgt << word << " " << vec << std::endl;
    }
  }

  ofs_src.close();
  ofs_tgt.close();
}

/** Evaluation code **/

void ParFastText::call_eval(
      char* filename_src_char, char* filename_tgt_char,
      char* src_lang_char, char* tgt_lang_char,
      char* provided_translation_dictionary_char,
      char* provided_translation_dictionary_unq_char,
      int dimension,
      bool call_eval_mono,
      bool call_eval_cldc,
      bool call_eval_muse,
      bool call_eval_dict_mult,
      bool call_eval_dict_unq) {

  std::cerr << "Iteration:\tEval Starts" << std::endl;

  if(call_eval_mono) {
    eval_mono(filename_src_char, src_lang_char);
    eval_mono(filename_tgt_char, tgt_lang_char);
  }

  if(strcmp(src_lang_char, "de") == 0) {
    if(call_eval_cldc) {
      cldc(filename_src_char, filename_tgt_char, src_lang_char, tgt_lang_char);
    }

    if(call_eval_muse) {
      museEval(filename_src_char, filename_tgt_char, src_lang_char, tgt_lang_char, dimension);
    }
  }

  if(call_eval_dict_mult) {
    if( strlen(provided_translation_dictionary_char) != 0 ) {
      dictionary_word_translate(
        filename_src_char, filename_tgt_char,
        src_lang_char, tgt_lang_char,
        provided_translation_dictionary_char);
    }
  }

  if(call_eval_dict_unq) {
    if (strlen(provided_translation_dictionary_unq_char) != 0) {
      dictionary_word_translate(
        filename_src_char, filename_tgt_char,
        src_lang_char, tgt_lang_char,
        provided_translation_dictionary_unq_char);
    }
  }
  std::cerr << "Iteration:\tEval Ends" << std::endl;
}

//void ParFastText::getWordVector(Vector& vec, const std::string& word, std::shared_ptr<Dictionary> &dict_p_oov, std::shared_ptr<Matrix> &input_p_oov) const {
//  const std::vector<int32_t>& ngrams = dict_p_oov->getSubwords(word);
//  vec.zero();
//  for (int i = 0; i < ngrams.size(); i++) {
//    addInputVector(vec, ngrams[i], input_p_oov);
//  }
//  if (ngrams.size() > 0) {
//    vec.mul(1.0 / ngrams.size());
//  }
//}

void ParFastText::loadModel(const std::string& filename_src, const std::string& filename_tgt, LanguageModel &lm_oov) {
  std::ifstream ifs_src_oov(filename_src), ifs_tgt_oov(filename_tgt);
  if (!checkModel(ifs_src_oov) || !checkModel(ifs_tgt_oov)) {
    throw std::invalid_argument(filename_src + " or "+ filename_tgt + " has wrong file format!");
  }

  lm_oov.args_src_ = std::make_shared<Args>();
  lm_oov.args_tgt_ = std::make_shared<Args>();

  int64_t cnt_src = 0, cnt_tgt = 0;
  ifs_src_oov >> cnt_src >> lm_oov.args_src_->dim;
  ifs_tgt_oov >> cnt_tgt >> lm_oov.args_tgt_->dim;

  lm_oov.dict_src_ = std::make_shared<Dictionary>(lm_oov.args_src_); lm_oov.dict_src_->load(ifs_src_oov, cnt_src);
  lm_oov.dict_tgt_ = std::make_shared<Dictionary>(lm_oov.args_src_); lm_oov.dict_tgt_->load(ifs_tgt_oov, cnt_tgt);

  std::cerr << "Loading src" << std::endl;
  lm_oov.input_src_ = std::make_shared<Matrix>(cnt_src, lm_oov.args_src_->dim); lm_oov.input_src_->load(ifs_src_oov);

  std::cerr << "Loading tgt" << std::endl;
  lm_oov.input_tgt_ = std::make_shared<Matrix>(cnt_tgt, lm_oov.args_tgt_->dim); lm_oov.input_tgt_->load(ifs_tgt_oov);

  ifs_src_oov.close();
  ifs_tgt_oov.close();

  std::cerr << "Loading complete" << std::endl;
}

bool ParFastText::checkModel(std::ifstream &filename) {
  return true;
}

void ParFastText::oov_lookup(
                  char* filename_src_char,
                  char* filename_tgt_char,
                  char* filename_src_bin_char,
                  char* filename_tgt_bin_char,
                  char* filename_src_fill_char,
                  char* filename_tgt_fill_char,
                  bool interactive,
                  bool subword_lookup_load,
                  char * dictionary_fill) {
  std::string word_lookup;
  LanguageModel lm_oov;

  if(subword_lookup_load) {
    loadModel(filename_src_bin_char, filename_tgt_bin_char, lm_oov);
  }

  Vector vec(lm_oov.args_src_->dim);
  int src_unseen = 0, src_seen = 0;
  int tgt_unseen = 0, tgt_seen = 0;
  bool unseen;
  std::unordered_map<std::string, bool> src_occur, tgt_occur;

  if(interactive) {
    std::cerr << "Interactive" << std::endl;
    while (interactive) {
      vec.zero();
      std::cerr << "Enter word to lookup [Language]:[Word]:\t";
      std::cin >> word_lookup;

      if (std::strcmp(word_lookup.c_str(), "OOV_end") == 0) {
        interactive = false;
        continue;
      }

      std::shared_ptr<Dictionary> *dict_oov;
      std::shared_ptr<Matrix> *input_oov;
      if (std::strcmp("en", word_lookup.substr(0, 2).c_str())) {
        dict_oov = &lm_oov.dict_src_;
        input_oov = &lm_oov.input_src_;
      } else {
        dict_oov = &lm_oov.dict_tgt_;
        input_oov = &lm_oov.input_tgt_;
      }
      getNgramVectorsNewConstruct(word_lookup.substr(3), lm_oov.args_src_, *dict_oov, *input_oov, vec);

      std::cerr << vec << std::endl;
    }
  } else {
    std::cerr << "Dictionary" << std::endl;
    std::shared_ptr<Dictionary> *dict_oov;
    std::shared_ptr<Matrix> *input_oov;
    std::string en_word, rare_word;
    std::string line;
    std::string string_word_lookup;

    std::ifstream ifs_dictionary_fill_read(dictionary_fill);
    while(getline(ifs_dictionary_fill_read, line)) {
      std::transform(line.begin(), line.end(), line.begin(), ::tolower);
      std::stringstream ss(line);

      getline(ss, en_word, '\t');
      //ss >> rare_word;
      getline(ss, rare_word, '\n');

      string_word_lookup = rare_word.substr(3);
      if(lm_oov.dict_src_->getId(string_word_lookup) < 0 && src_occur.find(string_word_lookup) == src_occur.end()) {
        src_unseen ++;
        src_occur[string_word_lookup] = true;
      } else {
        src_seen ++;
      }

      string_word_lookup = en_word.substr(3);
      if(lm_oov.dict_tgt_->getId(string_word_lookup) < 0 && tgt_occur.find(string_word_lookup) == tgt_occur.end()) {
        tgt_unseen ++;
        tgt_occur[string_word_lookup] = true;
      } else {
        tgt_seen ++;
      }
    }
    std::cerr << "Src:\tUnseen :\t" << src_unseen << "\tSeen:\t" << src_seen << std::endl;
    std::cerr << "Tgt:\tUnseen :\t" << tgt_unseen << "\tSeen:\t" << tgt_seen << std::endl;

    int vec_size, dim;
    std::ofstream src_fill(filename_src_fill_char);
    std::cerr << "filename_src_fill_char:\t" << filename_src_fill_char << std::endl;
    std::ifstream src_read_vec(filename_src_char);

    src_read_vec >> vec_size >> dim;
    src_fill << vec_size + src_unseen << " " << dim;
    while(getline(src_read_vec, line)) {
      std::transform(line.begin(), line.end(), line.begin(), ::tolower);
      src_fill << line << std::endl;
    }

    std::ofstream tgt_fill(filename_tgt_fill_char);
    std::ifstream tgt_read_vec(filename_tgt_char);
    std::cerr << "filename_tgt_fill_char:\t" << filename_tgt_fill_char << std::endl;
    tgt_read_vec >> vec_size >> dim;
    tgt_fill << vec_size + tgt_unseen << " " << dim;
    while(getline(tgt_read_vec, line)) {
      std::transform(line.begin(), line.end(), line.begin(), ::tolower);
      tgt_fill << line << std::endl;
    }

    std::ifstream ifs_dictionary_fill_rare(dictionary_fill);
    src_occur.clear();
    tgt_occur.clear();

    while(getline(ifs_dictionary_fill_rare, line)){
      std::transform(line.begin(), line.end(), line.begin(), ::tolower);
      std::stringstream ss(line);

      getline(ss, en_word, '\t');
      //ss >> rare_word;
      getline(ss, rare_word, '\n');

      dict_oov = &lm_oov.dict_src_;
      input_oov = &lm_oov.input_src_;
      string_word_lookup = rare_word.substr(3);
      vec.zero();
      unseen = getNgramVectorsNewConstruct(string_word_lookup, lm_oov.args_src_, *dict_oov, *input_oov, vec);

      if(unseen && src_occur.find(string_word_lookup) == src_occur.end()) {
        src_occur[string_word_lookup] = true;

        if (string_word_lookup.find (' ') != std::string::npos) {
          //std::cerr << "Rare:whitespace" << std::endl;
          std::replace(string_word_lookup.begin(), string_word_lookup.end(), ' ', '_');
          string_word_lookup = "SPACE_" + string_word_lookup;
        }

        src_fill << string_word_lookup << " " << vec << std::endl;
      }
    }
    ifs_dictionary_fill_rare.close();

    std::ifstream ifs_dictionary_fill(dictionary_fill);
    while(getline(ifs_dictionary_fill, line)){
      std::transform(line.begin(), line.end(), line.begin(), ::tolower);
      std::stringstream ss(line);

      getline(ss, en_word, '\t');
      //ss >> rare_word;
      getline(ss, rare_word, '\n');

      dict_oov = &lm_oov.dict_tgt_;
      input_oov = &lm_oov.input_tgt_;
      string_word_lookup = en_word.substr(3);
      vec.zero();
      unseen = getNgramVectorsNewConstruct(string_word_lookup, lm_oov.args_src_, *dict_oov, *input_oov, vec);

      if(unseen && tgt_occur.find(string_word_lookup) == tgt_occur.end()) {
        tgt_occur[string_word_lookup] = true;
        if (string_word_lookup.find (' ') != std::string::npos) {
          //std::cerr << "EN:whitespace" << std::endl;
          std::replace(string_word_lookup.begin(), string_word_lookup.end(), ' ', '_');
          string_word_lookup = "SPACE_" + string_word_lookup;
        }
        tgt_fill << string_word_lookup << " " << vec << std::endl;
      }
    }
    ifs_dictionary_fill.close();

    src_fill.close();   src_read_vec.close();
    tgt_fill.close();   tgt_read_vec.close();
  }
}

void ParFastText::call_parse_eval(char* cerr_logging_parse, char* experiment_epoch) {
  EvalParse evalParse;
  evalParse.read_logging(cerr_logging_parse, experiment_epoch);
  std::cerr << evalParse.create_line() << std::endl;
}

void ParFastText::execute(char* command){
  system(command);
}

void dirnameOf(char* fname_o, char*dirName) {
  size_t i;
  for (i = strlen(fname_o) - 1; i >= 0; i--) {
    if(fname_o[i] == '/') break;
  }

  strncpy(dirName, fname_o, i);
  dirName[i] = '\0';
}

void ParFastText::museEval(
                    char* filename_src,
                    char* filename_tgt,
                    char* src_lang_char,
                    char* tgt_lang_char,
                    int dimension) {

  char expName[MAX_STRING];
  char expPath[MAX_STRING];

  char cur_dir_char[MAX_STRING];
  std::string cur_dir = "./";
  realpath(cur_dir.c_str(), cur_dir_char);

  sprintf(expName, "%s_%s", filename_src, filename_tgt);
  dirnameOf(filename_src, expPath);

  char command[MAX_STRING];
  sprintf(command, "./run_museEval.sh %d %s %s %s %s %d %s %s %d",
          MAX_VOCAB,
          src_lang_char,
          tgt_lang_char,
          filename_src,
          filename_tgt,
          dimension,
          expPath,
          expName,
          1);
  fprintf(stdout, "\n%s\n", command);

  //chdir("../../eval/MUSE");
  chdir("../eval/MUSE");

  execute(command);
//  std::cerr << command << std::endl;
//  std::cerr << cur_dir_char << std::endl;
  chdir(cur_dir_char);
}

void ParFastText::dictionary_word_translate(
                    char* filename_src,
                    char* filename_tgt,
                    char* src_lang_char,
                    char* tgt_lang_char,
                    char* provided_translation_dictionary_char) {

  char cur_dir_char[MAX_STRING];
  std::string cur_dir = "./";
  realpath(cur_dir.c_str(), cur_dir_char);

  char case_insensitive_command[MAX_STRING];
  char case_sensitive_command[MAX_STRING];
  char emb[MAX_STRING];
  sprintf(emb, "%s.%s-%s.all", filename_src, src_lang_char, tgt_lang_char);

  chdir("../eval/word_translation");
  //Case in-sensitive
  sprintf(case_insensitive_command, "./word_translation.sh %s %s %s %s %s %s %s",
          emb, provided_translation_dictionary_char,
          src_lang_char, "redundantOutDir", "true",
          filename_src, filename_tgt);
  fprintf(stdout, "%s", case_insensitive_command);
  execute(case_insensitive_command);

//  //Case sensitive
//  sprintf(case_sensitive_command, "%s %s", case_insensitive_command, "True");
//  fprintf(stdout, "%s", case_sensitive_command);
//  execute(case_sensitive_command);
  chdir(cur_dir_char);
}

void ParFastText::eval_mono(char* emb_file, char* lang) {
  char cur_dir_char[MAX_STRING];
  std::string cur_dir = "./";
  realpath(cur_dir.c_str(), cur_dir_char);

  char new_lang[MAX_STRING];
  if(strcmp(lang, "en") != 0 && strcmp(lang, "de") != 0) {
    sprintf(new_lang, "rare");
  }
  else {
    sprintf(new_lang, "%s", lang);
  }

  char command[MAX_STRING];
  char also_print[MAX_STRING];

  /** WordSim **/
  chdir("../eval/wordsim/code");
  sprintf(also_print, "Iteration:\t%s\t%s\t", "wordSim", lang);
  std::cerr << also_print;
  sprintf(command, "./run_wordSim.sh %s 1 %s", emb_file, new_lang);
  execute(command);
  //chdir("../../../Bitext_thread");
  chdir(cur_dir_char);

  /** Analogy **/
  if(strcmp(lang, "en")==0){
    chdir("../eval/analogy/code");
    sprintf(also_print, "Iteration:\t%s\t%s\t", "analogy", "en");
    std::cerr << also_print;
    sprintf(command, "./run_analogy.sh %s 1", emb_file);
    execute(command);
    chdir(cur_dir_char);
  }
}

// cross-lingual document classification
void ParFastText::cldc(char* filename_src, char* filename_tgt, char* src_lang_char, char* tgt_lang_char) {
  if ( !( strcmp(src_lang_char, "de") == 0 && strcmp(tgt_lang_char, "en") == 0 ) ) {
    //Only allows src=de and tgt=en (as the cldc takes emb_src to be de, one could change that part of code for more flexibility)
    return;
  }

  char cur_dir_char[MAX_STRING];
  std::string cur_dir = "./";
  realpath(cur_dir.c_str(), cur_dir_char);

  char command[MAX_STRING];

//  /* de2en */
  chdir("../eval/cldc/scripts/de2en");
  sprintf(command, "./prepare-data-klement-4cat-1000-my-embeddings.ch %s %s", filename_src, filename_tgt);
  fprintf(stdout, "%s\n", command);
  execute(command);

  sprintf(command, "./run-perceptron-1000-my-embeddings.ch %s %s %s", filename_src, filename_tgt, "de2en");
  fprintf(stdout, "%s\n\n\n", command);
  execute(command);

  /** en2de **/
  chdir("../en2de");
  sprintf(command, "./prepare-data-klement-4cat-1000-my-embeddings.ch %s %s", filename_src, filename_tgt);
  fprintf(stdout, "%s\n", command);
  execute(command);

  sprintf(command, "./run-perceptron-1000-my-embeddings.ch %s %s %s", filename_src, filename_tgt, "en2de");
  fprintf(stdout, "%s\n", command);
  execute(command);

  chdir(cur_dir_char);
}
/** End Evaluation code **/

void ParFastText::saveBilingualModel(const std::string& filename_src, const std::string& filename_tgt) {
  std::ofstream ofs1(filename_src);
  std::vector<std::pair<std::string, Vector>> a;

  int64_t cnt = 0;
  for (size_t i = 0; i < languageModel->dict_src_->nwords(); i++) {
    a = getNgramVectorsNew(
          languageModel->dict_src_->getWord(i),
          languageModel->args_src_,
          languageModel->dict_src_,
          languageModel->input_src_
        );
    cnt += a.size();
  }
  ofs1 << cnt << " " << languageModel->args_src_->dim << std::endl;

  for (size_t i = 0; i < languageModel->dict_src_->nwords(); i++) {
    a = getNgramVectorsNew(
          languageModel->dict_src_->getWord(i),
          languageModel->args_src_,
          languageModel->dict_src_,
          languageModel->input_src_
        );
    for (int j = 0; j < a.size(); ++j) {
      ofs1 << a[j].first << " " << a[j].second << std::endl;
    }
  }
  ofs1.close();

  std::ofstream ofs2(filename_tgt);

  cnt = 0;
  for (size_t i = 0; i < languageModel->dict_tgt_->nwords(); i++) {
    cnt += a.size();
    a = getNgramVectorsNew(
          languageModel->dict_tgt_->getWord(i),
          languageModel->args_tgt_,
          languageModel->dict_tgt_,
          languageModel->input_tgt_
        );
  }
  ofs2 << cnt << " " << languageModel->args_tgt_->dim << std::endl;
  for (size_t i = 0; i < languageModel->dict_tgt_->nwords(); i++) {
    a = getNgramVectorsNew(
          languageModel->dict_tgt_->getWord(i),
          languageModel->args_tgt_,
          languageModel->dict_tgt_,
          languageModel->input_tgt_
        );
    for (int j = 0; j < a.size(); ++j) {
      ofs2 << a[j].first << " " << a[j].second << std::endl;
    }
  }
  ofs2.close();
}

void ParFastText::set_std_output_cerr_write(const std::string std_output) {
  sprintf(stdout_char, "%s_out.txt", std_output.c_str());
  sprintf(stderr_char, "%s_cerr.txt", std_output.c_str());
  freopen(stdout_char, "w", stdout);
  freopen(stderr_char, "w", stderr);

  //freopen ("/dev/tty", "a", stdout);
  //freopen ("/dev/tty", "a", stderr);
}

void ParFastText::set_std_output_cerr(const std::string std_output) {
  sprintf(stdout_char, "%s_out.txt", std_output.c_str());
  sprintf(stderr_char, "%s_cerr.txt", std_output.c_str());
  freopen(stdout_char, "a", stdout);
  freopen(stderr_char, "a", stderr);

  //freopen ("/dev/tty", "a", stdout);
  //freopen ("/dev/tty", "a", stderr);
}

std::vector<std::pair<std::string, Vector>> ParFastText::getNgramVectorsNew(
                                                          const std::string& word,
                                                          std::shared_ptr<Args> &args_p,
                                                          std::shared_ptr<Dictionary> &dict_p,
                                                          std::shared_ptr<Matrix> &input_p) const {
  std::vector<std::pair<std::string, Vector>> result;
  std::vector<int32_t> ngrams;
  std::vector<std::string> substrings;
  dict_p->getSubwords(word, ngrams, substrings);
  assert(ngrams.size() <= substrings.size());
  for (int32_t i = 0; i < ngrams.size(); i++) {
    Vector vec(args_p->dim);
    if (ngrams[i] >= 0) {
      vec.addRow(*input_p, ngrams[i]);
    }

    result.emplace_back(std::make_pair(substrings[i], std::move(vec)));
  }
  return result;
}

bool ParFastText::getNgramVectorsNewConstruct(
  const std::string& word,
  const std::shared_ptr<Args> &args_p,
  const std::shared_ptr<Dictionary> &dict_p,
  const std::shared_ptr<Matrix> &input_p,
  Vector &vec) const {

  int unseen = 0;

  //std::cout << "Word:\t" << word << std::endl;

  //Vector vec(args_p->dim);
  int dict_word_id_oov = dict_p->getId(word);
  //std::cout << "Word:\t" << word << "\tID:\t" << dict_word_id_oov << std::endl;

  if (dict_word_id_oov >= 0) {
    vec.addRow(*input_p, dict_word_id_oov);
  } else {
    unseen ++;
    std::vector<int32_t> ngrams;
    std::vector<std::string> substrings;
    dict_p->getSubwords(word, ngrams, substrings);
    //std::cout << "ngrams size:\t" << ngrams.size() << std::endl;
    assert(ngrams.size() <= substrings.size());

    for (const auto &substr_i: substrings) {
      //std::cerr << "Substring:\t" << substrings[i] << std::endl;

      int dict_substr_id_oov = dict_p->getId(substr_i);
      //std::cout << "Substring:\t" << substr_i << "\tID:\t" << dict_substr_id_oov << std::endl;

      if (dict_substr_id_oov >= 0) {
        vec.addRow(*input_p, dict_substr_id_oov);
      }
    }
  }

//  Vector vec_oov(args_p->dim);
//  getWordVector(vec_oov, word, dict_p, input_p);
//  std::cerr << "OOV:\t" << word << ":\t" << vec_oov << std::endl;

  //std::cerr << "OOV:\t" << word << ":\t" << vec << std::endl;

  //return std::move(vec);

  return unseen > 0;
}

void ParFastText::printInfo(real progress, real loss, std::ostream& log_stream) {
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  double t =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - languageModel->start_)
          .count() + languageModel->last_t_;
  languageModel->start_ = std::chrono::steady_clock::now();
  languageModel->last_t_ = t;

  double lr = languageModel->args_src_->lr * (1.0 - progress);
  double wst = 0;

  int64_t eta = 2592000; // Default to one month in seconds (720 * 3600)

  if (progress > 0 && t >= 0) {
    progress = progress * 100;
    eta = t * (100 - progress) / progress;
    wst = double(languageModel->tokenCount_tgt_) / t / languageModel->args_tgt_->thread;
  }
  int32_t etah = eta / 3600;
  int32_t etam = (eta % 3600) / 60;
  int32_t etas = eta % 60;

  if(languageModel->pause_print == false) {
    std::cerr << "\r";
    log_stream << std::fixed;
    log_stream << "Progress: ";
    log_stream << std::setprecision(1) << std::setw(5) << progress << "%";
    log_stream << " words/sec/thread: " << std::setw(7) << int64_t(wst);
    log_stream << " lr: " << std::setw(9) << std::setprecision(6) << lr;
    log_stream << " loss: " << std::setw(9) << std::setprecision(6) << loss;
    log_stream << " ETA: " << std::setw(3) << etah << "h";
    log_stream << std::setw(2) << etam << "m";
    log_stream << std::setw(2) << etas << "s";
    log_stream << std::flush;
  }
}

void ParFastText::call_pair_train(
                    Model& model_from,
                    Model& model_to,
                    real lr_from,
                    real lr_to,
                    const std::vector<int32_t>& line_from,
                    const std::vector<int32_t>& new_parallel_line_to,
                    std::shared_ptr<Dictionary> &dict_p_from_,
                    std::shared_ptr<Dictionary> &dict_p_to_,
                    std::unordered_map< int, std::vector<int> > &align_pairs_wid) {

  std::uniform_int_distribution<> uniform(1, languageModel->args_src_->ws);
  std::vector<int32_t> ngrams;

  for (int32_t w = 0; w < line_from.size(); w++) {
    ngrams.clear();
    int32_t boundary = uniform(model_from.rng);

    for (int i = 0; i < align_pairs_wid.size(); ++i) {
      for (int j = 0; j < align_pairs_wid[i].size(); ++j) {
        const std::vector<int32_t>& ngrams_temp = dict_p_to_->getSubwords(align_pairs_wid[i][j]);
        ngrams.insert(ngrams.end(), ngrams_temp.begin(), ngrams_temp.end());
      }
    }

    for (int32_t c = -boundary; c <= boundary; c++) {
      if (c != 0 && w + c >= 0 && w + c < line_from.size()) {
        model_from.update_bilingual(ngrams, line_from, w + c, lr_from, model_to);
      }
    }
  }
}

void ParFastText::bilingualSkipgram(
                    Model& model,
                    real lr,
                    const std::vector<int32_t>& line,
                    std::shared_ptr<Dictionary> &dict_p_) {

  std::uniform_int_distribution<> uniform(1, languageModel->args_src_->ws);

  for (int32_t w = 0; w < line.size(); w++) {
    int32_t boundary = uniform(model.rng);
    const std::vector<int32_t>& ngrams = dict_p_->getSubwords(line[w]);
    for (int32_t c = -boundary; c <= boundary; c++) {
      if (c != 0 && w + c >= 0 && w + c < line.size()) {
        model.update(ngrams, line, w + c, lr);
      }
    }
  }
}

void ParFastText::bilingualTrainThread(int32_t threadId) {
  //std::cerr << "shared:\t" + std::to_string(shared_model) + "\n";
  if(languageModel->visited_thread.load() == false) {
    languageModel->thread_elements[threadId].ifs_align.open(languageModel->args_src_->align);
    languageModel->thread_elements[threadId].ifs_src.open(languageModel->args_src_->src_train);
    languageModel->thread_elements[threadId].ifs_tgt.open(languageModel->args_tgt_->tgt_train);

    languageModel->thread_elements[threadId].thread_iter_move = threadId;
    utils::seek(
      languageModel->thread_elements[threadId].ifs_align,
      languageModel->line_blocks_align[languageModel->thread_elements[threadId].thread_iter_move]
    );
    utils::seek(
      languageModel->thread_elements[threadId].ifs_src,
      languageModel->line_blocks_src[languageModel->thread_elements[threadId].thread_iter_move]
    );
    utils::seek(
      languageModel->thread_elements[threadId].ifs_tgt,
      languageModel->line_blocks_tgt[languageModel->thread_elements[threadId].thread_iter_move]
    );

    languageModel->thread_elements[threadId].model_src.initialize(
      languageModel->input_src_,
      languageModel->output_src_,
      languageModel->args_src_,
      threadId
    );
    languageModel->thread_elements[threadId].model_tgt.initialize(
      languageModel->input_tgt_,
      languageModel->output_tgt_,
      languageModel->args_src_,
      threadId
    );
    languageModel->thread_elements[threadId].model_src.setTargetCounts(
      languageModel->dict_src_->getCounts(entry_type::word)
    );
    languageModel->thread_elements[threadId].model_tgt.setTargetCounts(
      (shared_model)? languageModel->global_dict_tgt_->getCounts(entry_type::word) : languageModel->dict_tgt_->getCounts(entry_type::word)
    );

    languageModel->thread_elements[threadId].localTokenCount_src = 0;
    languageModel->thread_elements[threadId].localTokenCount_tgt = 0;
    languageModel->thread_elements[threadId].local_lines = 0;

    languageModel->thread_elements[threadId].ntokens_src = languageModel->dict_src_->ntokens();
    languageModel->thread_elements[threadId].ntokens_tgt = languageModel->dict_tgt_->ntokens();
    languageModel->visited_thread.store(true);
  }
  const int64_t ntokens_tgt = languageModel->dict_tgt_->ntokens();

  while(true) {
    if(
      languageModel->thread_elements[threadId].local_lines ==
        languageModel->line_parts_src[languageModel->thread_elements[threadId].thread_iter_move + 1]
        - languageModel->line_parts_src[languageModel->thread_elements[threadId].thread_iter_move]) {

      if(languageModel->last_eval_lines.load() != languageModel->tokenCount_tgt_) {
        languageModel->semaphore--;
        languageModel->semaphore_thread[threadId] = false;

        while(languageModel->semaphore_thread[threadId] == false) {
          std::this_thread::sleep_for(std::chrono::milliseconds(*FREQ_THREAD));

          if(languageModel->iter_all > languageModel->args_src_->epoch) {
            if(languageModel->tokenCount_tgt_ > (languageModel->args_src_->epoch * ntokens_tgt) ) {
              std::cerr << "Breaking from the internal thread loop\n";
              break;
            }
            else {
              languageModel->semaphore_thread[threadId] = true;
            }
          }
        }
      }


      if(languageModel->iter_all > languageModel->args_src_->epoch) {
        if(languageModel->tokenCount_tgt_ > (languageModel->args_src_->epoch * ntokens_tgt) ) {
          std::cerr << "Breaking from the thread true loop\n";
          break;
        }
      }

      if(languageModel->signal_come_back_to_main.load()) {
        return;
      }
      languageModel->thread_elements[threadId].thread_iter_move =
        (languageModel->thread_elements[threadId].thread_iter_move + 1)%(languageModel->args_src_->thread);
      utils::seek(
        languageModel->thread_elements[threadId].ifs_align,
        languageModel->line_blocks_align[languageModel->thread_elements[threadId].thread_iter_move]
      );
      utils::seek(
        languageModel->thread_elements[threadId].ifs_src,
        languageModel->line_blocks_src[languageModel->thread_elements[threadId].thread_iter_move]
      );
      utils::seek(
        languageModel->thread_elements[threadId].ifs_tgt,
        languageModel->line_blocks_tgt[languageModel->thread_elements[threadId].thread_iter_move]
      );
      languageModel->thread_elements[threadId].local_lines = 0;
    }

    if(languageModel->tokenCount_tgt_ >= (languageModel->args_src_->epoch * ntokens_tgt)) {
      break;
    }


    languageModel->thread_elements[threadId].local_lines ++;
    real progress_src = real(languageModel->tokenCount_src_) / (
                          languageModel->args_src_->epoch * languageModel->thread_elements[threadId].ntokens_src
                        );
    real progress_tgt = real(languageModel->tokenCount_tgt_) / (
                          languageModel->args_src_->epoch * languageModel->thread_elements[threadId].ntokens_tgt
                        );
    real lr_src = languageModel->args_src_->lr * (1.0 - progress_src);
    real lr_tgt = languageModel->args_tgt_->lr * (1.0 - progress_tgt);

    languageModel->thread_elements[threadId].localTokenCount_src += languageModel->dict_src_->getLine(
                                                                      languageModel->thread_elements[threadId].ifs_src,
                                                                      languageModel->thread_elements[threadId].line_src,
                                                                      languageModel->thread_elements[threadId].model_src.rng,
                                                                      languageModel->thread_elements[threadId].wordtoken_wid_src
                                                                    );
    languageModel->thread_elements[threadId].localTokenCount_tgt += languageModel->dict_tgt_->getLine(
                                                                      languageModel->thread_elements[threadId].ifs_tgt,
                                                                      languageModel->thread_elements[threadId].line_tgt,
                                                                      languageModel->thread_elements[threadId].model_tgt.rng,
                                                                      languageModel->thread_elements[threadId].wordtoken_wid_tgt
                                                                    );
    bilingualSkipgram(
      languageModel->thread_elements[threadId].model_src,
      lr_src,
      languageModel->thread_elements[threadId].line_src,
      languageModel->dict_src_
    );
    bilingualSkipgram(
      languageModel->thread_elements[threadId].model_tgt,
      lr_tgt,
      languageModel->thread_elements[threadId].line_tgt,
      (shared_model)? languageModel->global_dict_tgt_ : languageModel->dict_tgt_
    );

    std::unordered_map< int, std::vector<int> > src_align_pairs, tgt_align_pairs;
    std::unordered_map< int, std::vector<int> > src_align_pairs_wid, tgt_align_pairs_wid;
    languageModel->dict_src_->getAlignLine(
      languageModel->thread_elements[threadId].ifs_align,
      src_align_pairs,
      tgt_align_pairs
    );

    for(int i=0; i<languageModel->thread_elements[threadId].wordtoken_wid_src.size(); i++) {
      if(languageModel->thread_elements[threadId].wordtoken_wid_src[i].second > -1) {
        for(int j=0; j<src_align_pairs[i].size(); j++) {

          int32_t cur_new_wid = (shared_model)?
            languageModel->global_dict_tgt_->getId(
              languageModel->thread_elements[threadId].wordtoken_wid_tgt[src_align_pairs[i][j]].first
            ): languageModel->dict_tgt_->getId(
              languageModel->thread_elements[threadId].wordtoken_wid_tgt[src_align_pairs[i][j]].first
            );

          if(cur_new_wid > -1) {
            languageModel->thread_elements[threadId].new_parallel_line_src.push_back(cur_new_wid);
            src_align_pairs_wid[i].push_back(cur_new_wid);
          }
        }
      }
    }

    for(int i=0; i<languageModel->thread_elements[threadId].wordtoken_wid_tgt.size(); i++) {
      if(languageModel->thread_elements[threadId].wordtoken_wid_tgt[i].second > -1) {
        for(int j=0; j<tgt_align_pairs[i].size(); j++) {
          int32_t cur_new_wid = languageModel->dict_src_->getId(
            languageModel->thread_elements[threadId].wordtoken_wid_src[tgt_align_pairs[i][j]].first
          );
          if(cur_new_wid > -1) {
            languageModel->thread_elements[threadId].new_parallel_line_tgt.push_back(cur_new_wid);
            tgt_align_pairs_wid[i].push_back(cur_new_wid);
          }
        }
      }
    }

    call_pair_train(
      languageModel->thread_elements[threadId].model_src,
      languageModel->thread_elements[threadId].model_tgt,
      lr_src,
      lr_tgt,
      languageModel->thread_elements[threadId].line_src,
      languageModel->thread_elements[threadId].new_parallel_line_src,
      languageModel->dict_src_,
      (shared_model)? languageModel->global_dict_tgt_ : languageModel->dict_tgt_,
      src_align_pairs_wid
    );

    call_pair_train(
      languageModel->thread_elements[threadId].model_tgt,
      languageModel->thread_elements[threadId].model_src,
      lr_tgt,
      lr_src,
      languageModel->thread_elements[threadId].line_tgt,
      languageModel->thread_elements[threadId].new_parallel_line_tgt,
      languageModel->dict_tgt_,
      languageModel->dict_src_,
      tgt_align_pairs_wid
    );

    if (( languageModel->thread_elements[threadId].localTokenCount_src > languageModel->args_src_->lrUpdateRate &&
            languageModel->thread_elements[threadId].localTokenCount_tgt > languageModel->args_src_->lrUpdateRate ) ||
        (languageModel->thread_elements[threadId].local_lines >= languageModel->line_parts_src[threadId + 1] - languageModel->line_parts_src[threadId]) ) {

      languageModel->tokenCount_src_ += languageModel->thread_elements[threadId].localTokenCount_src;
      languageModel->tokenCount_tgt_ += languageModel->thread_elements[threadId].localTokenCount_tgt;

      languageModel->thread_elements[threadId].localTokenCount_src = 0;
      languageModel->thread_elements[threadId].localTokenCount_tgt = 0;
      if (threadId == 0 && languageModel->args_src_->verbose > 0) {
        languageModel->loss_src_ = languageModel->thread_elements[threadId].model_src.getLoss();
        languageModel->loss_tgt_ = languageModel->thread_elements[threadId].model_tgt.getLoss();
      }
    }
  }
  languageModel->finish_train = true;

  if (threadId == 0) {
    languageModel->loss_src_ = languageModel->thread_elements[threadId].model_src.getLoss();
    languageModel->loss_tgt_ = languageModel->thread_elements[threadId].model_tgt.getLoss();
  }

  languageModel->thread_elements[threadId].ifs_src.close();
  languageModel->thread_elements[threadId].ifs_tgt.close();
  languageModel->thread_elements[threadId].ifs_align.close();
}

void ParFastText::validity(std::string src_file, std::string tgt_file, std::string align_file) {

  char line[MAX_SENT_LEN];
  long long int src_lines = 0, tgt_lines = 0, align_lines = 0;

  FILE *src_file_read = fopen(src_file.c_str(), "r");
  FILE *tgt_file_read = fopen(tgt_file.c_str(), "r");
  FILE *align_file_read = fopen(align_file.c_str(), "r");

  while (1) {
    fgets(line, MAX_SENT_LEN, src_file_read);
    if (feof(src_file_read)) break;
    ++src_lines;
  }

  while (1) {
    fgets(line, MAX_SENT_LEN, tgt_file_read);
    if (feof(tgt_file_read)) break;
    ++tgt_lines;
  }

  while (1) {
    fgets(line, MAX_SENT_LEN, align_file_read);
    if (feof(align_file_read)) break;
    ++align_lines;
  }
  std::cout << "num_src_lines " << src_lines << std::endl;
  std::cout << "num_tgt_lines " << tgt_lines << std::endl;
  std::cout << "num_align_lines " << align_lines << std::endl;

  assert(src_lines==tgt_lines);
  assert(src_lines==align_lines);

  ComputeBlockStartPoints(
    const_cast<char *>((languageModel->args_src_)->src_train.c_str()),
    (languageModel->args_src_)->thread,
    &languageModel->line_blocks_src,
    &languageModel->num_lines_src,
    &languageModel->line_parts_src
  );
  ComputeBlockStartPoints(
    const_cast<char *>((languageModel->args_src_)->tgt_train.c_str()),
    (languageModel->args_src_)->thread,
    &languageModel->line_blocks_tgt,
    &languageModel->num_lines_tgt,
    &languageModel->line_parts_tgt
  );
  ComputeBlockStartPoints(
    const_cast<char *>((languageModel->args_src_)->align.c_str()),
    (languageModel->args_src_)->thread,
    &languageModel->line_blocks_align,
    &languageModel->num_lines_align,
    &languageModel->line_parts_align
  );
}

void ParFastText::ComputeBlockStartPoints(
                        char* file_name,
                        int num_blocks,
                        long long **blocks,
                        long long *num_lines,
                        int **line_parts) {
  printf("# ComputeBlockStartPoints %s, num_blocks=%d\n", file_name, num_blocks);
  long long block_size;
  int line_count = 0;
  int curr_block = 0;
  char line[MAX_SENT_LEN];
  FILE *file;

  *num_lines = 0;
  file = fopen(file_name, "r");
  while (1) {
    fgets(line, MAX_SENT_LEN, file);
    if (feof(file)) break;
    ++(*num_lines);
  }
  printf("  num_lines=%lld, eof position %lld\n", *num_lines, (long long) ftell(file));

  fseek(file, 0, SEEK_SET);
  block_size = (*num_lines - 1) / num_blocks + 1;
  printf("  block_size=%lld lines\n  blocks = [0", block_size);

  *blocks = (long long *) malloc((num_blocks+1) * sizeof(long long));
  (*blocks)[0] = 0;

  *line_parts = (int *) malloc((num_blocks+1) * sizeof(int));
  (*line_parts)[0] = 0;

  curr_block = 0;
  long long int cur_size = 0;
  while (1) {
    fgets(line, MAX_SENT_LEN, file);
    line_count++;
    cur_size++;

    // done with a block or reach eof
    if (cur_size == block_size || line_count==(*num_lines)) {
      curr_block++;
      (*blocks)[curr_block] = (long long)ftell(file);
      (*line_parts)[curr_block] = (int)line_count;
      printf(" %lld", (*blocks)[curr_block]);
      if (line_count==(*num_lines)) { // eof
        break;
      }

      // reset
      cur_size = 0;
    }
  }
  printf("]\n");
  assert(curr_block==num_blocks);
  assert(line_count==(*num_lines));

  std::cout << "LineParts " << "[";
  for (int i = 0; i < num_blocks + 1; ++i) {
    std::cout << (*line_parts)[i] << " ";
  }
  std::cout << "]\n";

  fclose(file);
}

void ParFastText::bilingual_train(const Args& args) {
  languageModel = &l1;
  languageModel->args_src_ = std::make_shared<Args>(args);
  languageModel->args_tgt_ = std::make_shared<Args>(args);
  languageModel->dict_src_ = std::make_shared<Dictionary>(languageModel->args_src_);
  languageModel->dict_tgt_ = std::make_shared<Dictionary>(languageModel->args_tgt_);

  std::ifstream ifs_src(languageModel->args_src_->src_train), ifs_tgt(languageModel->args_tgt_->tgt_train);
  if (!ifs_src.is_open() || !ifs_tgt.is_open())
    throw std::invalid_argument(
      languageModel->args_src_->src_train + " or " + languageModel->args_tgt_->tgt_train + " cannot be opened for training!"
    );

  if(languageModel->args_src_->use_morphology) {
    std::ifstream ifs_src_map(languageModel->args_src_->src_map_f);
    std::ifstream ifs_tgt_map(languageModel->args_src_->tgt_map_f);
    if (!ifs_src_map.is_open() || !ifs_tgt_map.is_open()) {
      throw std::invalid_argument(
        languageModel->args_src_->src_map_f + " or " + languageModel->args_src_->tgt_map_f + " cannot be opened for training!");
    }

    languageModel->dict_src_->readFromFile_Morphology(ifs_src, ifs_src_map);
    languageModel->dict_tgt_->readFromFile_Morphology(ifs_tgt, ifs_tgt_map);

    ifs_src_map.close();
    ifs_tgt_map.close();
  } else {
    languageModel->dict_src_->readFromFile(ifs_src); ifs_src.close();
    languageModel->dict_tgt_->readFromFile(ifs_tgt); ifs_tgt.close();
  }

  validity(
    languageModel->args_src_->src_train,
    languageModel->args_tgt_->tgt_train,
    languageModel->args_src_->align
  );

  languageModel->input_src_ = std::make_shared<Matrix>(
    languageModel->dict_src_->nwords() + languageModel->args_src_->bucket,
    languageModel->args_src_->dim
  );
  languageModel->input_src_->uniform(1.0 / languageModel->args_src_->dim);

  languageModel->input_tgt_ = std::make_shared<Matrix>(
    languageModel->dict_tgt_->nwords() + languageModel->args_tgt_->bucket,
    languageModel->args_tgt_->dim
  );
  languageModel->input_tgt_->uniform(1.0 / languageModel->args_tgt_->dim);

  languageModel->output_src_ = std::make_shared<Matrix>(
    languageModel->dict_src_->nwords(),
    languageModel->args_src_->dim
  );
  languageModel->output_src_->zero();

  languageModel->output_tgt_ = std::make_shared<Matrix>(
    languageModel->dict_tgt_->nwords(),
    languageModel->args_tgt_->dim
  );
  languageModel->output_tgt_->zero();

  startBilingualThreads();
}

void ParFastText::bilingual_train_all(const Args& args) {
  languageModel = &l1;
  languageModel->args_src_ = std::make_shared<Args>(args);
  languageModel->args_tgt_ = std::make_shared<Args>(args);
  languageModel->dict_src_ = std::make_shared<Dictionary>(languageModel->args_src_);
  languageModel->dict_tgt_ = std::make_shared<Dictionary>(languageModel->args_tgt_);

  std::ifstream ifs_src(languageModel->args_src_->src_train), ifs_tgt(languageModel->args_tgt_->tgt_train);
  if (!ifs_src.is_open() || !ifs_tgt.is_open())
    throw std::invalid_argument(
      languageModel->args_src_->src_train + " or " + languageModel->args_tgt_->tgt_train + " cannot be opened for training!"
    );

  if(languageModel->args_src_->use_morphology) {
    std::ifstream ifs_src_map(languageModel->args_src_->src_map_f);
    std::ifstream ifs_tgt_map(languageModel->args_src_->tgt_map_f);

    std::ifstream ifs_src_map_f_all(languageModel->args_src_->src_map_f_all);
    std::ifstream ifs_tgt_map_f_all(languageModel->args_src_->tgt_map_f_all);

    std::ifstream ifs_src_map_f_bpe(languageModel->args_src_->src_map_f_bpe);
    std::ifstream ifs_tgt_map_f_bpe(languageModel->args_src_->tgt_map_f_bpe);
    if (!ifs_src_map.is_open() || !ifs_tgt_map.is_open()) {
      throw std::invalid_argument(
        languageModel->args_src_->src_map_f + " or " + languageModel->args_src_->tgt_map_f + " cannot be opened for training!");
    }

    //languageModel->dict_src_->readFromFile_Morphology(ifs_src, ifs_src_map);
    //readFromFile_All(std::istream& in, std::istream& in_morph_3, std::istream& in_morph_3p, std::istream& in_morph_bpe)
    languageModel->dict_src_->readFromFile_All(ifs_src, ifs_src_map, ifs_src_map_f_all, ifs_src_map_f_bpe);
    languageModel->dict_tgt_->readFromFile_All(ifs_tgt, ifs_tgt_map, ifs_tgt_map_f_all, ifs_tgt_map_f_bpe);

    ifs_src_map.close();
    ifs_tgt_map.close();

    ifs_src_map_f_all.close();
    ifs_src_map_f_bpe.close();
    ifs_tgt_map_f_all.close();
    ifs_tgt_map_f_bpe.close();
  } else {
    languageModel->dict_src_->readFromFile(ifs_src); ifs_src.close();
    languageModel->dict_tgt_->readFromFile(ifs_tgt); ifs_tgt.close();
  }

  validity(
    languageModel->args_src_->src_train,
    languageModel->args_tgt_->tgt_train,
    languageModel->args_src_->align
  );

  languageModel->input_src_ = std::make_shared<Matrix>(
    languageModel->dict_src_->nwords() + languageModel->args_src_->bucket,
    languageModel->args_src_->dim
  );
  languageModel->input_src_->uniform(1.0 / languageModel->args_src_->dim);

  languageModel->input_tgt_ = std::make_shared<Matrix>(
    languageModel->dict_tgt_->nwords() + languageModel->args_tgt_->bucket,
    languageModel->args_tgt_->dim
  );
  languageModel->input_tgt_->uniform(1.0 / languageModel->args_tgt_->dim);

  languageModel->output_src_ = std::make_shared<Matrix>(
    languageModel->dict_src_->nwords(),
    languageModel->args_src_->dim
  );
  languageModel->output_src_->zero();

  languageModel->output_tgt_ = std::make_shared<Matrix>(
    languageModel->dict_tgt_->nwords(),
    languageModel->args_tgt_->dim
  );
  languageModel->output_tgt_->zero();

  startBilingualThreads();
}

void ParFastText::construct_from_multiskip(const MultiskipArgs& multiskipargs, int i, Args &args_var) {
  args_var.bilingual = true;
  args_var.multiskip = multiskipargs.multiskip;
  args_var.eval_new = multiskipargs.eval_new;
  args_var.provided_translation_dictionary = multiskipargs.biskip_models[i].provided_translation_dictionary;
  args_var.provided_translation_dictionary_unq = multiskipargs.biskip_models[i].provided_translation_dictionary_unq;
  args_var.output = multiskipargs.biskip_models[i].output;

  args_var.lr = multiskipargs.lr;
  args_var.lrUpdateRate = multiskipargs.lrUpdateRate;
  args_var.dim = multiskipargs.dim;
  args_var.ws = multiskipargs.ws;
  args_var.epoch = multiskipargs.epoch;
  args_var.minCount = multiskipargs.minCount;
  args_var.minCountLabel = multiskipargs.minCountLabel;
  args_var.neg = multiskipargs.neg;
  args_var.model = multiskipargs.model;
  args_var.bucket = multiskipargs.bucket;
  args_var.thread = multiskipargs.thread;
  args_var.t = multiskipargs.t;
  args_var.verbose = multiskipargs.verbose;

  args_var.align = multiskipargs.biskip_models[i].align;
  args_var.bilingual_debug = multiskipargs.bilingual_debug;
  args_var.src_train = multiskipargs.biskip_models[i].src_train;
  args_var.tgt_train = multiskipargs.biskip_models[i].tgt_train;
  args_var.src_lang = multiskipargs.biskip_models[i].src_lang;
  args_var.tgt_lang = multiskipargs.biskip_models[i].tgt_lang;
  args_var.call_eval_mono = multiskipargs.call_eval_mono;
  args_var.call_eval_cldc = multiskipargs.call_eval_cldc;
  args_var.call_eval_muse = multiskipargs.call_eval_muse;
  args_var.call_eval_dict_mult = multiskipargs.call_eval_dict_mult;
  args_var.call_eval_dict_unq = multiskipargs.call_eval_dict_unq;
  args_var.save_bilingual_model = multiskipargs.save_bilingual_model;
  args_var.use_morphology = multiskipargs.use_morphology;
  args_var.src_map_f = multiskipargs.biskip_models[i].src_map_f;
  args_var.tgt_map_f = multiskipargs.biskip_models[i].tgt_map_f;

  args_var.wordNgrams = multiskipargs.wordNgrams;
  args_var.minn = multiskipargs.minn;
  args_var.maxn = multiskipargs.maxn;
}

void ParFastText::invoke_call_eval(std::string srcFile, std::string tgtFile, std::string word_binary_type) {
  //std::cerr << "\nCall eval\tIteration:\t" << languageModel->iter_all << std::endl;
  //return;

  std::cerr << "\nIteration:\t" << languageModel->iter_all << "\t"
            << word_binary_type << "\t"
            << "\nDim:\t" << languageModel->args_src_->dim
            << "\tSrc:\t" << languageModel->args_src_->src_lang
            << "\tTgt:\t" << languageModel->args_tgt_->tgt_lang
            << "\tThread:\t" << languageModel->args_src_->thread << std::endl;
  call_eval(
    const_cast<char *>(srcFile.c_str()),
    const_cast<char *>(tgtFile.c_str()),
    const_cast<char *>(languageModel->args_src_->src_lang.c_str()),
    const_cast<char *>(languageModel->args_tgt_->tgt_lang.c_str()),
    const_cast<char *>(languageModel->args_src_->provided_translation_dictionary.c_str()),
    const_cast<char *>(languageModel->args_src_->provided_translation_dictionary_unq.c_str()),
    languageModel->args_src_->dim,
    languageModel->args_src_->call_eval_mono,
    languageModel->args_src_->call_eval_cldc,
    languageModel->args_src_->call_eval_muse,
    languageModel->args_src_->call_eval_dict_mult,
    languageModel->args_src_->call_eval_dict_unq);
}

void ParFastText::insert_model(MultiskipArgs& multiskipargs) {
  //Try to figure out scope resolution to avoid individual object
  multiskipargs.languageModels.push_back(&l1);
  multiskipargs.languageModels.push_back(&l2);
  multiskipargs.languageModels.push_back(&l3);
  multiskipargs.languageModels.push_back(&l4);
  multiskipargs.languageModels.push_back(&l5);
  multiskipargs.languageModels.push_back(&l6);
  multiskipargs.languageModels.push_back(&l7);
  multiskipargs.languageModels.push_back(&l8);
  multiskipargs.languageModels.push_back(&l9);
  multiskipargs.languageModels.push_back(&l10);
  multiskipargs.languageModels.push_back(&l11);
  multiskipargs.languageModels.push_back(&l12);
}

void ParFastText::multi_bilingual_train(MultiskipArgs& multiskipargs) {
  insert_model(multiskipargs);
  //std::cerr << "multiskipargs.shared_tgt_model:\t" + std::to_string(multiskipargs.shared_tgt_model) + "\n";
  shared_model = multiskipargs.shared_tgt_model;

  //readFromMultipleFiles(
  std::vector<std::string> multi_skip_target, multi_skip_morphology;
  multi_skip_target.reserve(multiskipargs.languageModels.size());
  multi_skip_morphology.reserve(multiskipargs.languageModels.size());
  for (int i = 0; i < multiskipargs.biskip_models.size(); ++i) {
    multi_skip_target.push_back(multiskipargs.biskip_models[i].tgt_train);
    if(multiskipargs.use_morphology) {
      multi_skip_morphology.emplace_back(multiskipargs.biskip_models[i].tgt_map_f);
    }
  }

  for (int i = 0; i < multiskipargs.biskip_models.size(); ++i) {

    Args args_var = Args();

    construct_from_multiskip(multiskipargs, i, args_var);
    set_std_output_cerr(args_var.output);

    std::cerr << "multiskipargs.invoke_command " << multiskipargs.invoke_command << std::endl;
    std::cerr << "Index " << i << "\t"
              << multiskipargs.biskip_models[i].src_lang << "\t"
              << multiskipargs.biskip_models[i].tgt_lang << std::endl;

    multiskipargs.languageModels[i]->args_var_l = args_var;
    multiskipargs.languageModels[i]->args_src_ = std::make_shared<Args>(args_var);
    multiskipargs.languageModels[i]->args_tgt_ = std::make_shared<Args>(args_var);
    multiskipargs.languageModels[i]->dict_src_ = std::make_shared<Dictionary>(multiskipargs.languageModels[i]->args_src_);
    multiskipargs.languageModels[i]->global_dict_tgt_ = std::make_shared<Dictionary>(multiskipargs.languageModels[i]->args_tgt_);
    multiskipargs.languageModels[i]->dict_tgt_ = std::make_shared<Dictionary>(multiskipargs.languageModels[i]->args_tgt_);

    //Local file variables created
    std::ifstream ifs_src(multiskipargs.languageModels[i]->args_src_->src_train);
    std::ifstream ifs_tgt(multiskipargs.languageModels[i]->args_tgt_->tgt_train);
    if (!ifs_src.is_open() || !ifs_tgt.is_open())
      throw std::invalid_argument(
        multiskipargs.languageModels[i]->args_src_->src_train + " or "
        + multiskipargs.languageModels[i]->args_tgt_->tgt_train
        + " cannot be opened for training!"
      );

    if(multiskipargs.languageModels[i]->args_src_->use_morphology) {
      std::ifstream ifs_src_map(multiskipargs.languageModels[i]->args_src_->src_map_f);
      std::ifstream ifs_tgt_map(multiskipargs.languageModels[i]->args_src_->tgt_map_f);
      if (!ifs_src_map.is_open() || !ifs_tgt_map.is_open()) {
        throw std::invalid_argument(
          multiskipargs.languageModels[i]->args_src_->src_map_f + " or "
          + multiskipargs.languageModels[i]->args_src_->tgt_map_f
          + " cannot be opened for training!");
      }

      multiskipargs.languageModels[i]->dict_src_->readFromFile_Morphology(ifs_src, ifs_src_map);
      multiskipargs.languageModels[i]->dict_tgt_->readFromFile_Morphology(ifs_tgt, ifs_tgt_map);
      multiskipargs.languageModels[i]->global_dict_tgt_->readFromMultipleFiles_Morphology(multi_skip_target, multi_skip_morphology, i);

      ifs_src_map.close();
      ifs_tgt_map.close();
    } else {
      multiskipargs.languageModels[i]->dict_src_->readFromFile(ifs_src);
      multiskipargs.languageModels[i]->dict_tgt_->readFromFile(ifs_tgt);
      multiskipargs.languageModels[i]->global_dict_tgt_->readFromMultipleFiles(multi_skip_target, i);

      ifs_src.close();
      ifs_tgt.close();
    }
    //Local file variables closed
    languageModel = multiskipargs.languageModels[i];

    //Validity
    validity(
      multiskipargs.languageModels[i]->args_src_->src_train,
      multiskipargs.languageModels[i]->args_tgt_->tgt_train,
      multiskipargs.languageModels[i]->args_src_->align
    );

    //Input source starts
    multiskipargs.languageModels[i]->input_src_ = std::make_shared<Matrix>(
      multiskipargs.languageModels[i]->dict_src_->nwords() + multiskipargs.languageModels[i]->args_src_->bucket,
      multiskipargs.languageModels[i]->args_src_->dim
    );
    multiskipargs.languageModels[i]->input_src_->uniform(1.0 / multiskipargs.languageModels[i]->args_src_->dim);
    //Input source

    //Input target starts
    if(shared_model) {
      multiskipargs.languageModels[i]->input_tgt_ = std::make_shared<Matrix>(
        multiskipargs.languageModels[i]->global_dict_tgt_->nwords() + multiskipargs.languageModels[i]->args_tgt_->bucket,
        multiskipargs.languageModels[i]->args_tgt_->dim
      );
    }
    else {
      multiskipargs.languageModels[i]->input_tgt_ = std::make_shared<Matrix>(
        multiskipargs.languageModels[i]->dict_tgt_->nwords() + multiskipargs.languageModels[i]->args_tgt_->bucket,
        multiskipargs.languageModels[i]->args_tgt_->dim
      );
    }
    multiskipargs.languageModels[i]->input_tgt_->uniform(1.0 / multiskipargs.languageModels[i]->args_tgt_->dim);
    //Input target

    //Output source starts
    multiskipargs.languageModels[i]->output_src_ = std::make_shared<Matrix>(
      multiskipargs.languageModels[i]->dict_src_->nwords(),
      multiskipargs.languageModels[i]->args_src_->dim
    );
    multiskipargs.languageModels[i]->output_src_->zero();
    //Output source

    //Output target starts
    multiskipargs.languageModels[i]->output_tgt_ = std::make_shared<Matrix>(
      (shared_model) ? multiskipargs.languageModels[i]->global_dict_tgt_->nwords() : multiskipargs.languageModels[i]->dict_tgt_->nwords(),
      multiskipargs.languageModels[i]->args_tgt_->dim
    );
    multiskipargs.languageModels[i]->output_tgt_->zero();
    //Output target
  }

  for ( int model_id = 0;
        model_id < multiskipargs.biskip_models.size();
        model_id = (model_id+1)%multiskipargs.biskip_models.size()) {

    set_std_output_cerr(multiskipargs.languageModels[model_id]->args_var_l.output);
    if(completed_model.load() >= multiskipargs.biskip_models.size()) {
      break;
    }

    languageModel = multiskipargs.languageModels[model_id];
    languageModel->signal_come_back_to_main.store(false);

    if(languageModel->iter_all == 1) {
      languageModel->start_ = std::chrono::steady_clock::now();
      languageModel->semaphore_thread.reserve(languageModel->args_src_->thread);
      std::cerr << "Starting epoch 1" << std::endl;
    }

    languageModel->threads.clear();

    for (int32_t threadId = 0; threadId < languageModel->args_src_->thread; threadId++) {
      if(languageModel->iter_all == 1) {
        languageModel->thread_elements.emplace_back(ThreadElement());
      }
      languageModel->semaphore = languageModel->args_src_->thread;
      languageModel->semaphore_thread[threadId] = true;
      languageModel->threads.emplace_back(std::thread([=]() { bilingualTrainThread(threadId); }));
    }

    const int64_t ntokens_tgt = languageModel->dict_tgt_->ntokens();
    while (languageModel->finish_train == false && languageModel->signal_come_back_to_main.load() == false) {
      std::this_thread::sleep_for(std::chrono::milliseconds(*FREQ_MASTER));
      real progress = real(languageModel->tokenCount_tgt_) / (languageModel->args_src_->epoch * ntokens_tgt);
      printInfo(progress, (languageModel->loss_src_ + languageModel->loss_tgt_) / 2.0, std::cerr);

      if(languageModel->semaphore == 0 && languageModel->iter_all <= languageModel->args_src_->epoch) {

        if(languageModel->last_eval_lines.load() < languageModel->tokenCount_tgt_) {
          languageModel->pause_print = true;
          std::string srcFile = languageModel->args_src_->output + "."
                                + languageModel->args_src_->src_lang + ".iter"
                                + std::to_string(languageModel->iter_all) + ".vec";
          std::string tgtFile = languageModel->args_tgt_->output + "."
                                + languageModel->args_tgt_->tgt_lang + ".iter"
                                + std::to_string(languageModel->iter_all) + ".vec";

          saveBilingualVectors(srcFile, tgtFile);
          saveBilingualModel(srcFile + ".bin.txt", tgtFile + ".bin.txt");
          invoke_call_eval(srcFile, tgtFile, "Word");

          if(languageModel->args_src_->save_bilingual_model) {
            invoke_call_eval(srcFile + ".bin.txt", tgtFile + ".bin.txt", "Binary");
            std::cerr << std::endl;
          }
          languageModel->pause_print = false;

          languageModel->iter_all ++;
          languageModel->last_eval_lines.store(languageModel->tokenCount_tgt_);
          languageModel->signal_come_back_to_main.store(true);

          if(languageModel->iter_all <= languageModel->args_src_->epoch)
            std::cerr << "Starting epoch " + std::to_string(languageModel->iter_all) << std::endl;
        }

        languageModel->semaphore = languageModel->args_src_->thread;
        for(int i=0; i<languageModel->args_src_->thread; i++) {
          languageModel->semaphore_thread[i] = true;
        }

      }
      else if(languageModel->semaphore == 0) {
        std::cerr << "\nHere\n";

        languageModel->semaphore = languageModel->args_src_->thread;
        for(int i=0; i<languageModel->args_src_->thread; i++) {
          languageModel->semaphore_thread[i] = true;
        }
      }
    }

    if(languageModel->signal_come_back_to_main.load()) {
      for (int32_t i = 0; i < languageModel->args_src_->thread; i++) languageModel->threads[i].join();
    }

    else if(languageModel->iter_all > languageModel->args_src_->epoch) {
      for (int32_t i = 0; i < languageModel->args_src_->thread; i++) languageModel->threads[i].join();
      printInfo(1.0, (languageModel->loss_src_ + languageModel->loss_tgt_) / 2.0 , std::cerr);
      std::cerr << std::endl;
      completed_model.store(completed_model.load() + 1);
    }
  }
}

void ParFastText::startBilingualThreads() {
  languageModel->start_ = std::chrono::steady_clock::now();
  languageModel->last_t_ = 0;
  languageModel->tokenCount_src_ = 0, languageModel->tokenCount_tgt_ = 0;
  languageModel->loss_src_ = -1; languageModel->loss_tgt_ = -1;
  languageModel->iter_all = 1;
  languageModel->semaphore = languageModel->args_src_->thread;

  std::cerr << "Starting epoch 1" << std::endl;
  languageModel->semaphore_thread.reserve(languageModel->args_src_->thread);

  languageModel->threads.clear();
  for (int32_t threadId = 0; threadId < languageModel->args_src_->thread; threadId++) {
    if(languageModel->iter_all == 1) {
      languageModel->thread_elements.emplace_back(ThreadElement());
    }
    languageModel->semaphore_thread[threadId] = true;
    languageModel->threads.emplace_back(std::thread([=]() { bilingualTrainThread(threadId); }));
  }

  const int64_t ntokens_tgt = languageModel->dict_tgt_->ntokens();
  while (languageModel->finish_train == false) {
    std::this_thread::sleep_for(std::chrono::milliseconds(*FREQ_MASTER));
    real progress = real(languageModel->tokenCount_tgt_) / (languageModel->args_src_->epoch * ntokens_tgt);
    printInfo(progress, (languageModel->loss_src_ + languageModel->loss_tgt_) / 2.0, std::cerr);

    if(languageModel->semaphore == 0) {
      languageModel->pause_print = true;
      std::string srcFile = languageModel->args_src_->output + "."
                            + languageModel->args_src_->src_lang + ".iter"
                            + std::to_string(languageModel->iter_all) + ".vec";
      std::string tgtFile = languageModel->args_tgt_->output + "."
                            + languageModel->args_tgt_->tgt_lang + ".iter"
                            + std::to_string(languageModel->iter_all) + ".vec";

      saveBilingualVectors(srcFile, tgtFile);
      //saveBilingualModel(srcFile + ".bin.txt", tgtFile + ".bin.txt");

      invoke_call_eval(srcFile, tgtFile, "Word");
      if(languageModel->args_src_->save_bilingual_model) {
        invoke_call_eval(srcFile + ".bin.txt", tgtFile + ".bin.txt", "Binary");
        std::cerr << std::endl;
      }
      languageModel->pause_print = false;
      languageModel->iter_all ++;

      languageModel->semaphore = languageModel->args_src_->thread;
      for(int i=0; i<languageModel->args_src_->thread; i++) {
        languageModel->semaphore_thread[i] = true;
      }
    }
  }

  for (int32_t i = 0; i < languageModel->args_src_->thread; i++) languageModel->threads[i].join();
  printInfo(1.0, (languageModel->loss_src_ + languageModel->loss_tgt_) / 2.0 , std::cerr);
  std::cerr << std::endl;
}

} // namespace parfasttext
