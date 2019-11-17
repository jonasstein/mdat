#include "Lmfile.h"
#include "Bitslicer.h"
#include "Mdatevent.h"
#include "errorcodes.h"

#include <cstdint> // int8_t
#include <filesystem>
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

Lmfile::Lmfile(std::string const mypath, uint8_t myverbositylevel)
    : ifs(mypath, std::ifstream::ate | std::ifstream::binary), filesize(0),
      firsttimestamp_ns(0), verbositylevel(myverbositylevel) {

  filesize = std::filesystem::file_size(mypath);

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

uint64_t Lmfile::getsortedevent() {
  uint16_t Low{0};
  uint16_t Mid{0};
  uint16_t High{0};

  ifs.read(reinterpret_cast<char *>(&Low), 2);
  ifs.read(reinterpret_cast<char *>(&Mid), 2);
  ifs.read(reinterpret_cast<char *>(&High), 2);

  return bitslicer::LowMidHigh(Low, Mid, High);
}

void Lmfile::printposition() { std::cout << ifs.tellg(); }

void Lmfile::convertlistmodefile() {
  this->jumpbehindfileheader();
  uint64_t mysig = this->read64bit();

  /*bool fileEOF = false;
  while (fileEOF == false)
  {
    Lmfile::parsedatablock();
    fileEOF = Lmfile::EOFahead();
  };
  */
}

void Lmfile::jumpbehindfileheader() {
  // read first 2 lines of the file
  // parse second line with length of header
  // return length of header in number of lines
  std::string thisline;
  std::getline(ifs, thisline);

  if (thisline != "mesytec psd listmode data")
    throw std::runtime_error{error_001_noheader};

  std::getline(ifs, thisline); // header length: nnnnn lines
  uint64_t numberofheaderlines = thisline.find(": ");
  std::string sustri = thisline.substr(
      numberofheaderlines + 1, numberofheaderlines + 4); // TODO  + 4 => EOL

  uint64_t fileHeaderLength = std::stoi(sustri, nullptr, 10);

  if (fileHeaderLength != 2)
    throw std::runtime_error{error_002_headernottwolines};

  // for fileHeaderLength-1
  //     std::getline(ifs, thisline);

  Lmfile::readheadersignature();
}

void parsedatablock() {}

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

void Lmfile::setverbosity(uint8_t myverbositylevel) {
  this->verbositylevel = myverbositylevel;
}

uint8_t Lmfile::getverbosity() { return this->verbositylevel; }

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
