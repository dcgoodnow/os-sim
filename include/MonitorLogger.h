/* MonitorLogger.h
 *
 * Last Modified: Sat 25 Apr 2015 02:09:22 AM PDT
 *
*/
#ifndef __MONITORLOGGER_H
#define __MONITORLOGGER_H

#include <Logger.h>

/* MonitorLogger
 * =============
 *
 * This is an implementation of the logger class that outputs log messages to 
 * the standard output stream.
*/
class MonitorLogger : public Logger
{
   public:
      MonitorLogger();

      //Prints a message to the standard output stream
      void print(const std::string& message);

      //Prints a message to the standard output stream and appends a newline character (\n)
      void println(const std::string& message);
};


#endif // __MONITORLOGGER_H
