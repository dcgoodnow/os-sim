#ifndef __LOGGER_H
#define __LOGGER_H

#include <string>

class Logger
{

   public:
      virtual void print(std::string message) = 0;
      virtual void println(std::string message) = 0;
};

#endif // __LOGGER_H
