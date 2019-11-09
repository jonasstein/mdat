#include "Lmfile.h"
#include <fstream>      // std::ifstream
#include <string>
#include <istream>
#include <iostream>     // std::cout
#include "errorcodes.h"


namespace mevent {
/*Lmbuffer::Lmbuffer(){}

Lmbuffer::~Lmbuffer(){}
*/

Lmfile::Lmfile(std::string const mypath) : ifs ( mypath, std::ifstream::binary ), filesize ( 0 ),  firsttimestamp_ns ( 0 )
{
  // "ate" placed cursor to EOF, we can read out the filesize now and go to start.
 // filesize = ifs.tellg();

  if (filesize < 134)
  	  throw std::runtime_error{error_003_filetoosmall};

  ifs.seekg (0, ifs.beg);
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



void Lmfile::parsefileheader()
{
	// read first 2 lines of the file
	// parse second line with length of header
	// return length of header in number of lines
  std::string thisline;
  std::getline(ifs, thisline);

  if (thisline != "mesytec psd listmode data")
	  throw std::runtime_error{error_001_noheader};



  std::getline(ifs, thisline ); // header length: nnnnn lines
  int posi = thisline.find(": ");
  std::string sustri  = thisline.substr(posi+1,posi+4); // TODO  + 4 => EOL

  uint32_t fileHeaderLength {0};
  fileHeaderLength = std::stoi (sustri,nullptr,10);

  if (fileHeaderLength != 2)
  	  throw std::runtime_error{error_002_headernottwolines};

  // for n-1
  // read byte until \n
  // or std::getline(ifs, thisline);

  uint64_t sequenceRAW = Lmfile::read64bit ( );
  //assert(sequenceRAW == headersignature);

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
