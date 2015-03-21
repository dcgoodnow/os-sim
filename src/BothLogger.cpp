#include <BothLogger.h>
#include <FileLogger.h>
#include <iostream>

using namespace std;

BothLogger::BothLogger(const string &file): m_FileLogger(file) {}

void BothLogger::print(string message)
{
   cout << message;
   m_FileLogger.print(message);
}

void BothLogger::println(string message)
{
   cout << message << '\n';
   m_FileLogger.println(message);
}
