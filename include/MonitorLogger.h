#ifndef __MONITORLOGGER_H
#define __MONITORLOGGER_H

#include <Logger.h>

class MonitorLogger : public Logger
{
   public:
      void print(std::string message);
      void println(std::string message);
};


#endif // __MONITORLOGGER_H
