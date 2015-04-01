/* ProcessControlBlock.h
 *
 * Last Modified: Tue 31 Mar 2015 10:45:14 PM PDT
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


   public:
      ProcessControlBlock(vector<component>*);
      int GetPID();
      vector<component>::iterator GetBeginIter();
      vector<component>::iterator GetEndIter();
};


#endif // __PROCESS_CONTROL_BLOCK_H
