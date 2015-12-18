//
// Created by Baoxu Shi on 12/15/15.
//

#include <boost/array.hpp>
#include "Worker.h"

namespace KGMiner {
  bool Worker::write(boost::asio::local::stream_protocol::socket *socket, std::string str) const {
    bool success = true;
    try {
      size_t len = boost::asio::write(*socket, boost::asio::buffer(str, str.length()), boost::asio::transfer_all());
      if (len != str.length()) {
        logger.error(
            "write error, should write " + to_string(str.length()) + " bytes, actual " + to_string(len) + " bytes.");
        success = false;
      } else {
        logger.info("write " + to_string(str.length()) + " bytes to client.");
        success = true;
      }
      socket->close();
      delete socket;
    } catch (std::exception &exception) {
      logger.error("write error, " + string(exception.what()) + ". Please enlarge timeout and try again.");
      success = false;
    }

    return success;
  }

  rapidjson::Document Worker::read(local::stream_protocol::socket *socket) const {
    boost::array<char, 2048> buf;
    boost::system::error_code error;
    size_t len = boost::asio::read(*socket, boost::asio::buffer(buf), error);

    //TODO: deal with error

    return jParser.parse(string(buf.elems));
  }

  void workerFunc(local::stream_protocol::socket *socket, TypedDirectedGraph<string, string> *g) {

    Worker w;

    Document doc = w.read(socket);

    switch (w.jParser.command(doc)) {
      case AbstractParser::STATS:
        break;
      case AbstractParser::GRAPH:
        break;
      case AbstractParser::CONNECTED:
        break;
      case AbstractParser::IN_NEIGHBOR:
        break;
      case AbstractParser::OUT_NEIGHBOR:
        break;
      case AbstractParser::ONTOLOGY:
        break;
      case AbstractParser::SIBLINGS:
        break;
      case AbstractParser::PATH:
        break;
      case AbstractParser::ADAMIC_ADAR:
        break;
      case AbstractParser::SEMANTIC_PROXIMITY:
        break;
      case AbstractParser::PERSONALIZED_PAGERANK:
        break;
      case AbstractParser::PREFERENTIAL_ATTACHMENT:
        break;
      case AbstractParser::KATZ:
        break;
      case AbstractParser::UNDEFINED:
        break;
    }

  }
}
