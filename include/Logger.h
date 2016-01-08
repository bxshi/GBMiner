//
// Created by Baoxu Shi on 11/9/15.
//

#ifndef KGMINER_LOGGER_H
#define KGMINER_LOGGER_H

#define BOOST_LOG_DYN_LINK 1

#include <string>
#include <algorithm>
#include <sstream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

/*! \brief Wrapper of Boost's logging function
 *
 */
namespace KGMiner {

  using namespace std;
  using namespace boost::log;

  class Logger {
  private:

      enum LOG_LEVEL {
          LOG_LEVEL_TRACE = trivial::trace,
          LOG_LEVEL_DEBUG = trivial::debug,
          LOG_LEVEL_INFO = trivial::info,
          LOG_LEVEL_WARN = trivial::warning,
          LOG_LEVEL_ERROR = trivial::error,
          LOG_LEVEL_FATAL = trivial::fatal
      };

      LOG_LEVEL level;

      /*! Get level enum by string
       *
       * \param strLevel string of log level, can be "trace", "debug", "info", "warn", "error", and "fatal"
       */
      inline LOG_LEVEL getLevel(string strLevel) const {

        transform(strLevel.begin(), strLevel.end(), strLevel.begin(), ::tolower);

        if (strLevel == "trace") {
          return LOG_LEVEL_TRACE;
        } else if (strLevel == "debug") {
          return LOG_LEVEL_DEBUG;
        } else if (strLevel == "info") {
          return LOG_LEVEL_INFO;
        } else if (strLevel == "warn" || strLevel == "warning") {
          return LOG_LEVEL_WARN;
        } else if (strLevel == "error") {
          return LOG_LEVEL_ERROR;
        } else {
          return LOG_LEVEL_FATAL;
        }

      }

      string prefix;

  public:

      Logger() : level(LOG_LEVEL_INFO) {
        core::get()->set_filter(trivial::severity >= (trivial::severity_level) level);
      }

      Logger(LOG_LEVEL level) : level(level) {
        core::get()->set_filter(trivial::severity >= (trivial::severity_level) level);
      }

      Logger(string strLevel) {
        level = getLevel(strLevel);
      }

      inline void setPrefix(string str) {
        prefix = str;
      }

      inline string getPrefix() const {
        return prefix;
      }

      inline void log(string strLevel, string str) const {
        log(getLevel(strLevel), str);
      }

      inline void log(string str) const {
        log(level, str);
      }

      inline void log(LOG_LEVEL logLevel, string str) const {
        switch (logLevel) {
          case LOG_LEVEL_TRACE:
            trace(str);
            break;
          case LOG_LEVEL_DEBUG:
            debug(str);
            break;
          case LOG_LEVEL_INFO:
            info(str);
            break;
          case LOG_LEVEL_WARN:
            warn(str);
            break;
          case LOG_LEVEL_ERROR:
            error(str);
            break;
          case LOG_LEVEL_FATAL:
            fatal(str);
            break;
          default:
            fatal("Unknown log level!\n");
        }
      }

      inline void trace(string str) const {
        ostringstream oss;
        oss << getPrefix() << str;
        BOOST_LOG_TRIVIAL(trace) << oss.str();
      }

      inline void debug(string str) const {
        ostringstream oss;
        oss << getPrefix() << str;
        BOOST_LOG_TRIVIAL(debug) << oss.str();
      }

      inline void info(string str) const {
        ostringstream oss;
        oss << getPrefix() << str;
        BOOST_LOG_TRIVIAL(info) << oss.str();
      }

      inline void warn(string str) const {
        ostringstream oss;
        oss << getPrefix() << str;
        BOOST_LOG_TRIVIAL(warning) << oss.str();
      }

      inline void error(string str) const {
        ostringstream oss;
        oss << getPrefix() << str;
        BOOST_LOG_TRIVIAL(error) << oss.str();
      }

      inline void fatal(string str) const {
        ostringstream oss;
        oss << getPrefix() << str;
        BOOST_LOG_TRIVIAL(fatal) << oss.str();
      }

      inline string getDefaultLevel() const {
        switch(level) {
          case LOG_LEVEL_TRACE:
            return "trace";
          case LOG_LEVEL_DEBUG:
            return "debug";
          case LOG_LEVEL_INFO:
            return "info";
          case LOG_LEVEL_WARN:
            return "warn";
          case LOG_LEVEL_ERROR:
            return "error";
          case LOG_LEVEL_FATAL:
            return "fatal";
          default:
            return "unknown";
        }
      }

  };
}
#endif //KGMINER_LOGGER_H
