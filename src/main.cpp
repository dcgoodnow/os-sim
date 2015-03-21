#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <pthread.h>
#include <OS.h>

using namespace std;


struct readProgData {
   string fname;
   vector<component> components;
};

void *ReadProgram(void * threadArg);
int main(int argc, char* argv[])
{ 
   OS simos(argv[1]);

   vector<component> comps;
   simos.ReadProgram(comps);
   simos.Run(comps);
   return 0;
}

