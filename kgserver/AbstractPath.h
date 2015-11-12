//
// Created by Baoxu Shi on 11/12/15.
//

#ifndef KGMINER_ABSTRACTPATH_H
#define KGMINER_ABSTRACTPATH_H

#include <string>

namespace KGMiner {
  using namespace std;
  class AbstractPath {
  public:

      /*! \brief print path
       *
       */
      virtual string str() const = 0;

      /*! \brief append (edge, vertex) pair to path
       *
       */
      virtual bool append(unsigned int edge, unsigned int vertex) = 0;

      /*! \brief check if an vertex is in the path
       *
       */
      virtual inline bool exists(unsigned int vertex) const = 0;

      virtual unsigned int length() const = 0;

  };
}


#endif //KGMINER_ABSTRACTPATH_H
