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
   pthread_t readThread;
   readProgData rpd;
   rpd.fname = argv[1];
   int rc = pthread_create(&readThread, NULL, ReadProgram, (void *)&rpd);
   void* status;
   pthread_join(readThread, &status);
   vector<component> comps = rpd.components;
   for(vector<component>::iterator it = comps.begin(); it != comps.end(); ++it)
      cout << (*it).type << '\t' << (*it).operation << '\t' << (*it).cost << '\n';
   return 0;
}

/* Sample meta-data program
 * ========================
 *
 *  Start
 *  Program Meta-Data Code:
 *  S(start)0; A(start)0; P(run)14; I(hard drive)13; O(hard drive)15;
 *  P(run)12; O(hard drive)11; P(run)5; I(hard drive)12; O(hard drive)12;
 *  P(run)5; O(monitor)10; P(run)12; O(monitor)10; A(end)0; S(end)0.
 *  End Program Meta-Data Code.
*/

void *ReadProgram(void *threadArg)
{
   readProgData *data;
   data = (readProgData *) threadArg;
   ifstream metaFile;
   metaFile.open(data->fname.c_str(), fstream::in);
   string temp;

   //discard start line
   getline(metaFile, temp);
   component next;
   do
   {
      metaFile >> next.type;
      metaFile.get();
      getline(metaFile, next.operation, ')');
      metaFile >> next.cost;
      metaFile.get();
      data->components.push_back(next);
   } while(!(next.type == 'S' && next.operation == "end"));
   metaFile.close();
}
