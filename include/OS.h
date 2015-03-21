#ifndef __OS_H
#define __OS_H

#include <string>
#include <vector>
#include <Logger.h>

struct component {
   char type;
   std::string operation;
   int cost;
};

class OS{


   private:
      float m_Version;
      std::string m_Filename;
      int m_ProcTime;
      int m_DisplayTime;
      int m_HardDriveTime;
      int m_PrinterTime;
      int m_KeyboardTime;
      Logger* m_Logger;
      std::string m_LogFile;

      float m_Time;

   public:
      OS(std::string configFile);
      ~OS();

      void Run(std::vector<component>);
};


#endif //__OS_H
