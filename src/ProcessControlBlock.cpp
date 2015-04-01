#include <ProcessControlBlock.h>

int ProcessControlBlock::nextPID = 0;

ProcessControlBlock::ProcessControlBlock(vector<component>* program):
   m_Cost(0)
{
   m_ProgramCode = program;

   m_PID = ++nextPID;
}

int ProcessControlBlock::GetPID()
{
   return m_PID;
}

vector<component>::iterator ProcessControlBlock::GetBeginIter()
{
   return m_ProgramCode->begin();
}
vector<component>::iterator ProcessControlBlock::GetEndIter()
{
   return m_ProgramCode->end();
}

bool ProcessControlBlock::CompareCost(const ProcessControlBlock &lhs, const ProcessControlBlock &rhs)
{
   if(lhs.m_Cost < rhs.m_Cost)
   {
      return true;
   }
   else
   {
      return false;
   }
}

void ProcessControlBlock::SetCost(const int cost)
{
   m_Cost = cost;
}

int ProcessControlBlock::GetCost()
{
   return m_Cost;
}
