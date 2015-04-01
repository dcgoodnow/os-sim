/* OS.cpp
 *
 * Last Modified: Tue 31 Mar 2015 10:22:20 PM PDT
 *
*/
#include <OS.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <MonitorLogger.h>
#include <FileLogger.h>
#include <BothLogger.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

void* IO_OP(void* args);
double time_diff(timeval x , timeval y);

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
   m_Logger(NULL)
{
   m_ConfigFile = configFile;
}

void OS::ReadConfig() throw (ConfigReadException, MalformedConfigException)
{
   //open configuration file
   ifstream config;
   config.open(m_ConfigFile.c_str(), fstream::in);
   if(config.fail())
   {
      config.close();
      throw ConfigReadException();
   }
   
   string temp;

   //discard first line 
   getline(config, temp);
   
   //Discard version/phase descriptor
   getline(config, temp, ':');
   
   if(temp.compare("Version/Phase") != 0)
   {
      config.close();
      throw MalformedConfigException();
   }
   config >> m_Version;

   getline(config, temp, ':');
   
   config >> m_Filename;
   getline(config, temp, ':');
   
   config >> temp;
   if(temp.compare("FIFO") == 0)
   {
      m_ScheduleType = FIFO;
   }
   else if(temp.compare("SJF") == 0)
   {
      m_ScheduleType = SJF;
   }
   else
   {
      config.close();
      throw MalformedConfigException();
   }

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

   //get Log type
   getline(config, temp);
   
   //The log type is unique based on the beginning character of the third word. 
   switch(temp.c_str()[8])
   {
      case 'B':
         getline(config, temp, ':');
         config >> m_LogFile;

         m_Logger = new BothLogger(m_LogFile);
         break;
      case 'M':
         //we don't care about the logfile in this case
         m_Logger = new MonitorLogger();
         break;
      case 'F':
         getline(config, temp, ':');
         config >> m_LogFile;

         m_Logger = new FileLogger(m_LogFile);
         break;
   }
   config.close();

}

OS::~OS()
{
   if(m_Logger != NULL)
   {
      delete m_Logger;
   }
}

/* Sample meta-data program
 * ========================
 *
 *  Start Program Meta-Data Code:
 *  S(start)0; A(start)0; P(run)14; I(hard drive)13; O(hard drive)15;
 *  P(run)12; O(hard drive)11; P(run)5; I(hard drive)12; O(hard drive)12;
 *  P(run)5; O(monitor)10; P(run)12; O(monitor)10; A(end)0; S(end)0.
 *  End Program Meta-Data Code.
*/

void OS::ReadPrograms() throw(MetadataReadException)
{
   ifstream metaFile;
   metaFile.open(m_Filename.c_str(), fstream::in);

   if(metaFile.fail())
   {
      metaFile.close();
      throw MetadataReadException();
   }
   string temp;

   //discard start line
   getline(metaFile, temp);
   component nextComp;

   //Stop condition is when the type is S and operation is end
   do
   {
      vector<component> *nextProc = new vector<component>();
      //each program ends with type A and operation end
      do
      {
         metaFile >> nextComp.type;

         //skip open paren character
         metaFile.get();
         getline(metaFile, nextComp.operation, ')');
         metaFile >> nextComp.cost;

         //skip semicolon
         metaFile.get();
         nextProc->push_back(nextComp);
      } while(!(nextComp.type == 'A' && nextComp.operation == "end"));
      ProcessControlBlock pcb(nextProc);
      m_Programs.push_back(pcb);
   } while(!(nextComp.type == 'S' && nextComp.operation == "end"));
   metaFile.close();
}


void OS::Run(vector<component> &program)
{
   timeval start, now;
   pthread_t ioThread;
   void* status;
   int timeMult, delay;

   //used for building the log messages
   ostringstream message;
   message.setf(ios::fixed, ios::floatfield);
   message.precision(6);

   gettimeofday(&start, NULL);
   m_Logger->println("0.000000 - Simulator program starting"); 
   for(vector<component>::iterator next = program.begin(); next < program.end(); next++)
   {
      gettimeofday(&now, NULL);
      message << time_diff(start, now);
      switch(next->type)
      {
         case 'S':  //O/S Control
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
         case 'A': //Process start/stop
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
         case 'P':  //Processing
            message << " - Process 1: start processing action";
            m_Logger->println(message.str());

            //reset message
            message.str("");
            usleep(m_ProcTime * next->cost * 1000);
            gettimeofday(&now, NULL);
            
            message << time_diff(start, now) << " - Process 1: end processing action";
            break;
         case 'I': //Input
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
            message.str("");
            delay = timeMult * next->cost;

            //perform I/O operation in separate thread
            pthread_create(&ioThread, NULL, IO_OP, (void *)&delay);
            pthread_join(ioThread, &status);
            gettimeofday(&now, NULL);
            
            message << time_diff(start, now) << " - Process 1: ";
            if(next->operation.compare("hard drive") == 0)
            {
               message << "end hard drive input";
            }
            else
            {
               message << "end keyboard input";
            }
            break;
         case 'O':  //Output
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
            message.str("");
            delay = timeMult * next->cost;

            //Perform ouput operation in separate thread
            pthread_create(&ioThread, NULL, IO_OP, (void *)&delay);
            pthread_join(ioThread, &status);
            gettimeofday(&now, NULL);
            
            message << time_diff(start, now) << " - Process 1: ";
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
      message.str("");
   }
   gettimeofday(&now, NULL);
   message << time_diff(start, now) << " - Simulator program ending";
   m_Logger->println(message.str());
}

/*
 * Simple threadable function which sleeps for the amount specified. Uses
 * an int type as the passed argument
*/
void* IO_OP(void* args)
{
   usleep(*(int *)args * 1000);
   pthread_exit(NULL);
}

/*
 * Calculates the difference in seconds (with microsecond precision)between 
 * two timevalue types. 
*/
double time_diff(timeval x , timeval y)
{
   double x_us , y_us , diff;

   //get timeval x in microseconds
   x_us = (double)x.tv_sec*1000000 + (double)x.tv_usec;

   //get timeval y in microseconds
   y_us = (double)y.tv_sec*1000000 + (double)y.tv_usec;

   diff = (double)y_us - (double)x_us;

   //Convert back to seconds
   return diff / 1000000;
}

int OS::ComputeCost(vector<component> program)
{
   int cost = 0;
   for(vector<component>::iterator next = program.begin(); next < program.end(); next++)
   {
      switch(next->type)
      {
         case 'S':
         case 'A':
            //these operations have no cost
            break;
         case 'P':
            cost += next->cost * m_ProcTime;
            break;
         case 'I':
            if(next->operation.compare("hard drive") == 0)
            { 
               cost += next->cost * m_HardDriveTime;
            }
            else
            {
               cost += next->cost * m_KeyboardTime;
            }
            break;
         case 'O':
            if(next->operation.compare("hard drive") == 0)
            {
               cost += next->cost * m_HardDriveTime;
            }
            else
            {
               cost += next->cost * m_DisplayTime;
            }
            break;
      }
   }
   return cost;
}
