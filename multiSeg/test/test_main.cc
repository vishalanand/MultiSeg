/**
 * Copyright (c) 2019-present, Vishal Anand
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <gtest/gtest.h>

int main(int argc, char* argv[]){
  testing::InitGoogleTest(&argc, argv);

  ::testing::GTEST_FLAG(filter) = "BlackBoxTest*";
  RUN_ALL_TESTS();

//  ::testing::GTEST_FLAG(filter) = "BlackBoxMultiTest*";
//  RUN_ALL_TESTS();

  return 0;
}
