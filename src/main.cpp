#include <vector>
#include <OS.h>

using namespace std;


int main(int argc, char* argv[])
{ 
   OS simos(argv[1]);

   vector<component> comps;
   simos.ReadProgram(comps);
   simos.Run(comps);
   return 0;
}

