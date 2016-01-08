//
// Created by Baoxu Shi on 12/15/15.
//

#ifndef KGMINER_WORKER_H
#define KGMINER_WORKER_H

#include "jsonParser.h"
#include "TypedDirectedGraph.h"

#include <Logger.h>
#include <document.h>

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>


namespace KGMiner {
  namespace local = boost::asio::local;

  void workerFunc(local::stream_protocol::socket *socket, TypedDirectedGraph<string, string> *g);

  class Worker {
  public:
      Logger logger;

      jsonParser jParser;

      bool write(local::stream_protocol::socket *socket, string str) const;

      bool write(local::stream_protocol::socket *socket, const char *str) const;

      rapidjson::Document read(local::stream_protocol::socket *socket) const;

  };
}


#endif //KGMINER_WORKER_H
