//
// Created by Baoxu Shi on 1/4/16.
//

#ifndef KGMINER_OPTS_H
#define KGMINER_OPTS_H

#include <boost/program_options.hpp>

class Opts {
public:
    boost::program_options::options_description desc;

    std::string cmd;
    std::string endpoint = "/tmp/kgminer";

    Opts();

    bool parse(int argc, const char *argv[]);
};


#endif //KGMINER_OPTS_H
