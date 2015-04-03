#include <iostream>
#include <OS.h>

using namespace std;

int main(int argc, char* argv[])
{

   if(argc < 2)
   {
      cerr << "Usage: ./OS_Phase_2 [configfile]";
   }
   OS os(argv[1]);

   try{
      os.ReadConfig();
   }
   catch(exception e)
   {
      cerr << e.what();
   }

   try{
      os.ReadPrograms();
   }
   catch(exception e)
   {
      cerr << e.what();
   }

   os.Run();


}
