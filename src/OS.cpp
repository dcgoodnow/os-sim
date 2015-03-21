#include "OS.h"
#include <fstream>
#include <sstream>
#include <MonitorLogger.h>
#include <FileLogger.h>
#include <BothLogger.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

void* IO_OP(void* args);

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

OS::OS(string configFile):
   m_Time(0.000000)
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
         getline(config, temp, ':');
         config >> m_LogFile;
         m_Logger = new BothLogger(m_LogFile);
      case 'M':
         m_Logger = new MonitorLogger();
      case 'F':
         getline(config, temp, ':');
         config >> m_LogFile;
         m_Logger = new FileLogger(m_LogFile);
      default:
         break;
   }
   config.close();
}

OS::~OS()
{
   delete m_Logger;
}

void OS::Run(vector<component> program)
{
   timeval start, now;
   ostringstream message;
   pthread_t ioThread;
   int timeMult, delay;
   gettimeofday(&start, NULL);
   m_Logger->println("0.000000 - Simulator program starting"); 
   for(vector<component>::iterator next = program.begin(); next < program.end(); next++)
   {
      gettimeofday(&now, NULL);
      message << (now.tv_usec - start.tv_usec);
      switch(next->type)
      {
         case 'S':
            message << " - OS: ";
            if(next->operation.compare("start") == 0)
            {
               message << "preparing process 1";
            }
            else
            {
               message << "removing process 1";
            }
            break;
         case 'A':
            message << " - Process 1: ";
            if(next->operation.compare("start") == 0)
            {
               message << "starting process 1";
            }
            else
            {
               message << "ending process 1";
            }
            break;
         case 'P':
            message << " - Process 1: start processing action";
            m_Logger->println(message.str());
            message.flush();
            usleep(m_ProcTime * next->cost * 1000);
            gettimeofday(&now, NULL);
            message << (now.tv_usec - start.tv_usec) << " - Process 1: end processing action";
            break;
         case 'I':
            message << " - Process 1: ";
            if(next->operation.compare("hard drive") == 0)
            {
               message << "start hard drive input";
               timeMult = m_HardDriveTime;
            }
            else
            {
               message << "start keyboard input";
               timeMult = m_KeyboardTime;
            }
            m_Logger->println(message.str());
            message.flush();
            delay = timeMult * next->cost;
            pthread_create(&ioThread, NULL, IO_OP, (void *)&delay);
            gettimeofday(&now, NULL);
            message << (now.tv_usec - start.tv_usec) << " - Process 1: ";
            if(next->operation.compare("hard drive") == 0)
            {
               message << "end hard drive input";
            }
            else
            {
               message << "end keyboard input";
            }
            break;
         case 'O':
            message << " - Process 1: ";
            if(next->operation.compare("hard drive") == 0)
            {
               message << "start hard drive output";
               timeMult = m_HardDriveTime;
            }
            else
            {
               message << "start monitor output";
               timeMult = m_DisplayTime;
            }
            m_Logger->println(message.str());
            message.flush();
            delay = timeMult * next->cost;
            pthread_create(&ioThread, NULL, IO_OP, (void *)&delay);
            gettimeofday(&now, NULL);
            message << (now.tv_usec - start.tv_usec) << " - Process 1: ";
            if(next->operation.compare("hard drive") == 0)
            {
               message << "end hard drive output";
            }
            else
            {
               message << "end monitor output";
            }
            break;
      }
      m_Logger->println(message.str());
      message.flush();
   }
}

void* IO_OP(void* args)
{
   usleep(*(int *)args * 1000);
}
