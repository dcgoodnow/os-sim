/*******************************************
 * OS.H
 * Daniel Goodnow
 *
 * Last Modified: Sat 21 Mar 2015 09:19:40 PM PDT
*/
#ifndef __OS_H
#define __OS_H

#include <string>
#include <vector>
#include <Logger.h>

// Structure for meta program actions. 
struct component {
   char type;
   std::string operation;
   int cost;
};

/* OS Class
 * ========
 *
 * This class contains the top level functionality and logic for the OS simulator.
 * It is easily extensible to add more operations to the system. 
 *
*/
class OS{
   private:
      float m_Version;

      //program metadata file
      std::string m_Filename;

      //Processor cycle duration (msec)
      int m_ProcTime;

      //Display cycle duration (msec)
      int m_DisplayTime;

      //Hard Drive cycle duration (msec)
      int m_HardDriveTime;

      //Printer cycle duration (msec)
      int m_PrinterTime;

      //Keyboard cycle duration (msec)
      int m_KeyboardTime;

      //Output method
      Logger* m_Logger;

      //Destination logfile if logging is sent to a file
      std::string m_LogFile;

   public:
      OS(std::string configFile);
      ~OS();

      //Reads a the metadata program file into a vector of components
      void ReadProgram(std::vector<component> &data);

      //Runs the simulator using the given vector of components
      void Run(std::vector<component>&);
};


#endif //__OS_H
