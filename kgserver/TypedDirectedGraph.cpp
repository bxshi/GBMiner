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
        logger.debug("vertex "+to_string(src)+" already exists");
        return false;
      }
      if (!vertexExists(dst)) {
        logger.debug("vertex "+to_string(dst)+" already exists");
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
      if (insertEdge(srcVec[i], VD(), dstVec[i], ED(), data[i], false)) res = false;
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

  /* instantations decleared here */
  template class TypedDirectedGraph<int, int>;
  template class TypedDirectedGraph<string, string>;

}

