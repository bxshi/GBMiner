//
// Created by Baoxu Shi on 12/15/15.
//

#include <boost/array.hpp>
#include <rapidjson/prettywriter.h>
#include "Worker.h"

namespace KGMiner {
  bool Worker::write(boost::asio::local::stream_protocol::socket *socket, std::string str) const {
    return write(socket, str.c_str());
  }

  bool Worker::write(local::stream_protocol::socket *socket, const char *str) const {
    bool success = true;
    try {
      size_t len = boost::asio::write(*socket, boost::asio::buffer(str, strlen(str)), boost::asio::transfer_all());
      if (len != strlen(str)) {
        logger.error(
            "write error, should write " + to_string(strlen(str)) + " bytes, actual " + to_string(len) + " bytes.");
        success = false;
      } else {
        logger.info("write " + to_string(strlen(str)) + " bytes to client.");
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
    char buf[2048] = {0};
    boost::system::error_code error;

    rapidjson::Document doc;

    size_t len = 0;
    try {
      while (1) {
        len += socket->read_some(boost::asio::buffer(buf + len, 2048 - len));
        if (buf[len] == '\0') {
          doc = jParser.parse(string(buf, buf + len));
          break;
        }
        if (len >= 2048) {
          logger.warn(" buffer size is smaller than input command, please enlarge buffer.");
          break;
        }
      }
    } catch (boost::asio::error::misc_errors &err) {
      logger.warn(" get an error while reading command.");
    }

    return doc;
  }

  void workerFunc(local::stream_protocol::socket *socket, TypedDirectedGraph<string, string> *g) {

    try {
      Worker w;

      Document doc = w.read(socket);

      rapidjson::StringBuffer stringBuffer;
      rapidjson::PrettyWriter<rapidjson::StringBuffer> prettyWriter(stringBuffer);
      prettyWriter.SetIndent(' ', 2);
      prettyWriter.StartObject();

      switch (w.jParser.command(doc)) {
        case AbstractParser::STATS:

          prettyWriter.Key("status");
          prettyWriter.String("OK");

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
          prettyWriter.Key("status");
          prettyWriter.String("unsupported operator");
      }

      prettyWriter.EndObject();
      w.write(socket, stringBuffer.GetString());

    } catch (std::exception &exception) {
      std::cerr << exception.what() << std::endl;
    } catch (...) {
      std::cerr << "unknown exception\n";
    }

  }
}
