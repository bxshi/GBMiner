//
// Created by Baoxu Shi on 11/11/15.
//

#include <gtest/gtest.h>

#include <TypedDirectedGraph.h>

TEST(TypedDirectedGraphTest, construction) {
  using namespace KGMiner;
  TypedDirectedGraph<int, int> graph;

  EXPECT_EQ(graph.vertexExists(1), false);

  TypedDirectedGraph<int, int> graph1(100, 100);

  EXPECT_EQ(graph1.vertexExists(1), false);
}

TEST(TypedDirectedGraphTest, properties) {
  using namespace KGMiner;
  TypedDirectedGraph<int, int> graph;

  EXPECT_EQ(graph.getProperties().getVertices(), 0);
  EXPECT_EQ(graph.getProperties().getEdges(), 0);

}

TEST(TypedDirectedGraphTest, insertVertices) {
  using namespace KGMiner;
  TypedDirectedGraph<int, int> graph;

  EXPECT_EQ(graph.getProperties().getVertices(), 0);
  EXPECT_EQ(graph.getProperties().getEdges(), 0);

  // add new vertex
  graph.insertVertex(998, 2);

  EXPECT_EQ(graph.getProperties().getVertices(), 1);
  EXPECT_EQ(graph.getProperties().getEdges(), 0);

  EXPECT_EQ(graph.vertexExists(998), true);

  // try add existing vertex
  graph.insertVertex(998,3);

  EXPECT_EQ(graph.getProperties().getVertices(), 1);
  EXPECT_EQ(graph.getProperties().getEdges(), 0);

  // add a set of vertex
  vector<unsigned int> vids = {1,2,3,4,5};
  vector<int> vdata = {-1,-2,-3,-4};

  EXPECT_EQ(graph.insertVertices(vids, vdata), false);

  vdata.push_back(-5);

  EXPECT_EQ(graph.insertVertices(vids, vdata), true);

  EXPECT_EQ(graph.getProperties().getVertices(), 6);
  EXPECT_EQ(graph.getProperties().getEdges(), 0);

  // add a set of vertices that contains dup vertices
  vdata.push_back(0);
  vids.push_back(200);

  EXPECT_EQ(graph.insertVertices(vids, vdata), false);

  EXPECT_EQ(graph.getProperties().getVertices(), 7);
  EXPECT_EQ(graph.getProperties().getEdges(), 0);
}