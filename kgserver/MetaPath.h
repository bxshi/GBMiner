//
// Created by Baoxu Shi on 11/12/15.
//

#ifndef KGMINER_METAPATH_H
#define KGMINER_METAPATH_H

#include "AbstractPath.h"
#include <vector>

namespace KGMiner {
  using namespace std;

  class MetaPath : AbstractPath {
  private:
      vector<unsigned int> vertices;
      vector<long> edges;

      bool isValid() const;

  public:

      MetaPath(unsigned int src);

      unsigned int length() const;

      bool append(unsigned int edge, unsigned int vertex);

      bool append(unsigned int edge, bool reversed, unsigned int vertex);

      inline bool exists(unsigned int vertex) const;

      string str() const;

  };
}


#endif //KGMINER_METAPATH_H
