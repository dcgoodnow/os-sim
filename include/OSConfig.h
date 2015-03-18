#ifndef __OSCONFIG_H
#define __OSCONFIG_H

#include <string>

class OSConfig{

   enum Log_t { LOG_TO_MONITOR, LOG_TO_FILE, LOG_TO_BOTH };

   private:
      float m_Version;
      std::string m_Filename;
      int m_ProcTime;
      int m_DisplayTime;
      int m_HardDriveTime;
      int m_PrinterTime;
      int m_KeyboardTime;
      Log_t m_LogType;
      std::string m_LogFile;

   public:
      OSConfig(std::string configFile);
      ~OSConfig();
};


#endif //__OSCONFIG_H
