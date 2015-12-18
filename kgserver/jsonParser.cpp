//
// Created by Baoxu Shi on 12/15/15.
//

#include "jsonParser.h"

namespace KGMiner {
  using namespace std;

  rapidjson::Document jsonParser::parse(std::string jsonStr) const {
    Document doc;
    doc.Parse(jsonStr.c_str());
    if (doc.HasParseError()) {
      logger.warn("Can not parse json string " + jsonStr);
      doc.Clear();
    }
    return doc;
  }

  AbstractParser::COMMAND_TYPE jsonParser::command(const rapidjson::Document &d) const {
    if (!d.HasMember("cmd")) return UNDEFINED;

    string cmd = d["cmd"].GetString();

    // transfer cmd to upper

    transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (cmd.compare("stats") == 0) return STATS;
    if (cmd.compare("graph") == 0) return GRAPH;
    if (cmd.compare("connected") == 0) return CONNECTED;
    if (cmd.compare("in_neighbor") == 0) return IN_NEIGHBOR;
    if (cmd.compare("out_neighbor") == 0) return OUT_NEIGHBOR;
    if (cmd.compare("ontology") == 0) return ONTOLOGY;
    if (cmd.compare("siblings") == 0) return SIBLINGS;
    if (cmd.compare("path") == 0) return PATH;
    if (cmd.compare("adamic_adar") == 0) return ADAMIC_ADAR;
    if (cmd.compare("semantic_proximity") == 0) return SEMANTIC_PROXIMITY;
    if (cmd.compare("personalized_pagerank") == 0) return PERSONALIZED_PAGERANK;
    if (cmd.compare("preferential_attachment") == 0) return PREFERENTIAL_ATTACHMENT;
    if (cmd.compare("katz") == 0) return KATZ;

    return UNDEFINED;
  }

  jsonParser::jsonParser() {
    logger.setPrefix("[jsonParser]\t");
  }
}
