/* ProcessControlBlock.h
 *
 * Last Modified: Mon 30 Mar 2015 07:21:57 PM PDT
*/

#ifndef __PROCESS_CONTROL_BLOCK_H
#define __PROCESS_CONTROL_BLOCK_H

#include <vector>
#include <OS.h>

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
};


#endif // __PROCESS_CONTROL_BLOCK_H
