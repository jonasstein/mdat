#include "Lmfile.h"
#include <fstream>      // std::ifstream
#include <string>
#include <istream>
#include <iostream>     // std::cout
//#include <filesystem>
//#include <assert.h>
#include <sys/stat.h>


namespace mevent {



Lmfile::Lmfile(std::string const mypath) : ifs ( mypath, std::ifstream::binary ), filesize ( 0 ),  firsttimestamp_ns ( 0 )
{
  //filesize = std::filesystem::file_size(mypath); // use this C++17 feature in future

}

Lmfile::~Lmfile() {
	//close file automatically
}


} /* namespace mevent */
