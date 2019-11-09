#include "Lmfile.h"
#include <fstream>      // std::ifstream
#include <string>
#include <istream>
#include <iostream>     // std::cout
#include <filesystem>



namespace mevent {
Lmbuffer::Lmbuffer(){}

Lmbuffer::~Lmbuffer(){}


Lmfile::Lmfile(std::string const mypath) : ifs ( mypath, std::ifstream::binary ), filesize ( 0 ),  firsttimestamp_ns ( 0 )
{
  //filesize = std::filesystem::file_size(mypath); // use this C++17 feature in future

}

Lmfile::~Lmfile() {
	//close file automatically
}


uint64_t Lmfile::read64bit ( )
{
  uint64_t sequenceRAW = 0;
  ifs.read ( reinterpret_cast<char *> ( &sequenceRAW ),8 );
  // nb: little endian!

 // return __builtin_bswap64 ( sequenceRAW );
  return 0; //FIXME
}



uint32_t Lmfile::parsefileheader()
{
  // read first line and parse second line with number of lines
  std::string thisline;
  std::getline(ifs, thisline);

  if (thisline != "mesytec psd listmode data")
	  throw std::runtime_error{"Error 001: Listmode file header not found"};



  std::getline(ifs, thisline ); // header length: nnnnn lines
  int posi = thisline.find(": ");
  std::string sustri  = thisline.substr(posi+1,posi+4); // TODO  + 4 => EOL

  uint32_t fileHeaderLength {0};
  fileHeaderLength = std::stoi (sustri,nullptr,10);

 // assert(fileHeaderLength==2); // we do not know about files <> 2 header lines yet. execute n-2 readlines here some day.

  // for n-1
  // read byte until \n
  // or std::getline(ifs, thisline);

  uint64_t sequenceRAW = Lmfile::read64bit ( );
  //assert(sequenceRAW == headersignature);
  return(fileHeaderLength);
}

/*

Lmbuffer parsebuffer(){  //40 char go in, buffer goes out
	Lmbuffer::Lmbuffer buf;
	return buf;
}
*/


} /* namespace mevent */

//readfileheader
//buffer::buffer init (40 bytes)
//buffer.runid
//...
//buffer: uint16_t length
//
