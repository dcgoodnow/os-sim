#include <MonitorLogger.h>
#include <iostream>

using namespace std;

MonitorLogger::MonitorLogger()
{
   pthread_mutex_init(&m_OutputMutex, NULL);
}

void MonitorLogger::print(const string& message)
{
   pthread_mutex_lock(&m_OutputMutex);
   cout << message;
   pthread_mutex_unlock(&m_OutputMutex);
}

void MonitorLogger::println(const string& message)
{
   pthread_mutex_lock(&m_OutputMutex);
   cout << message << '\n';
   pthread_mutex_unlock(&m_OutputMutex);
}
