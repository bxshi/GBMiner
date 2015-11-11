//
// Created by Baoxu Shi on 11/9/15.
//

#ifndef KGMINER_ABSTRACTGRAPH_H
#define KGMINER_ABSTRACTGRAPH_H

#include <string>
#include <vector>

#include "GraphProperty.h"

namespace KGMiner {
  using namespace std;

  /*! Abstract base class of graphs
   *
   */
  class AbstractGraph {

  public:

      virtual const GraphProperty& getProperties() = 0;

      /*! Insert an vertex into graph
       *
       * \param id new vertex id
       *
       * \return false if such vertex already exists
       */
      virtual bool insertVertex(unsigned int id) = 0;

      /*! Insert an edge into graph
       *
       * \param src source vertex
       * \param dst destination vertex
       * \param insertNewVertex if insertNewVertex is true, we insert src and dst if they do not exist
       *
       * \return false if such edge already exists or can not insert the given edge due to missing endpoints
       */
      virtual bool insertEdge(unsigned int src, unsigned int dst,
                              bool insert = false) = 0;

      /*!
       *
       */
      virtual bool insertVertices(vector<unsigned int> idVec) = 0;

      /*!
       *
       */
      virtual bool insertEdges(vector<unsigned int> srcVec,
                               vector<unsigned int> dstVec,
                               bool insert = false) = 0;

      /*!
       *
       */
      virtual bool insertEdges(vector<vector<unsigned int>> edgeVec,
                               bool insert = false) = 0;

      /*!
       *
       */
      virtual bool insertEdges(vector<pair<unsigned int, unsigned int>> edgeVec,
                               bool insert = false) = 0;

  };

}

#endif //KGMINER_ABSTRACTGRAPH_H
