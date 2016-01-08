//
// Created by Baoxu Shi on 11/9/15.
//

#include "Opts.h"

#include <iostream>
#include <boost/asio.hpp>

#include <Logger.h>
#include <rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>


int main(int argc, const char *argv[]) {
  KGMiner::Logger logger;
  logger.setPrefix("[KGClient]\t");
  Opts options;

  if (!options.parse(argc, argv)) {
    logger.error("Parameter parsing error, exit.");
    return -1;
  }

  rapidjson::StringBuffer stringBuffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(stringBuffer);

  writer.StartObject();

  writer.Key("cmd");
  writer.String(options.cmd.c_str());

  writer.EndObject();

  //std::cout << stringBuffer.GetString() << std::endl;
  //std::cout << stringBuffer.GetSize() << std::endl;

  boost::asio::io_service my_service;
  boost::asio::local::stream_protocol::endpoint ep(options.endpoint);
  boost::asio::local::stream_protocol::socket my_socket(my_service);
  my_socket.connect(ep);

  boost::asio::write(my_socket,
                     boost::asio::buffer(stringBuffer.GetString(), stringBuffer.GetSize()),
                     boost::asio::transfer_all());

  logger.trace("command sent");

  char buf[81920];
  boost::system::error_code error;
  size_t len = 0;

  while (1) {
    len += my_socket.read_some(boost::asio::buffer(buf + len, 81920 - len));
    if (buf[len] == '\0') {
      break;
    }
    if (len >= 81920) break;
  }

  //std::cout << "read " << len << " bytes\n";
  logger.debug("read " + std::to_string(len) + " bytes");

  rapidjson::Document doc;
  doc.Parse(buf);

  rapidjson::GenericStringBuffer<rapidjson::UTF8<>> buffer;
  rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF8<> > > writer2(buffer);

  doc.Accept(writer2);

  my_socket.close();

  std::cout << buffer.GetString();

  return 0;

}