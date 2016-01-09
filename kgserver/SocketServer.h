//
// Created by Baoxu Shi on 12/15/15.
//

#ifndef KGMINER_SOCKETSERVER_H
#define KGMINER_SOCKETSERVER_H

#include <Logger.h>
#include "Worker.h"

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#include <string>
#include <sstream>
#include <vector>

namespace KGMiner {
  namespace local = boost::asio::local;

  class SocketServer {

  private:

      boost::asio::io_service worker_io_service;
      boost::thread_group threadpool;
      boost::asio::io_service::work *work;

      int nworker;
      bool isAlive;

      Logger logger;

      void init();

      void stop();

  public:

      SocketServer();

      SocketServer(int nworker);

      void start(string socketName, TypedDirectedGraph<string, string> *g, string const writerType);

      void tryStop();

  };
}


#endif //KGMINER_SOCKETSERVER_H
