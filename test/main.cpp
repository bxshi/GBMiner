#include <iostream>

#include <gtest/gtest.h>

using namespace std;

GTEST_API_ int main(int argc, char **argv) {
  cout << "KGServer Tests" << endl;

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();

}
