#include <ProcessControlBlock.h>
#include <limits.h>

//PID of 0 is not an initialized process, just FYI
int ProcessControlBlock::m_nextPID = 0;

ProcessControlBlock::ProcessControlBlock(vector<component>* program):
   m_Cost(0),
   m_State(START)
{
   m_ProgramCode = program;
   m_ProgramCounter = 0;

   //reset PID counter if it reaches max integer value
   if(m_nextPID > INT_MAX)
   {
      m_nextPID = 0;
   }
   m_PID = ++m_nextPID;
}

ProcessControlBlock::ProcessControlBlock():
   m_Cost(0),
   m_State(START)
{
   m_ProgramCode = NULL;

   m_PID = 0;
}
ProcessControlBlock::ProcessControlBlock(const ProcessControlBlock& pcb)
{
   *this = pcb;
}

ProcessControlBlock& ProcessControlBlock::operator=(const ProcessControlBlock& pcb)
{
   if(this == &pcb)
   {
      return *this;
   }

   m_Cost = pcb.m_Cost;
   m_PID = pcb.m_PID;
   m_State = pcb.m_State;
   m_ProgramCounter = pcb.m_ProgramCounter;
   m_ProgramCode = new vector<component>;
   for(vector<component>::iterator next = pcb.m_ProgramCode->begin(); next < pcb.m_ProgramCode->end(); next++)
   {
      m_ProgramCode->push_back(*next);
   }
   return *this;
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

bool ProcessControlBlock::ComparePID(const ProcessControlBlock &lhs, const ProcessControlBlock &rhs)
{
   if(lhs.m_PID < rhs.m_PID)
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


void ProcessControlBlock::GetCurrentComponent(component& copyTo)
{
   copyTo = m_ProgramCode->at(m_ProgramCounter);
}

component* ProcessControlBlock::GetCurrentComponentPtr()
{
   return &(m_ProgramCode->at(m_ProgramCounter));
}

