//
// Created by Baoxu Shi on 11/10/15.
//

#include <string>

#include <gtest/gtest.h>

#include <Logger.h>

TEST(LoggerTest, construction) {
  using namespace KGMiner;
  Logger defaultLogger;

  EXPECT_EQ(defaultLogger.getDefaultLevel(), "trace");

  Logger warnLogger("warn");

  EXPECT_EQ(warnLogger.getDefaultLevel(), "warn");

}

TEST(LoggerTest, log) {
  // TODO: Test loggers
}