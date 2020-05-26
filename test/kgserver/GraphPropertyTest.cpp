//
// Created by Baoxu Shi on 11/11/15.
//

#include <gtest/gtest.h>

#include <GraphProperty.h>

TEST(GraphPropertyTest, construction) {
  using namespace KGMiner;
  GraphProperty graphProperty;

  EXPECT_EQ(graphProperty.getEdges(), 0);
  EXPECT_EQ(graphProperty.getVertexTypes(), 0);
  EXPECT_EQ(graphProperty.getVertices(), 0);
  EXPECT_EQ(graphProperty.getEdgeTypes(), 0);

}

TEST(GraphPropertyTest, modification) {
  using namespace KGMiner;
  GraphProperty graphProperty;

  graphProperty.setVertices(100);
  graphProperty.setEdges(20);
  graphProperty.setVertexTypes(9);
  graphProperty.setEdgeTypes(10);

  EXPECT_EQ(graphProperty.getEdges(), 20);
  EXPECT_EQ(graphProperty.getVertexTypes(), 9);
  EXPECT_EQ(graphProperty.getVertices(), 100);
  EXPECT_EQ(graphProperty.getEdgeTypes(), 10);

  EXPECT_EQ(graphProperty.getGraphType(), GRAPH_DIRECTED);

}

TEST(GraphPropertyTest, print) {
  using namespace KGMiner;
  GraphProperty graphProperty;

  string str = graphProperty.str();

  EXPECT_NE(str, "");

}