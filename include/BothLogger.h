/* BothLogger.h
 *
 * Last Modified: Sat 25 Apr 2015 02:09:31 AM PDT
 *
*/

#ifndef __BOTHLOGGER_H
#define __BOTHLOGGER_H

#include <Logger.h>
#include <FileLogger.h>

/* BothLogger
 * ==========
 *
 * This is an implementation of the Logger class that outputs log messages to
 * both the standard output stream as well as a file.
*/
class BothLogger : public Logger
{
   private:
      FileLogger m_FileLogger;

   public:

      //file is the file to be logged to
      BothLogger(const std::string &file);

      //prints a message to the outputs
      void print(const std::string& message);

      //prints a message to the outputs and appends a newline (\n)
      void println(const std::string& message);
};


#endif // __BOTHLOGGER_H
