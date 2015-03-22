#include <MonitorLogger.h>
#include <iostream>

using namespace std;


void MonitorLogger::print(const string& message)
{
   cout << message;
}

void MonitorLogger::println(const string& message)
{
   cout << message << '\n';
}
