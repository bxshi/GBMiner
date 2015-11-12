//
// Created by Baoxu Shi on 11/12/15.
//

#include <sstream>

#include "MetaPath.h"

namespace KGMiner{
  using namespace std;

  bool MetaPath::isValid() const {
    return (vertices.size() == 0 && edges.size() == vertices.size()) ||
           vertices.size() - 1 == edges.size();
  }

  bool MetaPath::append(unsigned int edge, unsigned int vertex) {
    return append(edge, false, vertex);
  }

  bool MetaPath::append(unsigned int edge, bool reversed, unsigned int vertex) {
    if (exists(vertex)) return false;

    vertices.push_back(vertex);
    edges.push_back(reversed ? ((long)edge) * -1 : edge);

    return true;

  }

  inline bool MetaPath::exists(unsigned int vertex) const {
    for(auto v : vertices) {
      if (v == vertex) return true;
    }
    return false;
  }

  unsigned int MetaPath::length() const {
    return (unsigned int)edges.size();
  }

  string MetaPath::str() const {
    ostringstream oss;
    if (!isValid()) return oss.str();

    int i = 0;
    int j = 0;

    while(i < vertices.size() || j < edges.size()) {
      if (i < vertices.size()) {
        oss << vertices[i++];
      }
      if (j < edges.size()) {
        oss << "-(" << edges[j++] << ")->";
      }
    }

    oss << endl;

    return oss.str();
  }

  MetaPath::MetaPath(unsigned int src) {
    vertices.push_back(src);
  }
}