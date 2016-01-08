//
// Created by Baoxu Shi on 11/9/15.
//

#include "SocketServer.h"
#include "Opts.h"

using namespace std;

KGMiner::SocketServer *ss = nullptr;

void signalHandler(int sig) {
  if (ss) {
    ss->tryStop();
  }
}

int main(int argc, const char *argv[]) {

  KGMiner::Logger logger;
  logger.setPrefix("[Main]\t");

  KGMiner::Opts options;
  if (!options.parse(argc, argv)) {
    logger.error("Parameter parsing error, exit.");
    return -1;
  }

  /* Load graph */
  KGMiner::TypedDirectedGraph<string, string> graph;


  /* Start server */
  ss = new KGMiner::SocketServer(options.getNworker());

  signal(SIGABRT, signalHandler);
  signal(SIGTERM, signalHandler);
  signal(SIGINT, signalHandler);

  ss->start("/tmp/kgminer", &graph);


  return 0;
}