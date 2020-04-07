/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <iomanip>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <thread>
#include <sys/times.h>
#include "args.h"
#include "parfasttext.h"

using namespace parfasttext;

void train(const std::vector<std::string> &args, char* invoke_command) {
  Args args_var = Args();
  args_var.parseArgs(args);
  args_var.invoke_command = invoke_command;

  if(args_var.bilingual) {

    ParFastText parfasttext;

    parfasttext.set_std_output_cerr(args_var.output);
    std::cerr << args_var.invoke_command << std::endl;

    parfasttext.bilingual_train(args_var);
  }
}

void train_all(const std::vector<std::string> &args, char* invoke_command) {
  Args args_var = Args();
  args_var.parseArgs(args);
  args_var.invoke_command = invoke_command;

  if(args_var.bilingual) {

    ParFastText parfasttext;

    parfasttext.set_std_output_cerr(args_var.output);
    std::cerr << args_var.invoke_command << std::endl;

    parfasttext.bilingual_train_all(args_var);
  }
}

void call_eval_only(const std::vector<std::string> &args, char* invoke_command) {
  Args args_var = Args();
  args_var.parseArgs(args);

  ParFastText parfastText;

  //parfastText.set_std_output_cerr_write(args_var.output);
  std::cerr << invoke_command << std::endl;

  parfastText.call_eval(
      const_cast<char *>(args_var.src_emb.c_str()),
      const_cast<char *>(args_var.tgt_emb.c_str()),
      const_cast<char *>(args_var.src_lang.c_str()),
      const_cast<char *>(args_var.tgt_lang.c_str()),
      const_cast<char *>(args_var.provided_translation_dictionary.c_str()),
      const_cast<char *>(args_var.provided_translation_dictionary_unq.c_str()),
      args_var.dim,
      args_var.call_eval_mono,
      args_var.call_eval_cldc,
      args_var.call_eval_muse,
      args_var.call_eval_dict_mult,
      args_var.call_eval_dict_unq);
}

void call_oov_lookup(const std::vector<std::string> &args, char* invoke_command) {
  Args args_var = Args();
  args_var.parseArgs(args);

  ParFastText parfastText;

  //parfastText.set_std_output_cerr_write(args_var.output);
  std::cerr << invoke_command << std::endl;

  parfastText.oov_lookup(
    const_cast<char *>(args_var.src_emb.c_str()),
    const_cast<char *>(args_var.tgt_emb.c_str()),
    const_cast<char *>(args_var.src_bin_emb.c_str()),
    const_cast<char *>(args_var.tgt_bin_emb.c_str()),
    const_cast<char *>(args_var.src_fill_emb.c_str()),
    const_cast<char *>(args_var.tgt_fill_emb.c_str()),
    args_var.interactive,
    args_var.subword_lookup_load,
    const_cast<char *>(args_var.dictionary_fill.c_str()));
}

void call_parse_eval(const std::vector<std::string> &args, char* invoke_command) {
  Args args_var = Args();
  args_var.parseArgs(args);

  ParFastText parfastText;

  parfastText.set_std_output_cerr("parse_output");
  //std::cerr << invoke_command << std::endl;

  parfastText.call_parse_eval(
    const_cast<char *>(args_var.cerr_logging_parse.c_str()),
    const_cast<char *>(args_var.experiment_epoch.c_str()));
}

void call_multiskip(const std::vector<std::string> &args, char* invoke_command) {
  MultiskipArgs multiskipargs_var = MultiskipArgs();
  multiskipargs_var.parseArgs(args);
  multiskipargs_var.invoke_command = invoke_command;
  sprintf(
    multiskipargs_var.invoke_command, "%s\nmultiskipargs.shared_tgt_model:\t%d",
      multiskipargs_var.invoke_command,
      multiskipargs_var.shared_tgt_model
  );

  ParFastText parfasttext;
  parfasttext.multi_bilingual_train(multiskipargs_var);
}

std::string parse_tim(intmax_t milliseconds) {
  std::string parsed;

  intmax_t s = milliseconds / 1000;
  intmax_t m = s / 60;
  intmax_t h = m / 60;
  intmax_t d = h / 24;

  if(d > 0)
    parsed = parsed + std::to_string(d) + "d";
  if(! parsed.empty() || h > 0)
    parsed = parsed + std::to_string(h%24) + "h";
  if(! parsed.empty() || m > 0)
    parsed = parsed + std::to_string(m%60) + "m";

  parsed = parsed + std::to_string(s%60) + ".";
  parsed = parsed + std::to_string(milliseconds%1000) + "s";

  return parsed;
}

int main(int argc, char** argv) {

  static struct tms st_cpu, en_cpu;
  struct timespec ts1, tw1, ts2, tw2;

  times(&st_cpu);
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts1);
  clock_gettime(CLOCK_REALTIME, &tw1);

  std::vector<std::string> args(argv, argv + argc);
  if (args.size() < 2) {
    exit(EXIT_FAILURE);
  }

  char invoke_command[50 * MAX_STRING];
  for(auto& s : std::vector<char*>(argv, argv+argc)) {
    sprintf(invoke_command, "%s%s ", invoke_command, s);
  }
  sprintf(invoke_command, "%s\nThreads possible:\t%d", invoke_command, std::thread::hardware_concurrency());

  std::string command(args[1]);
  if (command == "eval_only") {
    call_eval_only(args, invoke_command);
  }
  else if (command == "parse_eval") {
    call_parse_eval(args, invoke_command);
  }
  else if (command == "skipgram") {
    train(args, invoke_command);
  } else if (command == "skipgram_all") {
    train_all(args, invoke_command);
  } else if (command == "multiskip") {
    call_multiskip(args, invoke_command);
  } else if (command == "oov_lookup") {
    call_oov_lookup(args, invoke_command);
  }

  times(&en_cpu);
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts2);
  clock_gettime(CLOCK_REALTIME, &tw2);

  auto real_dur = intmax_t(1000.0*tw2.tv_sec + 1e-6*tw2.tv_nsec - (1000.0*tw1.tv_sec + 1e-6*tw1.tv_nsec));
  auto user_dur = intmax_t(1000.0*ts2.tv_sec + 1e-6*ts2.tv_nsec - (1000.0*ts1.tv_sec + 1e-6*ts1.tv_nsec));
  auto sys_dur = intmax_t(en_cpu.tms_stime - st_cpu.tms_stime) * 10;

  if(command != "parse_eval") {
    std::cerr << "\n\nreal\t" << parse_tim(real_dur) << "\t"
              << "user\t" << parse_tim(user_dur) << "\t"
              << "sys\t" << parse_tim(sys_dur) << std::endl;
  }

  freopen ("/dev/tty", "a", stdout);
  freopen ("/dev/tty", "a", stderr);

  return 0;
}
