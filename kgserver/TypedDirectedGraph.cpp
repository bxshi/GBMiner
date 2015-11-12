//
// Created by Baoxu Shi on 11/11/15.
//

#include "TypedDirectedGraph.h"

namespace KGMiner{

  template<class VD, class ED>
  TypedDirectedGraph<VD, ED>::TypedDirectedGraph() : graphProperty(GRAPH_TYPED_DIRECTED) {
    logger.setPrefix("[TypedDirectedGraph]\t");
  }

  template<class VD, class ED>
  TypedDirectedGraph<VD, ED>::TypedDirectedGraph(unsigned int approxVertices, unsigned int approxEdges) : graphProperty(GRAPH_TYPED_DIRECTED) {
    vData.reserve(approxVertices);
    eData.reserve(approxEdges);
    directedGraph.reserve(approxVertices);
    directedGraph.reserve(approxVertices);

    logger.setPrefix("[TypedDirectedGraph]\t");
  }

  template<class VD, class ED>
  bool TypedDirectedGraph<VD, ED>::insertVertex(unsigned int id, VD data) {
    // vertex already exists
    if (vertexExists(id)) {
      logger.debug("vertex "+to_string(id)+" already exists");
      return false;
    }

    vData[id] = data;
    graphProperty.setVertices(vData.size());
    // TODO: count vertex types
    return true;
  }

  template<class VD, class ED>
  bool TypedDirectedGraph<VD, ED>::insertEdge(unsigned int src, VD srcData,
                                              unsigned int dst, VD dstData,
                                              ED data, bool insert) {

    // linking to invalid vertices
    if (!insert) {
      if (!vertexExists(src)) {
        logger.debug("vertex "+to_string(src)+" does not exist");
        return false;
      }
      if (!vertexExists(dst)) {
        logger.debug("vertex "+to_string(dst)+" does not exist");
        return false;
      }
    }

    // insert vertices if they are not in the graph
    insertVertex(src, srcData);
    insertVertex(dst, dstData);

    // TODO: we assume that all insert edges do not have duplications, or the duplication are intended

    unsigned int eid = (unsigned int)graphProperty.getEdges();
    if (eData.size() <= eid) eData.resize(eid + 1);

    eData[eid] = data;

    if (directedGraph.find(src) == directedGraph.end()) {
      directedGraph[src] = unordered_map<unsigned int, vector<unsigned int>>();
    }

    if (directedGraph[src].find(dst) == directedGraph[src].end()) {
      directedGraph[src][dst] = vector<unsigned int>();
    }

    directedGraph[src][dst].push_back(eid); // insert this edge

    if (reversedGraph.find(dst) == reversedGraph.end()) {
      reversedGraph[dst] = unordered_map<unsigned int, vector<unsigned int>>();
    }

    if (reversedGraph[dst].find(src) == reversedGraph[dst].end()) {
      reversedGraph[dst][src] = vector<unsigned int>();
    }

    reversedGraph[dst][src].push_back(eid);

    graphProperty.setEdges(eid + 1); // eid starts at 0

    //TODO: count edge types

    return true;

  }

  template<class VD, class ED>
  bool TypedDirectedGraph<VD, ED>::vertexExists(unsigned int id) {
    return vData.find(id) != vData.end();
  }

  template<class VD, class ED>
  bool TypedDirectedGraph<VD, ED>::insertVertices(vector<unsigned int> idVec,
                                                  vector<VD> dataVec) {
    if (idVec.size() != dataVec.size()) return false;

    bool res = true;
    for(auto i = 0; i < idVec.size(); ++i) {
      if (!insertVertex(idVec[i], dataVec[i])) {
        logger.debug("vertex "+to_string(idVec[i])+" already exists");
        res = false;
      }
    }
    return res;
  }

  template<class VD, class ED>
  bool TypedDirectedGraph<VD, ED>::insertEdges(vector<unsigned int> srcVec,
                                       vector<unsigned int> dstVec,
                                       vector<ED> data) {
    if (!(srcVec.size() == dstVec.size() && srcVec.size() == data.size())) {
      return false;
    }

    bool res = true;
    for(auto i = 0; i < data.size(); ++i) {
      if (!insertEdge(srcVec[i], VD(), dstVec[i], ED(), data[i], false)) res = false;
    }

    return res;

  }

  template<class VD, class ED>
  const GraphProperty &TypedDirectedGraph<VD, ED>::getProperties() {
    return graphProperty;
  }

  template<class VD, class ED>
  string TypedDirectedGraph<VD, ED>::str() {
    ostringstream oss;
    for(auto src : directedGraph) {
      for(auto dst : src.second) {
        for(auto eid : dst.second) {
          oss << src.first << "-(" << eData[eid] << ")->" << dst.first << "\n";
        }
      }
    }
    return oss.str();
  }


  template<class VD, class ED>
  vector<deque<unsigned int>> TypedDirectedGraph<VD, ED>::cartesianProduct(
      const vector<deque<unsigned int>> &lhs, const vector<deque<unsigned int>> &rhs) const {

    vector<deque<unsigned int>> vertexPaths;

    for(auto lpath : lhs) {
      for(auto rpath : rhs) {
        vertexPaths.push_back(lpath);
        for(auto rvertex : rpath) {
          vertexPaths.back().push_back(rvertex);
        }
      }
    }

    return vertexPaths;
  }

  template<class VD, class ED>
  vector<Path> TypedDirectedGraph<VD, ED>::getPathsBetween(unsigned int src, unsigned int dst, unsigned int maxLength,
                                                           const unordered_set<unsigned int> &vertexMask,
                                                           const unordered_set<ED> &edgeMask) {
    vector<Path> pathVec;

    // we assume that there is no self-loop
    if (src == dst) return pathVec;

    if (vertexMask.find(src) != vertexMask.end() || vertexMask.find(dst) != vertexMask.end()) {
      return pathVec; // src and dst are masked
    }


    vector<unsigned int> vertexPath = {src};

    function<void()> helper = [&]() {

      if (vertexPath.size() > maxLength + 1) return;

      if (vertexPath.back() == dst) {
        auto expandedPaths = expandVertexPath(vertexPath, edgeMask);
        pathVec.insert(pathVec.end(), expandedPaths.begin(), expandedPaths.end());
        return;
      }

      auto neighbors = directedGraph.find(vertexPath.back());

      // visited set for current level
      unordered_set<unsigned int> visitedSet;

      visitedSet.insert(vertexPath.begin(), vertexPath.end());

      if (neighbors != directedGraph.end()) {
        for(auto neighbor : neighbors->second) {
          // not masked
          if (vertexMask.find(neighbor.first) == vertexMask.end() && visitedSet.find(neighbor.first) == visitedSet.end()) {
            vertexPath.push_back(neighbor.first);
            helper();
            vertexPath.pop_back();
            visitedSet.insert(neighbor.first);
          }
        }
      }

      neighbors = reversedGraph.find(vertexPath.back());

      if (neighbors != reversedGraph.end()) {
        for(auto neighbor : neighbors->second) {
          if (vertexMask.find(neighbor.first) == vertexMask.end() &&
              visitedSet.find(neighbor.first) == visitedSet.end()) {
            vertexPath.push_back(neighbor.first);
            helper();
            vertexPath.pop_back();
            visitedSet.insert(neighbor.first);
          }
        }
      }

    };

    helper();

    return pathVec;
  }

  template<class VD, class ED>
  vector <Path> TypedDirectedGraph<VD, ED>::expandVertexPath(const vector<unsigned int> &vertexPath,
                                                               const unordered_set <ED> &edgeMask) const {

    deque<Path> paths;
    for(auto vertex : vertexPath) {
      if (paths.size() == 0) {
        paths.push_back(Path(vertex));
      } else {
        // still have old paths
        while(paths.size() > 0 && paths.front().last() != vertex) {
          Path p = paths.front();
          paths.pop_front();

          auto neighbors = directedGraph.find(vertex);
          auto rneighbors = reversedGraph.find(vertex);

          if (neighbors != directedGraph.end() &&
              neighbors->second.find(p.last()) != neighbors->second.end()) {
            for(auto edge : neighbors->second.at(p.last())) { // p.last() <-(edge)- vertex
              if (edgeMask.find(eData[edge]) == edgeMask.end()) { // only log unmasked link
                paths.push_back(p);
                paths.back().append(edge, true, vertex);
              }
            }
          }

          if (rneighbors != reversedGraph.end() &&
              rneighbors->second.find(p.last()) != rneighbors->second.end()) {
            for(auto edge : rneighbors->second.at(p.last())) { // p.last() -(edge)-> vertex
              if (edgeMask.find(eData[edge]) == edgeMask.end()) { // only log unmasked link
                paths.push_back(p);
                paths.back().append(edge, false, vertex);
              }
            }
          }

        }

        if (paths.size() == 0) return vector<Path>(); // the path breaks at certain point that there are no valid edges

      }
    }

    return vector<Path>(paths.begin(), paths.end());
  }

  template<class VD, class ED>
  vector <Path> TypedDirectedGraph<VD, ED>::expandVertexPaths(const vector <vector<unsigned int>> &vertexPaths,
                                                                    const unordered_set <ED> &edgeMask) const {

    vector<Path> paths;

    // TODO: optimize this part by eliminating deep copy
    for (auto vertexPath : vertexPaths) {
      auto tmpPaths = expandVertexPath(vertexPath, edgeMask);
      paths.insert(paths.end(), tmpPaths.begin(), tmpPaths.end());
    }

    return paths;
  }

  /* instantations decleared here */
  template class TypedDirectedGraph<int, int>;
}

