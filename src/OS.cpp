/* OS.cpp
 *
 * Last Modified: Mon 27 Apr 2015 04:52:19 AM PDT
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
   pthread_mutex_init(&m_InterruptQueueMutex, NULL);
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
   //ignore S type
   metaFile.ignore(255, ';');
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

   //add all programs to ready queue
   for(vector<ProcessControlBlock>::iterator program = m_Programs.begin(); program < m_Programs.end(); program++)
   {
      program->SetState(READY);
      m_ReadyQueue.push_back(*program);
   }


   while(!m_Programs.empty())
   {
      if(!m_ReadyQueue.empty())
      {
         gettimeofday(&now, NULL);
         message << time_diff(start, now) << " - OS: selecting next process";
         m_Logger->println(message.str());
         message.str("");
         if(m_ScheduleType == FIFO)
         {
            sort(m_ReadyQueue.begin(), m_ReadyQueue.end(), ProcessControlBlock::ComparePID);
         }

         //'pop' next process and perform its operation
         currentPCB = m_ReadyQueue.front();
         m_ReadyQueue.erase(m_ReadyQueue.begin());
         DoOperation(currentPCB, start, m_Quantum);
      }
      //if all programs are blocked or finished (ready queue empty) we need to check here for interrupts
      else if(Interrupted())
      {
         //handle interrupts
         pthread_mutex_lock(&m_InterruptQueueMutex);
         while(!m_InterruptQueue.empty())
         {
            //move program from interrupt queue to ready queue
            m_ReadyQueue.push_back( m_InterruptQueue.front());
            m_InterruptQueue.pop();
         }
         pthread_mutex_unlock(&m_InterruptQueueMutex);
      }
   }
   gettimeofday(&now, NULL);
   message << time_diff(start, now) << " - Simulator program ending";
   m_Logger->println(message.str());
}


/*
 * Simple threadable function which sleeps for the amount specified. Uses
 * an int type as the passed argument
*/
void* IO_OP(void* arg)
{
   IO_OPargs* args = (IO_OPargs*)arg;
   timeval start, now;
   component comp;
   args->PCB.GetCurrentComponent(comp);
   gettimeofday(&start, NULL);

   //simulate i/o time
   do
   {
      gettimeofday(&now, NULL);
   }while(time_diff(start, now) < (double)args->time/1000);

   ostringstream message;
   message.setf(ios::fixed, ios::floatfield);
   message.precision(6);

   gettimeofday(&now, NULL);
   message << time_diff(args->start, now) << " - Process " << args->PCB.GetPID() << ": end ";
   if(comp.type == 'i')  //input
   {
      message << comp.operation << " input";
   }
   else //output
   {
      message << comp.operation << " output";
   }
   args->logger->println(message.str());

   //'send' interrupt
   pthread_mutex_lock(args->intrptQueueMtx);
   args->PCB.SetState(READY);
   args->PCB.IncrementCounter();
   args->intrptQueue->push((args->PCB));
   pthread_mutex_unlock(args->intrptQueueMtx);
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

void OS::Process(ProcessControlBlock &pcb, timeval start, int quantum)
{
   int cycles = 0;
   component *comp = pcb.GetCurrentComponentPtr();
   timeval now;
   //used for building the log messages
   ostringstream message;
   message.setf(ios::fixed, ios::floatfield);
   message.precision(6);

   do
   {
      usleep(m_ProcTime * 1000);
      cycles ++;

   //Check for an interrupt, going over the allowed quantum time, and if we're finished 
   //processing this component
   }while(!Interrupted() && cycles < quantum && cycles < comp->cost);

   gettimeofday(&now, NULL);
   message << time_diff(start, now) << " - Process " << pcb.GetPID() << ": end processing action";
   m_Logger->println(message.str());

   if(Interrupted())
   {
      //handle interrupts
      pthread_mutex_lock(&m_InterruptQueueMutex);
      while(!m_InterruptQueue.empty())
      {
         m_ReadyQueue.push_back( m_InterruptQueue.front());
         m_InterruptQueue.pop();
      }
      pthread_mutex_unlock(&m_InterruptQueueMutex);
   }

   //subtract the cycles completed from the cost
   comp->cost -= cycles;

   if(comp->cost > 0) //not done processing
   {
      pcb.SetState(READY);
      m_ReadyQueue.push_back(pcb);
   }
   else
   {
      pcb.IncrementCounter();
      if(cycles < quantum) //done processing and still have cycles left
      {
         DoOperation(pcb, start, quantum - cycles);
      }
      else
      {
         pcb.SetState(READY);
         m_ReadyQueue.push_back(pcb);
      }
   }
}

//checks for interrupts
bool OS::Interrupted()
{
   bool interrupted = false;
   pthread_mutex_lock(&m_InterruptQueueMutex);
   interrupted = !m_InterruptQueue.empty();
   pthread_mutex_unlock(&m_InterruptQueueMutex);
   return interrupted;

}

void OS::DoOperation(ProcessControlBlock &pcb, timeval start, int quantum)
{
   timeval  now;
   int timeMult;
   pthread_t ioThread;
   component comp;
   pcb.GetCurrentComponent(comp);   
   IO_OPargs *ioArgs;

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
            pcb.SetState(READY);
            pcb.IncrementCounter();
            m_ReadyQueue.push_back(pcb);
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
            pcb.SetState(READY);
            pcb.IncrementCounter();
            m_ReadyQueue.push_back(pcb);
         }
         else
         {
            message << "removing process " << pcb.GetPID();
            pcb.SetState(EXIT);

            //get index of program to remvoe
            vector<ProcessControlBlock>::iterator removeIndex = m_Programs.begin();
            while(removeIndex->GetPID() != pcb.GetPID()) {removeIndex++;}

            //remove it
            m_Programs.erase(removeIndex);
         }
         m_Logger->println(message.str());
         break;
      case 'P':  //Processing
         message << " - Process " << pcb.GetPID() << ": start processing action";
         m_Logger->println(message.str());

         //reset message
         message.str("");
         pcb.SetState(RUNNING);
         Process(pcb, start, quantum);
         gettimeofday(&now, NULL);
         
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
         ioArgs = new IO_OPargs;
         ioArgs->start = start;
         ioArgs->logger = m_Logger;
         ioArgs->intrptQueueMtx = &m_InterruptQueueMutex;
         ioArgs->intrptQueue = &m_InterruptQueue;
         ioArgs->time = timeMult * comp.cost;
         pcb.SetState(BLOCKED);
         ioArgs->PCB = pcb;

         //perform I/O operation in separate thread
         pthread_create(&ioThread, NULL, IO_OP, (void *)ioArgs);
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
         ioArgs = new IO_OPargs;
         ioArgs->start = start;
         ioArgs->logger = m_Logger;
         ioArgs->intrptQueueMtx = &m_InterruptQueueMutex;
         ioArgs->intrptQueue = &m_InterruptQueue;
         ioArgs->time = timeMult * comp.cost;
         pcb.SetState(BLOCKED);
         ioArgs->PCB = pcb;

         //Perform ouput operation in separate thread
         pthread_create(&ioThread, NULL, IO_OP, (void *)ioArgs);
         break;
   }
}
