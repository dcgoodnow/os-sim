#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

struct component {
   char type;
   string operation;
   int cost;
};

void ReadProgram(string fname, vector<component> &components);
int main(int argc, char* argv[])
{
   vector<component> comps;
   ReadProgram(argv[1], comps);
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

void ReadProgram(string fname, vector<component> &components)
{
   ifstream metaFile;
   metaFile.open(fname.c_str(), fstream::in);
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
      components.push_back(next);
   } while(!(next.type == 'S' && next.operation == "end"));
   metaFile.close();
}
