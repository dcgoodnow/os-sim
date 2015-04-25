#include <BothLogger.h>
#include <FileLogger.h>
#include <iostream>

using namespace std;

BothLogger::BothLogger(const string& file): m_FileLogger(file) {}

void BothLogger::print(const string& message)
{
   pthread_mutex_lock(&m_OutputMutex);
   cout << message;
   m_FileLogger.print(message);
   pthread_mutex_unlock(&m_OutputMutex);
}

void BothLogger::println(const string& message)
{
   pthread_mutex_lock(&m_OutputMutex);
   cout << message << '\n';
   m_FileLogger.println(message);
   pthread_mutex_unlock(&m_OutputMutex);
}
