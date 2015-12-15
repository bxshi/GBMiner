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

TEST(TypedDirectedGraphTest, insertEdges) {
  using namespace KGMiner;
  TypedDirectedGraph<int, int> graph;

  // insert edge that connects to unexist nodes
  EXPECT_EQ(graph.insertEdge(1,0,1,0,1,false), false);

  // insert edge and node
  EXPECT_EQ(graph.insertEdge(1,0,1,0,1,true), true);

  EXPECT_EQ(graph.getProperties().getVertices(), 1);
  EXPECT_EQ(graph.getProperties().getEdges(), 1);

  EXPECT_EQ(graph.insertEdge(1,1,2,2,3,true), true);
  EXPECT_EQ(graph.getProperties().getVertices(), 2);
  EXPECT_EQ(graph.getProperties().getEdges(), 2);

  vector<unsigned int> srcids = {1,2,3,4,5};
  vector<unsigned int> dstids = {1,2,3,4,5};

  vector<int> ed = {1,2,3,4,5};

  EXPECT_EQ(graph.insertEdges(srcids, dstids, ed), false);
  EXPECT_EQ(graph.getProperties().getVertices(), 2);
  EXPECT_EQ(graph.getProperties().getEdges(), 4); // 1-1-1 and 2-2-2 are inserted

}

TEST(TypedDirectedGraphTest, pathsBetweenTwoNodes) {
  using namespace KGMiner;
  TypedDirectedGraph<int, int> graph;

  vector<unsigned int> vids = {1,2,3,4,5,6};
  vector<int> vdata = {1,2,3,4,5,6};

  EXPECT_EQ(graph.insertVertices(vids, vdata), true);
  EXPECT_EQ(graph.getProperties().getVertices(), 6);

  vector<unsigned int> srcs = {1,1,1,1,1,2,4,2,2,4,5};
  vector<unsigned int> dsts = {2,2,3,3,3,4,2,5,6,6,6};
  vector<int> edata = {1,1,1,2,1,2,2,1,1,1,1};

  EXPECT_EQ(graph.insertEdges(srcs, dsts, edata), true);
  EXPECT_EQ(graph.getProperties().getEdges(), 11);

  unordered_set<unsigned int> vertexMask;
  unordered_set<int> edgeMask;

  //self link
  EXPECT_EQ(graph.getPathsBetween(1,1,3, vertexMask, edgeMask).size(), 0);

  //unexist node
  EXPECT_EQ(graph.getPathsBetween(1,99,3,vertexMask,edgeMask).size(), 0);

  vector<Path> paths = graph.getPathsBetween(1,6,3, vertexMask, edgeMask);
  EXPECT_EQ(paths.size(), 8);

  edgeMask.insert(2);
  paths = graph.getPathsBetween(1,6,3, vertexMask, edgeMask);
  EXPECT_EQ(paths.size(), 4);

  vertexMask.insert(4);
  paths = graph.getPathsBetween(1,6,3, vertexMask, edgeMask);
  EXPECT_EQ(paths.size(), 4);

  vertexMask.insert(5);
  paths = graph.getPathsBetween(1,6,3, vertexMask, edgeMask);
  EXPECT_EQ(paths.size(), 2);

  edgeMask.insert(1);
  paths = graph.getPathsBetween(1,6,3, vertexMask, edgeMask);
  EXPECT_EQ(paths.size(), 0);

}

TEST(TypedDirectedGraphTest, printGraph) {
  using namespace KGMiner;
  TypedDirectedGraph<int, int> graph;

  vector<unsigned int> vids = {1,2,3};
  vector<int> vdata = {1,2,3};

  EXPECT_EQ(graph.insertVertices(vids, vdata), true);
  EXPECT_EQ(graph.getProperties().getVertices(), 3);

  EXPECT_EQ(graph.insertEdge(1, 2, 12), true);
  EXPECT_EQ(graph.insertEdge(1, 3, 13), true);
  EXPECT_EQ(graph.insertEdge(3, 1, 31), true);

  EXPECT_EQ(graph.str(), "3-(31)->1\n1-(12)->2\n1-(13)->3\n");
}