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
#include <vector>

namespace mfile {

Lmbuffer::Lmbuffer(std::vector<uint16_t> rawbuffer) {
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

  headertimestamp_ns = bitslicer::LowMidHigh(htsLow, htsMid, htsHigh) * 100;
}

Lmbuffer::~Lmbuffer() {}

uint16_t Lmbuffer::getbufferlengthinwords(){
	return this->bufferlengthinwords;
}

uint16_t Lmbuffer::getheaderlengthinwords(){
	return this->headerlengthinwords;
}


uint64_t Lmbuffer::getheadertimestamp_ns(){
	return this->headertimestamp_ns;
}

uint16_t Lmbuffer::getrunid(){
	return this->runid;
}


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
  return bitslicer::byteswap64(sequenceRAW);
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

filesize_t Lmfile::getposition() { return ifs.tellg(); }

void Lmfile::convertlistmodefile() {
  this->jumpbehindfileheader();
  std::cerr << "\n getposition:" << this->getposition() << "\n" << std::endl;
  this->readheadersignature();
  std::cerr << "\n getposition:" << this->getposition() << "\n" << std::endl;
  std::vector<uint16_t> bhwords;
  uint16_t numberofevents {0};


  while (getbytestillEOF() > (40+8+8)){
	  bhwords = this->getbufferheader();

	  mfile::Lmbuffer *mybuffer;
	  mybuffer = new mfile::Lmbuffer(bhwords);

	  numberofevents = (mybuffer-> getbufferlengthinwords() - mybuffer->getheaderlengthinwords())/3;

	  for (uint16_t i = 0; i < numberofevents; i++) {

		  mevent::Mdatevent newevent(this->getsortedevent(), mybuffer->getheadertimestamp_ns);
		    delete newevent;
	  	  }
	  this->readbuffersignature();
	  delete mybuffer;
  }
  readfilesignature();
}

std::vector<uint16_t> Lmfile::getbufferheader() {
  std::vector<uint16_t> mybuf(20);
  uint16_t aword{0};

  for (int i = 0; i < mybuf.size(); ++i) {
    ifs.read(reinterpret_cast<char *>(&aword), 2);
    mybuf[i] = aword;
  }
  return mybuf;
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
      numberofheaderlines + 1, numberofheaderlines + 4);

  uint64_t fileHeaderLength = std::stoi(sustri, nullptr, 10);

  std::string commentline;
  for(auto i=2; i<fileHeaderLength; ++i)
  {
	  std::getline(ifs, commentline);
  }
}

void parsedatablock() {}

void Lmfile::readheadersignature() {
  uint64_t sequenceRAW = Lmfile::read64bit();
//	uint64_t sequenceRAW ;
//	ifs >> sequenceRAW ;
  std::cerr << "\n sequenceRAW: "<< sequenceRAW << "\n" << std::endl;

  if (headersignature != sequenceRAW)
    throw std::runtime_error{error_005_noheadersig};
}

void Lmfile::readbuffersignature() {
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

filesize_t Lmfile::getfilesize() { return this->filesize; }

filesize_t Lmfile::getbytestillEOF(){
	return (this->filesize - ifs.tellg());
}

} /* namespace mfile */

// buffer::buffer init (40 bytes)
// buffer.runid
//...
// buffer: uint16_t length
//
