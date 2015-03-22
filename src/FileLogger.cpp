#include <FileLogger.h>
#include <iostream>

using namespace std;

FileLogger::FileLogger(const string& file)
{
   m_FileName = file;
   m_Output.open(m_FileName.c_str(), fstream::app | fstream::out);
}

FileLogger::~FileLogger()
{
   m_Output.close();
}

void FileLogger::print(const string& message)
{
   m_Output << message;
}

void FileLogger::println(const string& message)
{
   m_Output << message << '\n';
}
