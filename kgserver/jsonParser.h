//
// Created by Baoxu Shi on 12/15/15.
//

#ifndef KGMINER_JSONCOMMANDPARSER_H
#define KGMINER_JSONCOMMANDPARSER_H

#include "AbstractParser.h"
#include <Logger.h>


namespace KGMiner {
  class jsonParser : AbstractParser {
  private:
      Logger logger;
  public:
      jsonParser();

      Document parse(string jsonStr) const;

      COMMAND_TYPE command(const Document &d) const;
  };
}


#endif //KGMINER_JSONCOMMANDPARSER_H
