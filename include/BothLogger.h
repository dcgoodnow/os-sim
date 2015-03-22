#ifndef __BOTHLOGGER_H
#define __BOTHLOGGER_H

#include <Logger.h>
#include <FileLogger.h>

class BothLogger : public Logger
{
   private:
      FileLogger m_FileLogger;

   public:
      BothLogger(const std::string &file);
      void print(const std::string& message);
      void println(const std::string& message);
};


#endif // __BOTHLOGGER_H
