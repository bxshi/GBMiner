//
// Created by Baoxu Shi on 11/11/15.
//

#include "GraphProperty.h"

namespace KGMiner {

  GraphProperty::GraphProperty() : nVertices(0), nEdges(0), nVertexTypes(0),
                                   nEdgeTypes(0), graphType(GRAPH_DIRECTED) { }

  GraphProperty::GraphProperty(GraphType gtype) : nVertices(0), nEdges(0), nVertexTypes(0),
                                                  nEdgeTypes(0), graphType(gtype)  { }

  unsigned long GraphProperty::getVertices() const { return nVertices; }

  unsigned long GraphProperty::getEdges() const { return nEdges; }

  unsigned long GraphProperty::getVertexTypes() const { return nVertexTypes; }

  unsigned long GraphProperty::getEdgeTypes() const { return nEdgeTypes; }

  void GraphProperty::setVertexTypes(unsigned long vtypes) { nVertexTypes = vtypes; }

  void GraphProperty::setEdgeTypes(unsigned long etypes) { nEdgeTypes = etypes; }

  void GraphProperty::setVertices(unsigned long vertices) { nVertices = vertices; }

  void GraphProperty::setEdges(unsigned long edges) { nEdges = edges; }

  GraphType GraphProperty::getGraphType() const { return graphType; }

  string GraphProperty::str() const {

    ostringstream oss;
    oss << "Graph :\t" << (graphType == GRAPH_DIRECTED ? "directed" : "undirected") << endl;
    oss << "Vertices :\t" << getVertices() << endl;
    oss << "Edges :\t" << getEdges() << endl;
    oss << "Vertex types :\t" << getVertexTypes() << endl;
    oss << "Edge types :\t" << getEdgeTypes() << endl;

    return oss.str();
  }
}