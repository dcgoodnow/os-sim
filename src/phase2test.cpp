#include <iostream>
#include <OS.h>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
   OS os(argv[1]);

   try{
      os.ReadConfig();
   }
   catch(exception e)
   {
      cout << e.what();
   }

   vector<component> comps;
   try{
      os.ReadProgram(comps);
   }
   catch(exception e)
   {
      cout << e.what();
   }

   cout << "\n\n\n" << os.ComputeCost(comps);

}