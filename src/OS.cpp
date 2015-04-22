/* OS.cpp
 *
 * Last Modified: Wed 22 Apr 2015 03:23:59 AM PDT
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
#include <algorithm>

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
   
   //Discard descriptor
   config.ignore(255, ':');
   
   config >> m_Version;

   config.ignore(255, ':');
   
   config >> m_Filename;
   config.ignore(255, ':');
   
   
   config >> temp;
   if(temp.compare("FIFO-P") == 0)
   {
      m_ScheduleType = FIFO;
   }
   else if(temp.compare("RR") == 0)
   {
      m_ScheduleType = RR;
   }
   else
   {
      config.close();
      throw MalformedConfigException();
   }

   config.ignore(255, ':');
   config >> m_Quantum;

   config.ignore(255, ':');
   config >> m_ProcTime;

   config.ignore(255, ':');
   config >> m_DisplayTime;

   config.ignore(255, ':');
   config >> m_HardDriveTime;

   config.ignore(255, ':');
   config >> m_PrinterTime;

   config.ignore(255, ':');
   config >> m_KeyboardTime;

   getline(config, temp, ':');

   //get Log type
   getline(config, temp);
   
   //The log type is unique based on the beginning character of the third word. 
   switch(temp.c_str()[8])
   {
      case 'B':
         config.ignore(255, ':');
         config >> m_LogFile;

         m_Logger = new BothLogger(m_LogFile);
         break;
      case 'M':
         //we don't care about the logfile in this case
         m_Logger = new MonitorLogger();
         break;
      case 'F':
         config.ignore(255, ':');
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

   //Stop condition for this loop is when the type is S and operation is end
   do
   {
      vector<component> *nextProc = new vector<component>();

      //each program ends with type A and operation end
      do
      {
         metaFile >> nextComp.type;

         //skip open parentheses character
         metaFile.ignore();

         //get operation
         getline(metaFile, nextComp.operation, ')');

         //get operation cost
         metaFile >> nextComp.cost;

         //skip semicolon
         metaFile.ignore();

         nextProc->push_back(nextComp);
      } while(!(nextComp.type == 'A' && nextComp.operation == "end"));

      //create new process control block
      ProcessControlBlock pcb(nextProc);

      m_Programs.push_back(pcb);

      //skip space
      metaFile.ignore();

   } while(metaFile.peek() != 'S');

   metaFile.close();
}


void OS::Run()
{

   timeval start, now;

   //used for building the log messages
   ostringstream message;
   message.setf(ios::fixed, ios::floatfield);
   message.precision(6);

   gettimeofday(&start, NULL);
   m_Logger->println("0.000000 - Simulator program starting"); 
   ProcessControlBlock currentPCB;


   while(!m_Programs.empty())
   {
      switch(m_ScheduleType)
      {
         case FIFO:
            break;
         case RR:
            if(m_ReadyQueue.empty())
            {
               m_ReadyQueue.push_back(m_Programs.front());
               m_Programs.erase(m_Programs.begin());
            }
            currentPCB = m_ReadyQueue.front();
            m_ReadyQueue.erase(m_ReadyQueue.begin());
            DoOperation(currentPCB, start);
            break;
      }
   }
   gettimeofday(&now, NULL);
   message << time_diff(start, now) << " - Simulator program ending";
   m_Logger->println(message.str());
}

int OS::ComputeCost(vector<component>::iterator begin, vector<component>::iterator end)
{
   int cost = 0;
   for(vector<component>::iterator nextComp = begin; nextComp < end; nextComp++)
   {
      switch(nextComp->type)
      {
         case 'S':
         case 'A':
            //these operations have no cost
            break;
         case 'P':
            cost += nextComp->cost * m_ProcTime;
            break;
         case 'I':
            if(nextComp->operation.compare("hard drive") == 0)
            { 
               cost += nextComp->cost * m_HardDriveTime;
            }
            else
            {
               cost += nextComp->cost * m_KeyboardTime;
            }
            break;
         case 'O':
            if(nextComp->operation.compare("hard drive") == 0)
            {
               cost += nextComp->cost * m_HardDriveTime;
            }
            else
            {
               cost += nextComp->cost * m_DisplayTime;
            }
            break;
      }
   }
   return cost;
}

/*
 * Simple threadable function which sleeps for the amount specified. Uses
 * an int type as the passed argument
*/
void* IO_OP(void* arg)
{
   IO_OPargs* args = (IO_OPargs*)arg;
   timeval start, now;
   gettimeofday(&start, NULL);
   do
   {
      gettimeofday(&now, NULL);
   }while(time_diff(start, now) < (double)args->time/1000);
   pthread_mutex_lock(args->IOCompMtx);
   args->PCB->SetState(READY);
   args->PCB->IncrementCounter();
   args->readyQueue->push_back(*(args->PCB));
   *(args->IOComp) = true;
   pthread_mutex_unlock(args->IOCompMtx);
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

void OS::Process(component &comp)
{
   int cycles = 0;
   do
   {
      usleep(m_ProcTime * 1000);
      cycles ++;
   //Check for an interrupt, going over the allowed quantum time, and if we're finished 
   //processing this component
   }while(!Interrupt() && cycles < m_Quantum && cycles < comp.cost);

   if(Interrupt())
   {
      pthread_mutex_lock(&m_IOCompleteMtx);
      m_IOComplete = false;
      pthread_mutex_unlock(&m_IOCompleteMtx);
   }
   //subtract the cycles completed from the cost
   comp.cost -= cycles;
}
bool OS::Interrupt()
{
   bool iocomp;
   pthread_mutex_lock(&m_IOCompleteMtx);
   iocomp = m_IOComplete;
   pthread_mutex_unlock(&m_IOCompleteMtx);
   return iocomp;

}

void OS::DoOperation(ProcessControlBlock pcb, timeval start)
{
   timeval  now;
   int timeMult;
   pthread_t ioThread;
   void* status;
   component comp = *pcb.GetProgramCounter();
   IO_OPargs ioArgs;
   ioArgs.IOCompMtx = &m_IOCompleteMtx;
   ioArgs.IOComp = &m_IOComplete;
   ioArgs.readyQueue = &m_ReadyQueue;

   //used for building the log messages
   ostringstream message;
   message.setf(ios::fixed, ios::floatfield);
   message.precision(6);

   gettimeofday(&now, NULL);
   message << time_diff(start, now);
   switch(comp.type)
   {
      case 'S':  //O/S Control
         message << " - OS: ";
         if(comp.operation.compare("start") == 0)
         {
            message << "preparing process " << pcb.GetPID();
         }
         else
         {
            message << "removing process " << pcb.GetPID();
         }
         break;
      case 'A': //Process start/stop
         message << " - OS: ";
         if(comp.operation.compare("start") == 0)
         {
            message << "starting process " << pcb.GetPID();
         }
         else
         {
            message << "removing process " << pcb.GetPID();
         }
         pcb.SetState(READY);
         pcb.IncrementCounter();
         m_ReadyQueue.push_back(pcb);
         break;
      case 'P':  //Processing
         message << " - Process " << pcb.GetPID() << ": start processing action";
         m_Logger->println(message.str());

         //reset message
         message.str("");
         Process(comp);
         pcb.SetState(READY);
         pcb.IncrementCounter();
         m_ReadyQueue.push_back(pcb);
         gettimeofday(&now, NULL);
         
         message << time_diff(start, now) << " - Process " << pcb.GetPID() << ": end processing action";
         break;
      case 'I': //Input
         message << " - Process " << pcb.GetPID() << ": ";
         if(comp.operation.compare("hard drive") == 0)
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
         ioArgs.time = timeMult * comp.cost;
         ioArgs.PCB = &pcb;

         //perform I/O operation in separate thread
         pthread_create(&ioThread, NULL, IO_OP, (void *)&ioArgs);
         pthread_join(ioThread, &status);
         gettimeofday(&now, NULL);
         
         message << time_diff(start, now) << " - Process " << pcb.GetPID() << ": ";
         if(comp.operation.compare("hard drive") == 0)
         {
            message << "end hard drive input";
         }
         else
         {
            message << "end keyboard input";
         }
         break;
      case 'O':  //Output
         message << " - Process " << pcb.GetPID() << ": ";
         if(comp.operation.compare("hard drive") == 0)
         {
            message << "start hard drive output";
            timeMult = m_HardDriveTime;
         }
         else if(comp.operation.compare("monitor") == 0)
         {
            message << "start monitor output";
            timeMult = m_DisplayTime;
         }
         else
         {
            message << "start printer output";
            timeMult = m_PrinterTime;
         }
         m_Logger->println(message.str());
         message.str("");
         ioArgs.time = timeMult * comp.cost;

         //Perform ouput operation in separate thread
         pthread_create(&ioThread, NULL, IO_OP, (void *)&ioArgs);
         pthread_join(ioThread, &status);
         gettimeofday(&now, NULL);
         
         message << time_diff(start, now) << " - Process " << pcb.GetPID() << ": ";
         if(comp.operation.compare("hard drive") == 0)
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
