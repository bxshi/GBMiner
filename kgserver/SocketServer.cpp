//
// Created by Baoxu Shi on 12/15/15.
//

#include "SocketServer.h"

namespace KGMiner {
  SocketServer::SocketServer() : nworker(2), isAlive(true) {
    logger.setPrefix("[SocketServer]\t");
    init();
  }

  SocketServer::SocketServer(int nworker) : nworker(nworker), isAlive(true) {
    logger.setPrefix("[SocketServer]\t");
    init();
  }

  void SocketServer::init() {
    work = new boost::asio::io_service::work(worker_io_service);

    for (int i = 0; i < nworker; ++i) {
      threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &worker_io_service));
    }

    logger.info(to_string(nworker) + " workers are initialized");
  }

  void SocketServer::stop() {
    worker_io_service.stop();
    threadpool.join_all();
  }

  void SocketServer::start(string socketName, TypedDirectedGraph<string, string> *g) {
    boost::asio::io_service socket_io_service;

    ::unlink(socketName.c_str());

    local::stream_protocol::endpoint ep(socketName);
    local::stream_protocol::acceptor acceptor(socket_io_service, ep);

    while (isAlive) {
      local::stream_protocol::socket *socket = new local::stream_protocol::socket(socket_io_service);
      acceptor.accept(*socket);
      worker_io_service.post(boost::bind(workerFunc, socket, g));
    }

    logger.info("Stopping server now...");
    stop();
  }

  void SocketServer::tryStop() {
    logger.info("Stop the server after all current requests are served.");
    isAlive = false;
  }
}
