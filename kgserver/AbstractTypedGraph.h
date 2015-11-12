//
// Created by Baoxu Shi on 11/11/15.
//

#ifndef KGMINER_ABSTRACTAUGMENTEDGRAPH_H
#define KGMINER_ABSTRACTAUGMENTEDGRAPH_H

#include <vector>

#include "GraphProperty.h"

namespace KGMiner {
  using namespace std;

  template <class VD, class ED>
  class AbstractTypedGraph {
  public:

      virtual const GraphProperty& getProperties() = 0;


      virtual bool vertexExists(unsigned int id) = 0;

      /*! \brief Insert an vertex with its data
       *
       * \param id vertex id
       * \param data vertex data
       *
       * \return true if vertex successfully inserted, false if already exists
       */
      virtual bool insertVertex(unsigned int id, VD data) = 0;

      /*! \brief Insert an edge with data
       *
       * \param src source vertex
       * \param dst destination vertex
       * \param data edge data
       * \param insertVertex if true and src or dst does not exist in the graph,
       *                     this will insert src/dst with a default data value
       * \param override override the
       * \return true if edge successfully inserted, false if already exists or connects to invalid vertices
       *
      virtual bool insertEdge(unsigned int src, unsigned int dst,
                              ED data, bool insertVertex = false) = 0;
       */

      /*! \brief Insert an edge with vertices
       *
       * \param src new source vertex
       * \param srcData if src does not exist, execute insertVertex(src, srcData)
       * \param dst new destination vertex
       * \param dstData if dst does not exist, execute insertVertex(dst, dstData)
       * \param data edge data
       * \param insert determines insert src/dst or not if they do not exist
       *
       * \return true if successfully inserted, false if already exists or connects to invalid vertices
       *
       */
      virtual bool insertEdge(unsigned int src, VD srcData,
                              unsigned int dst, VD dstData,
                              ED data, bool insert = false) = 0;

      /*!
       *
       */
      virtual bool insertVertices(vector<unsigned int> idVec,
                                  vector<VD> dataVec) = 0;

      /*!
       *
       */
      virtual bool insertEdges(vector<unsigned int> srcVec,
                               vector<unsigned int> dstVec,
                               vector<ED> data) = 0;
  };
}


#endif //KGMINER_ABSTRACTAUGMENTEDGRAPH_H
