/* FileLogger.h
 *
 * Last Modified: Sat 21 Mar 2015 09:25:43 PM PDT
 *
*/

#ifndef __FILELOGGER_H
#define __FILELOGGER_H

#include <Logger.h>
#include <fstream>

/* FileLogger
 * ==========
 *
 * An implementation of the Logger class which outputs to the file specified
 * in the constructor. 
*/
class FileLogger : public Logger
{
   private:

      //File being written to
      std::string m_FileName;

      //Output stream of the file
      std::ofstream m_Output;

   public:
      FileLogger(const std::string &file);

      //Destructor is necessary to close the file
      ~FileLogger();

      //Prints the message to the file
      void print(const std::string& message);

      //Prints the message to the file and appends a newline character (\n)
      void println(const std::string& message);
};


#endif // __FILELOGGER_H
