#include <vector>
#include <OS.h>
#include <iostream>

using namespace std;


int main(int argc, char* argv[])
{ 

   if(argc < 2)
   {
      cerr << "No configuration file.\n";
      return -1;
   }
   OS simos(argv[1]);

   vector<component> comps;
   try
   {
      simos.ReadConfig();
   }
   catch(ConfigReadException)
   {
      cerr << "ERROR: could not read configuration file.\n";
      return -1;
   }
   catch(MalformedConfigException)
   {
      cerr << "ERROR: Configuration file is malformed.\n";
      return -1;
   }

   try
   {
      simos.ReadProgram(comps);
   }
   catch(MetadataReadException)
   {
      cerr << "ERROR: Could not read metadata file.\n";
      return -1;
   }
   simos.Run(comps);
   return 0;
}

