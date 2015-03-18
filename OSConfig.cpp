#include "OSConfig.h"
#include <fstream>

using namespace std;

/* Configuration file spec
 * =======================
 *
 * Start Simulator Configuration File
 * Version/Phase: <number>
 * File Path: <complete file path and name of program file>
 * Processor cycle time (msec/cycle): <time>
 * Monitor display time (msec/cycle): <time>
 * Hard drive cycle time (msec/cycle): <time>
 * Printer cycle time (msec/cycle): <time>
 * Keyboard cycle time (msec/cycle): <time>
 * Log: <Log to Both> OR <Log to Monitor> OR <Log to File>
 * Log File Path: <complete file path and name of log file name>
 * End Simulator Configuration File
 *
*/

OSConfig::OSConfig(string configFile)
{
   ifstream config;
   config.open(configFile.c_str(), fstream::in);
   
   string temp;
   getline(config, temp);
   config >> temp; 
   if(!temp.compare("Version/Phase:"))
   {
      //TODO: Error handling
   }
   config >> m_Version;
   getline(config, temp, ':');
   if(!temp.compare("File Path"))
   {
      //TODO: Error handling
   }
   config >> m_Filename;
   getline(config, temp, ':');
   config >> m_ProcTime;
   getline(config, temp, ':');
   config >> m_DisplayTime;
   getline(config, temp, ':');
   config >> m_HardDriveTime;
   getline(config, temp, ':');
   config >> m_PrinterTime;
   getline(config, temp, ':');
   config >> m_KeyboardTime;
   getline(config, temp, ':');
   config >> temp;
   switch(temp.c_str()[7])
   {
      case 'B':
         m_LogType = LOG_TO_BOTH;
      case 'M':
         m_LogType = LOG_TO_MONITOR;
      case 'F':
         m_LogType = LOG_TO_FILE;
   }
   getline(config, temp, ':');
   config >> m_LogFile;
}
