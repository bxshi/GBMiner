//
// Created by Baoxu Shi on 11/11/15.
//

#include "TypedDirectedGraph.h"
#include <fstream>

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
  bool TypedDirectedGraph<VD, ED>::insertEdge(unsigned int src, unsigned int dst, ED data) {
    if (!vertexExists(src)) {
      logger.debug("vertex "+to_string(src)+" does not exist");
      return false;
    }
    if (!vertexExists(dst)) {
      logger.debug("vertex "+to_string(dst)+" does not exist");
      return false;
    }

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

    insertEdge(src, dst, data);

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
  string TypedDirectedGraph<VD, ED>::str() const {
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

  /*! \brief Helper function to check if id is in maskSet.
   *
   */
  inline bool masked(const unordered_set<unsigned int> &maskSet, unsigned int id) {
    return maskSet.find(id) != maskSet.end();
  }

  inline string print(const vector<unsigned int> &vec) {
    ostringstream oss;
    for (auto id : vec) {
      oss << id << " ";
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
    unordered_set<unsigned int> visitedSet = {src}; //TODO: optimize this

    /*! DFS traverse helper function */
    function<void()> helper = [&]() {

      if (vertexPath.size() > maxLength + 1) return; /*< terminate if exceeds the length requirement */

      if (vertexPath.back() == dst) { /* reaches the destination node */
        // logger.debug(print(vertexPath)+"\n");
        auto expandedPaths = expandVertexPath(vertexPath, edgeMask);
        pathVec.insert(pathVec.end(), expandedPaths.begin(), expandedPaths.end());
        return;
      }

      auto neighbors = directedGraph.find(vertexPath.back()); /*< find all out-going nodes */

      // visited set for current level
      unordered_set<unsigned int> levelSet; /*< make sure if we only visited a node once by either neighbors or rneighbors */

      if (neighbors != directedGraph.end()) { /*< if have out-going nodes */
        for(auto neighbor : neighbors->second) {
          // not masked
          if (!masked(vertexMask, neighbor.first) && !masked(visitedSet, neighbor.first) &&
              !masked(levelSet, neighbor.first)) {
            vertexPath.push_back(neighbor.first);
            visitedSet.insert(neighbor.first);
            // logger.trace("try next node "+to_string(neighbor.first)+"\n");
            helper();
            vertexPath.pop_back();
            visitedSet.erase(neighbor.first);
            levelSet.insert(neighbor.first);
          }
        }
      }

      neighbors = reversedGraph.find(vertexPath.back());

      if (neighbors != reversedGraph.end()) { /*< if have in-coming nodes */
        for(auto neighbor : neighbors->second) {
          if (!masked(vertexMask, neighbor.first) && !masked(visitedSet, neighbor.first) &&
              !masked(levelSet, neighbor.first)) {
            vertexPath.push_back(neighbor.first);
            visitedSet.insert(neighbor.first);
            helper();
            vertexPath.pop_back();
            visitedSet.erase(neighbor.first);
            levelSet.insert(neighbor.first);
          }
        }
      }

    };

    helper();

    return pathVec;
  }

  /*! \brief convert entity path into meta path.
   *
   * \param vertexPath entity path
   * \param edgeMask edge types we will discard
   *
   * Note this program returns a vector of Path objects, in which the edges are edgeId instead of edge type.
   * Therefore 1-(2)->3 means node 1 connects to node 3 by edge 2, not by edge type 2. The type of that edge is edgeData[2]
   *
   */
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
          paths.pop_front(); // TODO: optimize performance

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

  template<class VD, class ED>
  void TypedDirectedGraph<VD, ED>::getNodeCluster(unordered_set<unsigned int> &srcCluster, unsigned int maxLength,
                                                  const unordered_set<unsigned int> &vertexMask,
                                                  const unordered_set<ED> &edgeMask) {
    deque<unsigned int> q;

    // check if there is masked vertex in the src cluster
    for (auto vertex : vertexMask) {
      auto v = srcCluster.find(vertex);
      if (v != srcCluster.end()) {
        srcCluster.erase(v);
      }
    }

    q.insert(q.end(), srcCluster.begin(), srcCluster.end());

    while (maxLength > 0 && q.size() > 0) {
      logger.trace("node cluster iteration remains: " + to_string(maxLength));
      --maxLength;

      size_t iterSize = q.size(); // how many nodes in this level
      logger.trace("nodes in this level: " + to_string(iterSize));

      while (iterSize > 0 && q.size() > 0) {
        --iterSize;
        unsigned int vid = q.front();
        q.pop_front();

        auto nextFunc = [&](
            unordered_map<unsigned int, unordered_map<unsigned int, vector<unsigned int>>>::iterator neighbors) {
          if (neighbors != directedGraph.end()) {
            for (auto neighbor : neighbors->second) {

              // make sure neighbor->first has not been visited
              if (srcCluster.find(neighbor.first) != srcCluster.end() ||
                  vertexMask.find(neighbor.first) != vertexMask.end())
                continue;

              //TODO: to speed this up, we need to change the structure of TypedDirectedGraph to id : {id : {rel_type : count}}

              for (auto edge : neighbor.second) {
                if (edgeMask.find(eData[edge]) == edgeMask.end()) {
                  // if connected by a non-masked edge type, add it into queue
                  q.push_back(neighbor.first);
                  srcCluster.insert(neighbor.first);
                  break;
                }
              }
            }
          }
        };

        nextFunc(directedGraph.find(vid));
        nextFunc(reversedGraph.find(vid));
      }

    }


  }

  template<class VD, class ED>
  bool TypedDirectedGraph<VD, ED>::isConnected(unsigned int src, unsigned int dst,
                                               const unordered_set<unsigned int> &vertexMask,
                                               const unordered_set<ED> &edgeMask) {
    return false;
  }

  template<class VD, class ED>
  void TypedDirectedGraph<VD, ED>::load(const string &nodeFlie, const string &edgeFile, const string &edgeTypeFile) {

    // read nodes
    {
      fstream fin(nodeFlie, fstream::in);
      VD vd;
      unsigned int id;

      while (!fin.eof()) {
        fin >> id;
        fin.get();
        fin >> vd;

        insertVertex(id, vd);
      }
      fin.close();
    }

    unordered_map<int, ED> eMap;

    {
      fstream fin(edgeTypeFile, fstream::in);
      unsigned int edge;
      ED ed;


      while (!fin.eof()) {
        fin >> edge;
        fin.get();
        fin >> ed;
        eMap[edge] = ed;
      }
      fin.close();
    }

    {
      fstream fin(edgeFile, fstream::in);

      unsigned int src, dst, eid;

      while (!fin.eof()) {
        fin >> src;
        fin >> dst;
        fin.get();
        fin >> eid;

        insertEdge(src, dst, eMap[eid]);
      }
      fin.close();
    }
  }

  /* instantations decleared here */
  template class TypedDirectedGraph<int, int>;

  template
  class TypedDirectedGraph<string, string>;

}

