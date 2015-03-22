#ifndef __MONITORLOGGER_H
#define __MONITORLOGGER_H

#include <Logger.h>

class MonitorLogger : public Logger
{
   public:
      void print(const std::string& message);
      void println(const std::string& message);
};


#endif // __MONITORLOGGER_H
