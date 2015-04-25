#include <FileLogger.h>
#include <iostream>

using namespace std;

FileLogger::FileLogger(const string& file)
{
   pthread_mutex_init(&m_OutputMutex, NULL);
   m_FileName = file;
   m_Output.open(m_FileName.c_str(), fstream::app | fstream::out);
}

FileLogger::~FileLogger()
{
   m_Output.close();
}

void FileLogger::print(const string& message)
{
   pthread_mutex_lock(&m_OutputMutex);
   m_Output << message;
   pthread_mutex_unlock(&m_OutputMutex);
}

void FileLogger::println(const string& message)
{
   pthread_mutex_lock(&m_OutputMutex);
   m_Output << message << '\n';
   pthread_mutex_unlock(&m_OutputMutex);
}
