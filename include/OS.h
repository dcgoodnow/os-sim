/*******************************************
 * OS.H
 *
 * Last Modified: Mon 27 Apr 2015 04:34:57 AM PDT
*/
#ifndef __OS_H
#define __OS_H

#include <string>
#include <vector>
#include <queue>
#include <Logger.h>
#include <exception>
#include <Component.h>
#include <ProcessControlBlock.h>


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

struct IO_OPargs
{
   ProcessControlBlock PCB;
   int time;
   pthread_mutex_t *intrptQueueMtx;
   std::queue<ProcessControlBlock> *intrptQueue;
   timeval start;
   Logger *logger;
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

      //Scheduling types
      enum SCHEDULING {FIFO, RR};

      std::string m_ConfigFile;

      float m_Version;

      //program metadata file
      std::string m_Filename;

      //scheduling type
      SCHEDULING m_ScheduleType;

      //Quantum time (cycles)
      int m_Quantum;

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

      //programs to run
      std::vector<ProcessControlBlock> m_Programs;

      //programs to run
      std::vector<ProcessControlBlock> m_ReadyQueue;

      ///IO interrupt queue
      std::queue<ProcessControlBlock> m_InterruptQueue;

      //IO interrupt queue mutex
      pthread_mutex_t m_InterruptQueueMutex;


      /** private functions **/


      bool Interrupted();

      //simulates processing in the cpu
      void Process(ProcessControlBlock&, timeval, int);

      void DoOperation(ProcessControlBlock&, timeval, int);

   public:
      OS(std::string configFile);
      ~OS();

      void ReadConfig() throw (ConfigReadException, MalformedConfigException);

      //Reads a the metadata program file into a vector of components
      void ReadPrograms() throw (MetadataReadException);

      //Runs the simulator using the given vector of components
      void Run();



};


#endif //__OS_H
