#include <FileLogger.h>

using namespace std;

FileLogger::FileLogger(const string &file)
{
   m_FileName = file;
   m_Output.open(file.c_str(), fstream::out);
}

FileLogger::~FileLogger()
{
   m_Output.close();
}

void FileLogger::print(string message)
{
   m_Output << message;
}

void FileLogger::println(string message)
{
   m_Output << message;
}
