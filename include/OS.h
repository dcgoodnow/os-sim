/*******************************************
 * OS.H
 * Daniel Goodnow
 *
 * Last Modified: Sun 22 Mar 2015 03:56:37 AM PDT
*/
#ifndef __OS_H
#define __OS_H

#include <string>
#include <vector>
#include <Logger.h>
#include <exception>

// Structure for meta program actions. 
struct component {
   char type;
   std::string operation;
   int cost;
};

class ConfigReadException: public std::exception
{
   virtual const char* what() const throw()
   {
      return "Could not read config file";
   }
};

class MalformedConfigException: public std::exception
{
   virtual const char* what() const throw()
   {
      return "Configuration file is malformed";
   }
};

class MetadataReadException: public std::exception
{
   virtual const char* what() const throw()
   {
      return "Metadata file could not be opened";
   }
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
      std::string m_ConfigFile;

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

      void ReadConfig() throw (ConfigReadException, MalformedConfigException);

      //Reads a the metadata program file into a vector of components
      void ReadProgram(std::vector<component> &data) throw (MetadataReadException);

      //Runs the simulator using the given vector of components
      void Run(std::vector<component>&);
};


#endif //__OS_H
