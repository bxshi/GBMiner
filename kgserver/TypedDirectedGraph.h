//
// Created by Baoxu Shi on 11/11/15.
//

#ifndef KGMINER_DIRECTEDAUGMENTEDGRAPH_H
#define KGMINER_DIRECTEDAUGMENTEDGRAPH_H

#include <unordered_map>

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

      unordered_map<unsigned int, VD> vData; /*< vertex data indexed by id */
      unordered_map<unsigned int, ED> eData; /*< edge data indexed by edge id */

      vector<edge> edgeLists; /*< edge list */

      /*! the original graph, keys are vertex id and values are edge ids (index in eData) */
      unordered_map<unsigned int, vector<unsigned int> > directedGraph;
      /*! the graph with reversed edges, keys are vertex id and values are edge ids (index in eData) */
      unordered_map<unsigned int, vector<unsigned int> > reversedGraph;

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
                       vector<ED> data,
                       bool insert = false);

      bool insertEdges(vector<vector<unsigned int>> edgeVec,
                       vector<ED> data, bool insert = false);

      bool insertEdges(vector<pair<unsigned int, unsigned int>> edgeVec,
                       vector<ED> data, bool insert = false);
  };
}


#endif //KGMINER_DIRECTEDAUGMENTEDGRAPH_H
