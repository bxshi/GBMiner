//
// Created by Baoxu Shi on 12/15/15.
//

#ifndef KGMINER_ABSTRACTCOMMANDPARSER_H
#define KGMINER_ABSTRACTCOMMANDPARSER_H

#include <document.h>
#include <writer.h>
#include <stringbuffer.h>

#include <string>

namespace KGMiner {
  using namespace std;
  using namespace rapidjson;

  class AbstractParser {

  public:

      enum COMMAND_TYPE {
          // basic commands


          UNDEFINED,

          STATS, // server status
          GRAPH, // graph stats
          CONNECTED, // if two entities are connected
          IN_NEIGHBOR, // all nodes that connect to the given query node
          OUT_NEIGHBOR, // all nodes that query node can reach in one hop

          // information network related

          ONTOLOGY, // ontology fo query node
          SIBLINGS, // nodes that partials matches the ontology of a given query node

          // graph algorithms

          PATH, // paths between two given nodes

          ADAMIC_ADAR,
          SEMANTIC_PROXIMITY,
          PERSONALIZED_PAGERANK,
          PREFERENTIAL_ATTACHMENT,
          KATZ
      };

      /*! \brief Parse JSON string and return a document object
       *
       */
      virtual Document parse(string jsonStr) const = 0;

      /*! \brief Return command type for switch-case
       *
       */
      virtual COMMAND_TYPE command(const Document &d) const = 0;

  };
}


#endif //KGMINER_ABSTRACTCOMMANDPARSER_H
