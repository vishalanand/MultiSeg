#
# Copyright (c) 2019-present, Vishal Anand
# All rights reserved.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#

CXX = c++
HEADER_DIR=include
CXXFLAGS = -pthread -w -std=c++14 -march=native -I$(HEADER_DIR)
OBJDIR=bin
GTEST_DIR=$(PWD)/test/build
GTEST=googletest/googletest
PWD_DIR=$(PWD)
OBJS = $(addprefix $(OBJDIR)/, args.o dictionary.o evalparse.o matrix.o vector.o model.o utils.o biskip.o multiskip.o parfasttext.o )

opt: CXXFLAGS += -O3 -funroll-loops
opt: $(OBJDIR)/parfasttext
opt: test_build

coverage: CXXFLAGS += -O0 -fno-inline -fprofile-arcs --coverage
coverage: $(OBJDIR)/parfasttext
coverage: test_build

debug: CXXFLAGS += -g -O0 -fno-inline
debug: $(OBJDIR)/parfasttext
debug: test_build

$(OBJDIR):
	if [ ! -d "$(OBJDIR)" ];then mkdir $(OBJDIR); fi
	if [ ! -d "test/log" ];then mkdir test/log; fi

$(OBJDIR)/args.o: src/args.cc include/args.h
	$(CXX) $(CXXFLAGS) -c src/args.cc -o $@

$(OBJDIR)/dictionary.o: src/dictionary.cc include/dictionary.h $(HEADER_DIR)/args.h
	$(CXX) $(CXXFLAGS) -c -w src/dictionary.cc -o $@

$(OBJDIR)/evalparse.o: src/evalparse.cc $(HEADER_DIR)/evalparse.h
	$(CXX) $(CXXFLAGS) -c src/evalparse.cc -o $@

$(OBJDIR)/matrix.o: src/matrix.cc $(HEADER_DIR)/matrix.h $(HEADER_DIR)/utils.h
	$(CXX) $(CXXFLAGS) -c src/matrix.cc -o $@

$(OBJDIR)/vector.o: src/vector.cc $(HEADER_DIR)/vector.h $(HEADER_DIR)/utils.h
	$(CXX) $(CXXFLAGS) -c src/vector.cc -o $@

$(OBJDIR)/model.o: src/model.cc $(HEADER_DIR)/model.h $(HEADER_DIR)/args.h
	$(CXX) $(CXXFLAGS) -c src/model.cc -o $@

$(OBJDIR)/utils.o: src/utils.cc $(HEADER_DIR)/utils.h
	$(CXX) $(CXXFLAGS) -c src/utils.cc -o $@

$(OBJDIR)/biskip.o: src/biskip.cc $(HEADER_DIR)/biskip.h
	$(CXX) $(CXXFLAGS) -c src/biskip.cc -o $@

$(OBJDIR)/multiskip.o: src/multiskip.cc $(HEADER_DIR)/multiskip.h
	$(CXX) $(CXXFLAGS) -c src/multiskip.cc -o $@

$(OBJDIR)/parfasttext.o: src/parfasttext.cc $(HEADER_DIR)/*.h
	$(CXX) $(CXXFLAGS) -c -w src/parfasttext.cc -o $@

$(OBJDIR)/parfasttext: $(OBJDIR) $(OBJS) src/parfasttext.cc
	$(CXX) $(CXXFLAGS) $(OBJS) -w src/main.cc -o bitext

$(OBJDIR)/libgtest.a:
	if [ ! -d "$(GTEST_DIR)/googletest" ];then mkdir $(GTEST_DIR) && git clone https://github.com/google/googletest.git $(GTEST_DIR)/googletest ; fi
	rm -rf $(GTEST_DIR)/googletest/.git*
	rm -rf $(GTEST_DIR)/googletest/CMakeLists.txt
	cd $(GTEST_DIR)/$(GTEST) && $(CXX) $(CXXFLAGS) -isystem $(GTEST_DIR)/$(GTEST)/include -I$(GTEST_DIR)/$(GTEST) -pthread -c $(GTEST_DIR)/$(GTEST)/src/gtest-all.cc
	ar -rv $(OBJDIR)/libgtest.a $(GTEST_DIR)/$(GTEST)/gtest-all.o

test_log_clean:
	rm -rf test/log/*

test_build: $(OBJDIR)/libgtest.a ./test/test_method.cc ./test/test_main.cc
	g++ -std=c++11 -isystem $(GTEST_DIR)/$(GTEST)/include/ -pthread ./test/test_method.cc ./test/test_main.cc $(OBJDIR)/libgtest.a -w -o test_parfast

#test: test_log_clean test_build
#	time ./test_parfast

test: test_build
	time ./test_parfast

# test_build1: $(OBJDIR)/libgtest.a ./test/test_method.cc ./test/test_main1.cc
# 	g++ -std=c++11 -isystem $(GTEST_DIR)/$(GTEST)/include/ -pthread ./test/test_method.cc ./test/test_main1.cc $(OBJDIR)/libgtest.a -w -o test_parfast1

#test1: test_log_clean test_build1
#	time ./test_parfast1

#clean:
#	rm -rf bin/*.o *.gcno *.gcda bitext test_parfast test_parfast1 bin test/log test/build

# clean:
# 	rm -rf bin/*.o *.gcno *.gcda bitext test_parfast test_parfast1 bin test/build

clean:
	rm -rf bin/*.o *.gcno *.gcda bitext test_parfast bin test/build
