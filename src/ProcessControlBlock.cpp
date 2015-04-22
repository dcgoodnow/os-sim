#include <ProcessControlBlock.h>
#include <iostream>
#include <limits.h>

int ProcessControlBlock::m_nextPID = 0;

ProcessControlBlock::ProcessControlBlock(vector<component>* program):
   m_Cost(0)
{
   m_ProgramCode = program;

   //reset PID counter if it reaches max integer value
   if(m_nextPID > INT_MAX)
   {
      m_nextPID = 0;
   }
   m_PID = ++m_nextPID;

   m_State = START;
}

ProcessControlBlock::~ProcessControlBlock()
{
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

void ProcessControlBlock::SetState(ProcessState newState)
{
   m_State = newState;
}

ProcessState ProcessControlBlock::GetState()
{
   return m_State;
}

void ProcessControlBlock::IncrementCounter()
{
   m_ProgramCounter++;
}
