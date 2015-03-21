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
      void print(std::string message);
      void println(std::string message);
};


#endif // __BOTHLOGGER_H
