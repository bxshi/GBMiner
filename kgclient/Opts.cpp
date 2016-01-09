//
// Created by Baoxu Shi on 1/4/16.
//

#include "Opts.h"
#include <iostream>

bool Opts::parse(int argc, const char *argv[]) {
  try {
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv)
                                      .options(Opts::desc).run(), vm);

    if (vm.count("help")) {
      desc.print(std::cout);
      return false;
    }

    if (vm.count("cmd")) {
      cmd = vm["cmd"].as<std::string>();
    }

  } catch (std::exception &err) {
    std::cerr << err.what() << std::endl;
    return false;
  } catch (...) {
    std::cerr << "unknown error" << std::endl;
    return false;
  }

  return true;
}

Opts::Opts() {
  desc.add_options()
      ("help,h", "display help message")
      ("endpoint", boost::program_options::value<std::string>(), "socket endpoint, default is /tmp/kgminer")
      ("cmd,c", boost::program_options::value<std::string>()->required(), "command");
}
