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
    logger.trace("try to write "+to_string(strlen(str))+" bytes to client");
    try {
      size_t len = boost::asio::write(*socket, boost::asio::buffer(str, strlen(str)), boost::asio::transfer_all());
      if (len != strlen(str)) {
        logger.error(
            "write error, should write " + to_string(strlen(str)) + " bytes, actual " + to_string(len) + " bytes.");
        success = false;
      } else {
        logger.trace("write " + to_string(strlen(str)) + " bytes to client.");
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
  void workerFunc(local::stream_protocol::socket *socket, TypedDirectedGraph<string, string> *g, string const writerType) {

    try {
      Worker w;

      Document doc = w.read(socket); //< read input command from socket


      // json writers
      rapidjson::StringBuffer prettyBuffer;
      rapidjson::PrettyWriter<rapidjson::StringBuffer> prettyWriter(prettyBuffer);
      prettyWriter.SetIndent(' ', 2);

      rapidjson::StringBuffer denseBuffer;
      rapidjson::Writer<rapidjson::StringBuffer> denseWriter(denseBuffer);


      Document returnJson;
      returnJson.SetObject();

      switch (w.jParser.command(doc)) {
        case AbstractParser::STATS:

          //TODO: change this to return real status of the server
          returnJson.AddMember("status", "ok", returnJson.GetAllocator());
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

          /**
           * Input format:
           * {
           *  node : an unsigned integer or an array of unsigned integers,
           *  length : an unsigned integer,
           *  [vMask : an unsigned integer or an array of unsigned integers],
           *  [eMask : a string or an array of strings]
           * }
           *
           * node are the starting nodes
           * length is the max radius
           * vMask is the vertex mask set (optional)
           * eMask is the edge type mask set (optional)
           *
           * Example:
           * { node : 1, vMask : [2,5], eMask : ["capital", "locate"], length : 3}
           * get all nodes that can be reached from node `1` within 3 hops, ignore nodes 2 and 5, and edges with type "capital" or "locate"
           *
           */

          if (!doc.HasMember("node") || !(doc["node"].IsUint() || doc["node"].IsArray())) {
            returnJson.AddMember("status", "Do not have valid source node(s)", returnJson.GetAllocator());
            break;
          }

          if (!doc.HasMember("length") || !doc["length"].IsUint()) {
            returnJson.AddMember("status", "Do not have valid cluster radius", returnJson.GetAllocator());
            break;
          }

          returnJson.AddMember("status", "ok", returnJson.GetAllocator());

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

          rapidjson::Value clusterArray(rapidjson::kArrayType);
          for (auto id : cluster) {
            clusterArray.PushBack(id, returnJson.GetAllocator());
          }

          returnJson.AddMember("cluster", clusterArray, returnJson.GetAllocator());

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
          returnJson.AddMember("status", "unsupported operator", returnJson.GetAllocator());
      }


      // stringify JSON and return
      if (writerType.compare("pretty") == 0) {
        returnJson.Accept(prettyWriter);
        w.write(socket, prettyBuffer.GetString());
      } else {
        returnJson.Accept(denseWriter);
        w.write(socket, denseBuffer.GetString());
      }

    } catch (std::exception &exception) {
      std::cerr << exception.what() << std::endl;
    } catch (...) {
      std::cerr << "unknown exception\n";
    }

  }
}
