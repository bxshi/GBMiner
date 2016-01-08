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

  /**
   * Main handler of incoming requests.
   */
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

          //TODO: change this to return real status of the server
          prettyWriter.Key("status");
          prettyWriter.String("OK");

          break;
        case AbstractParser::GRAPH:
          //TODO: return graph properties, e.g. number of edges, number of vertices, ontology types, etc,.

          break;
        case AbstractParser::CONNECTED:
          //TODO: return if two nodes are connected, and by what it connected
          break;
        case AbstractParser::IN_NEIGHBOR:
          //TODO: return in-coming neighbors of a given node
          break;
        case AbstractParser::OUT_NEIGHBOR:
          //TODO: return out-going neighbors of a given node
          break;
        case AbstractParser::NODE_CLUSTER: {
          //TODO: return nodes within a certain radius

          prettyWriter.Key("status");
          if (!doc.HasMember("node") || !(doc["node"].IsUint() || doc["node"].IsArray())) {
            prettyWriter.String("Do not have valid source node(s)");
            break;
          }

          if (!doc.HasMember("length") || !doc["length"].IsUint()) {
            prettyWriter.String("Do not have valid cluster radius");
            break;
          }

          prettyWriter.String("OK"); // all required fields are met

          unordered_set<unsigned int> cluster;
          if (doc["node"].IsArray()) {
            const rapidjson::Value &nodes = doc["node"];
            for (rapidjson::SizeType i = 0; i < nodes.Size(); ++i) {
              const rapidjson::Value &node = nodes[i];
              cluster.insert(node.GetUint());
            }
          } else if (doc["node"].IsUint()) {
            cluster.insert(doc["node"].GetUint());
          }

          unordered_set<unsigned int> vertexMask;
          if (doc.HasMember("vMask")) {
            const rapidjson::Value &nodes = doc["vMask"];
            if (nodes.IsArray()) {
              for (rapidjson::SizeType i = 0; i < nodes.Size(); ++i) {
                const rapidjson::Value &node = nodes[i];
                vertexMask.insert(node.GetUint());
              }
            } else if (doc["vMask"].IsUint()) {
              vertexMask.insert(doc["vMask"].GetUint());
            }
          }

          unordered_set<string> edgeMask;
          if (doc.HasMember("eMask")) {
            const rapidjson::Value &nodes = doc["eMask"];
            if (nodes.IsArray()) {
              for (rapidjson::SizeType i = 0; i < nodes.Size(); ++i) {
                const rapidjson::Value &node = nodes[i];
                edgeMask.insert(node.GetString());
              }
            } else {
              edgeMask.insert(doc["eMask"].GetString());
            }
          }

          g->getNodeCluster(cluster, doc["length"].GetUint(), vertexMask, edgeMask);

          prettyWriter.Key("cluster");
          prettyWriter.StartArray();
          for (auto id : cluster) {
            prettyWriter.Uint(id);
          }
          prettyWriter.EndArray();

          break;
        }
        case AbstractParser::ONTOLOGY:
          //TODO: return ontology of a given node
          break;
        case AbstractParser::SIBLINGS:
          //TODO: return nodes that sharing the same neighbors with a given node
          break;
        case AbstractParser::PATH:
          //TODO: return paths between two given nodes
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
