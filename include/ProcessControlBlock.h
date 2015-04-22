/* ProcessControlBlock.h
 *
 * Last Modified: Wed 22 Apr 2015 01:57:49 AM PDT
*/

#ifndef __PROCESS_CONTROL_BLOCK_H
#define __PROCESS_CONTROL_BLOCK_H

#include <vector>
#include <Component.h>
enum ProcessState { START, READY, RUNNING, BLOCKED, EXIT };

using namespace std;
class ProcessControlBlock
{
   private:

      //metadata commands
      vector<component>* m_ProgramCode;

      //process identifier number
      int m_PID;

      //process IDs are assigned in ascending order, starting at 1
      static int m_nextPID;

      //Cost of running the metadata commands
      int m_Cost;

      //current component
      vector<component>::iterator m_ProgramCounter;

      /** Private Functions **/

      //compute cost of a program
      int ComputeCost();

      //state
      ProcessState m_State;


   public:

      //constructor
      ProcessControlBlock();
      ProcessControlBlock(vector<component>*);
      ProcessControlBlock(const ProcessControlBlock& pcb);
      ProcessControlBlock& operator=(const ProcessControlBlock& pcb);
      ~ProcessControlBlock();
      int GetPID();
      vector<component>::iterator GetBeginIter();
      vector<component>::iterator GetEndIter();
      static bool CompareCost(const ProcessControlBlock& lhs, const ProcessControlBlock& rhs);
      void SetCost(const int cost);
      int GetCost();

      void SetState(ProcessState newState);
      ProcessState GetState();

      void IncrementCounter();

      vector<component>::iterator GetProgramCounter();
};


#endif // __PROCESS_CONTROL_BLOCK_H
