#include <ProcessControlBlock.h>

int ProcessControlBlock::nextPID = 0;

ProcessControlBlock::ProcessControlBlock(vector<component>* program)
{
   m_ProgramCode = program;

   m_PID = ++nextPID;
}

int ProcessControlBlock::GetPID()
{
   return m_PID;
}
