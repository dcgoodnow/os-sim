/* ProcessControlBlock.h
 *
 * Last Modified: Wed 01 Apr 2015 12:30:23 AM PDT
*/

#ifndef __PROCESS_CONTROL_BLOCK_H
#define __PROCESS_CONTROL_BLOCK_H

#include <vector>
#include <Component.h>

using namespace std;
class ProcessControlBlock
{
   private:
      vector<component>* m_ProgramCode;
      int m_PID;
      static int nextPID;
      int m_Cost;

      /** Private Functions **/

      //compute cost of a program
      int ComputeCost();


   public:
      ProcessControlBlock(vector<component>*);
      int GetPID();
      vector<component>::iterator GetBeginIter();
      vector<component>::iterator GetEndIter();
      static bool CompareCost(const ProcessControlBlock& lhs, const ProcessControlBlock& rhs);
      void SetCost(const int cost);
      int GetCost();
};


#endif // __PROCESS_CONTROL_BLOCK_H
