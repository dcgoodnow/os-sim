/* ProcessControlBlock.h
 *
 * Last Modified: Thu 02 Apr 2015 10:19:06 PM PDT
*/

#ifndef __PROCESS_CONTROL_BLOCK_H
#define __PROCESS_CONTROL_BLOCK_H

#include <vector>
#include <Component.h>

using namespace std;
class ProcessControlBlock
{
   private:

      //metadata commands
      vector<component>* m_ProgramCode;

      //process identifier number
      int m_PID;

      //process IDs are assigned in ascending order, starting at 1
      static int nextPID;

      //Cost of running the metadata commands
      int m_Cost;

      //current component
      vector<component>::iterator m_ProgramCounter;

      /** Private Functions **/

      //compute cost of a program
      int ComputeCost();


   public:

      //constructor
      ProcessControlBlock(vector<component>*);
      ~ProcessControlBlock();
      int GetPID();
      vector<component>::iterator GetBeginIter();
      vector<component>::iterator GetEndIter();
      static bool CompareCost(const ProcessControlBlock& lhs, const ProcessControlBlock& rhs);
      void SetCost(const int cost);
      int GetCost();
};


#endif // __PROCESS_CONTROL_BLOCK_H
