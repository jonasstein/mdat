//============================================================================
// Name        : extractmdat.cpp
// Author      : Jonas Stein
// Version     :
// Copyright   : Your copyright notice
// Description : Extract events with timestamps from binary mdat files
//============================================================================

#include <iostream>
#include <string>
#include <sstream>      // std::stringstream
#include "QMRConfig.h"
#include "Mdatevent.h"
#include "Lmfile.h"
#include "errorcodes.h"


// true if file exists
bool fileExists(const std::string& file) {
   // struct stat buf;
   //return (stat(file.c_str(), &buf) == 0);
    return true;
}

void printhelp(){
	std::cout << "Version: " << QMR_VERSION_STRING << "\n"
	          << "Usage: dumpmdat <filename> \n\n";
    fprintf(stdout, "dumpmdat was compiled at %s on %s \n", __TIME__, __DATE__);
  }

int main(int argc, char *argv[]){
    std::stringstream msgerr;                               // make cout thread save

    if (argc != 2)
    {
        fprintf(stderr, error_004_arguments);
        fprintf(stderr, "\nExpected 1, got %d. Stopped\n", argc-1);
        printhelp();
        exit(3);
    }
    else  {
        std::string ArgFilename(argv[1]);
        int verbosity = 1;

        if(!fileExists(ArgFilename))
        {
            fprintf(stderr, "Error: File not found: %s \n", ArgFilename.c_str());
            return(EXIT_FAILURE);
        }
        else {

        	mfile::Lmfile* limo;
            limo = new mfile::Lmfile(ArgFilename);
            /*   limo->setverbositylevel(verbosity);

            msgerr << "# size (Bytes): " << limo->getfilesize() << std::endl ;
            limo->convertlistmodefile();
            std::cerr << "# Number of Events: " << limo->getNumberOfEvents() << std::endl;

            delete(limo);
            */
            return(EXIT_SUCCESS);
          }
      }
  }
