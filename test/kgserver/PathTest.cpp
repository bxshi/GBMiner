//
// Created by Baoxu Shi on 11/12/15.
//

#include <gtest/gtest.h>

#include "Path.h"

TEST(MetaPath, append) {
  using namespace KGMiner;

  Path metaPath(12u);

  EXPECT_EQ(metaPath.length(), 0);

  EXPECT_EQ(metaPath.append(1,1), true);
  EXPECT_EQ(metaPath.append(2,1), false);

  EXPECT_EQ(metaPath.length(), 1);
}

TEST(MetaPath, exists) {
  using namespace KGMiner;

  Path metaPath(3u);

  EXPECT_EQ(metaPath.append(1,1), true);

  EXPECT_EQ(metaPath.exists(1), true);
  EXPECT_EQ(metaPath.exists(2), false);
}

TEST(MetaPath, str) {
  using namespace KGMiner;

  Path metaPath(9u);

  EXPECT_EQ(metaPath.append(1, 1), true);
  EXPECT_EQ(metaPath.append(1, true, 2), true);

  EXPECT_EQ(metaPath.length(), 2);

  EXPECT_EQ(metaPath.str(), "9-(1)->1-(-1)->2\n");


}

