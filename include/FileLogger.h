#ifndef __FILELOGGER_H
#define __FILELOGGER_H

#include <Logger.h>
#include <fstream>

class FileLogger : public Logger
{
   private:
      std::string m_FileName;
      std::ofstream m_Output;

   public:
      FileLogger(const std::string &file);
      ~FileLogger();

      void print(std::string message);
      void println(std::string message);
};


#endif // __FILELOGGER_H
