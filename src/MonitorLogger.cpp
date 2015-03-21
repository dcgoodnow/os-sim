#include <MonitorLogger.h>
#include <iostream>

using namespace std;

void MonitorLogger::print(string message)
{
   cout << message;
}

void MonitorLogger::println(string message)
{
   cout << message << '\n';
}
