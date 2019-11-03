#include "Lmfile.h"
#include <fstream>      // std::ifstream
#include <string>
#include <istream>
#include <iostream>     // std::cout
//#include <filesystem>



namespace mevent {


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
  uint32_t fileHeaderLength;
  // read first line and parse second line with number of lines
  std::string thisline;
  std::getline(ifs, thisline);
  //assert(thisline == ("mesytec psd listmode data"));

  std::getline(ifs, thisline ); // header length: nnnnn lines
  int posi = thisline.find(": ");
  std::string sustri  = thisline.substr(posi+1,posi+4); // TODO  + 4 => EOL
  fileHeaderLength = std::stoi (sustri,nullptr,10);
 // assert(fileHeaderLength==2); // we do not know about files <> 2 header lines yet. execute n-2 readlines here some day.

  // for n-1
  // read byte until \n
  // or std::getline(ifs, thisline);

  uint64_t sequenceRAW = Lmfile::read64bit ( );
  //assert(sequenceRAW == headersignature);
  return(fileHeaderLength);
}



} /* namespace mevent */

//readfileheader
//buffer readbufferheader
//buffer: uint16_t length
