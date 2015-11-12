//
// Created by Baoxu Shi on 11/11/15.
//

#ifndef KGMINER_DIRECTEDAUGMENTEDGRAPH_H
#define KGMINER_DIRECTEDAUGMENTEDGRAPH_H

#include <unordered_map>
#include <unordered_set>

#include "AbstractTypedGraph.h"
#include "GraphProperty.h"
#include <Logger.h>

namespace KGMiner {
  using namespace std;

  template <class VD, class ED>
  class TypedDirectedGraph : AbstractTypedGraph<VD, ED> {
  private:

      Logger logger;

      struct edge {
          unsigned int src;
          unsigned int dst;
          unsigned int eid;

          edge(unsigned int src, unsigned int dst, unsigned int eid) : src(src), dst(dst), eid(eid) {}
      };

      GraphProperty graphProperty;

      unordered_set<VD> vtypes; /*< set of unique vertex types */
      unordered_set<ED> etypes; /*< set of unique edge types */

      unordered_map<unsigned int, VD> vData; /*< vertex data indexed by id */
      vector<ED> eData; /*< edge data indexed by edge id */

      /*! the original graph, keys are src and values are dicts with dst as key and edge ids (index in eData) as values */
      unordered_map<unsigned int, unordered_map<unsigned int, vector<unsigned int>>> directedGraph;
      /*! the graph with reversed edges, keys are vertex id and values are edge ids (index in eData) */
      unordered_map<unsigned int, unordered_map<unsigned int, vector<unsigned int>>> reversedGraph;

  public:
      TypedDirectedGraph();

      /*! \brief Constructor with graph size approximations
       *
       * \param approxVertices approximation of vertex numbers
       * \param approxEdges approximation of edge numbers
       */
      TypedDirectedGraph(unsigned int approxVertices, unsigned int approxEdges);

      const GraphProperty& getProperties();

      bool vertexExists(unsigned int id);

      bool insertVertex(unsigned int id, VD data);

      bool insertEdge(unsigned int src, VD srcData,
                      unsigned int dst, VD dstData,
                      ED data, bool insert = false);

      bool insertVertices(vector<unsigned int> idVec,
                          vector<VD> dataVec);

      bool insertEdges(vector<unsigned int> srcVec,
                       vector<unsigned int> dstVec,
                       vector<ED> data);

      string str();
  };
}


#endif //KGMINER_DIRECTEDAUGMENTEDGRAPH_H
