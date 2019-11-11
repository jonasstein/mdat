#include "Lmfile.h"
#include "Bitslicer.h"
#include "Mdatevent.h"
#include "errorcodes.h"

#include <cstdint>  // int8_t
#include <fstream>  // std::ifstream
#include <iostream> // std::cout
#include <istream>
#include <string>

namespace mfile {

Lmbuffer::Lmbuffer(uint16_t const rawbuffer[20]) {
  bufferlengthinwords = bitslicer::byteswap(rawbuffer[0]);
  buffertype = bitslicer::byteswap(rawbuffer[1]);
  headerlengthinwords = bitslicer::byteswap(rawbuffer[2]);
  buffernumber = bitslicer::byteswap(rawbuffer[3]);
  runid = bitslicer::byteswap(rawbuffer[4]);
  // mcpdid
  // status

  uint16_t htsLow = bitslicer::byteswap(rawbuffer[6]);
  uint16_t htsMid = bitslicer::byteswap(rawbuffer[7]);
  uint16_t htsHigh = bitslicer::byteswap(rawbuffer[8]);

  headertimestamp = bitslicer::LowMidHigh(htsLow, htsMid, htsHigh);
}

Lmbuffer::~Lmbuffer() {}

Lmfile::Lmfile(std::string const mypath)
    : ifs(mypath, std::ifstream::ate | std::ifstream::binary), filesize(0),
      firsttimestamp_ns(0) {
  // "ate" placed cursor to EOF, we can read out the filesize now and go to
  // start.
  filesize = ifs.tellg();

  if (filesize < 134) {
    std::cerr << "Size of file: " << filesize << " Bytes. \n";
    throw std::runtime_error{error_003_filetoosmall};
  }
  ifs.seekg(0, ifs.beg);
}

Lmfile::~Lmfile() {
  // close file automatically
}

uint64_t Lmfile::read64bit() {
  uint64_t sequenceRAW = 0;
  ifs.read(reinterpret_cast<char *>(&sequenceRAW), 8);
  return sequenceRAW;
}

void Lmfile::convertlistmodefile() {
  Lmfile::parsefileheader();
  //bool fileEOF = false;

  /*
  while (fileEOF == false)
  {
    Lmfile::parsedatablock();
    fileEOF = Lmfile::EOFahead();
  };
  */
}

void Lmfile::parsefileheader() {
  // read first 2 lines of the file
  // parse second line with length of header
  // return length of header in number of lines
  std::string thisline;
  std::getline(ifs, thisline);

  if (thisline != "mesytec psd listmode data")
    throw std::runtime_error{error_001_noheader};

  std::getline(ifs, thisline); // header length: nnnnn lines
  int numberofheaderlines = thisline.find(": ");
  std::string sustri = thisline.substr(
      numberofheaderlines + 1, numberofheaderlines + 4); // TODO  + 4 => EOL

  uint32_t fileHeaderLength = std::stoi(sustri, nullptr, 10);

  if (fileHeaderLength != 2)
    throw std::runtime_error{error_002_headernottwolines};

  // for fileHeaderLength-1
  //     std::getline(ifs, thisline);

  Lmfile::readheadersignature();

  std::cout << "\n Position: " << ifs.tellg() << " \n \n";
}

void parsedatablock();

void Lmfile::setverbositylevel(uint8_t vlevel) { verbositylevel = vlevel; }

void Lmfile::readheadersignature() {
  uint64_t sequenceRAW = Lmfile::read64bit();
  if (headersignature != sequenceRAW)
    throw std::runtime_error{error_005_noheadersig};
}

void Lmfile::readdatablocksignature() {
  uint64_t sequenceRAW = Lmfile::read64bit();
  if (datablocksignature != sequenceRAW)
    throw std::runtime_error{error_006_nodatasig};
}

void Lmfile::readfilesignature() {
  uint64_t sequenceRAW = Lmfile::read64bit();
  if (filesignature != sequenceRAW)
    throw std::runtime_error{error_007_noeofsig};
}

/*

Lmbuffer parsebuffer(){  //40 char go in, buffer goes out
        Lmbuffer::Lmbuffer buf;
        return buf;
}
*/

} /* namespace mfile */

// buffer::buffer init (40 bytes)
// buffer.runid
//...
// buffer: uint16_t length
//
