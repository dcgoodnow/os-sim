#ifndef __LOGGER_H
#define __LOGGER_H

#include <string>

class Logger
{

   public:
      virtual void print(const std::string& message) = 0;
      virtual void println(const std::string& message) = 0;
      virtual ~Logger(){}
};


#endif // __LOGGER_H
