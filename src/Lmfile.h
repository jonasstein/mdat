#ifndef LMFILE_H_
#define LMFILE_H_

#include <cstdint> // int8_t
#include <fstream>
#include <string>
#include <vector>
#include "Mdatevent.h"

namespace mfile {

class Lmbuffer {
private:
  uint16_t bufferlengthinwords{0};
  uint16_t buffertype{0};
  uint16_t headerlengthinwords{0};
  uint16_t buffernumber{0};
  uint16_t runid{0};
  uint8_t mcpdid{0};
  uint8_t status{0};
  TimestampClass headertimestamp_ns{0};
  uint64_t parameter0{0};
  uint64_t parameter1{0};
  uint64_t parameter2{0};
  uint64_t parameter3{0};

public:
  Lmbuffer(std::vector<uint16_t> rawbuffer);
  virtual ~Lmbuffer();
  uint16_t getbufferlengthinwords();
  uint16_t getheaderlengthinwords();
  TimestampClass getheadertimestamp_ns();
  uint16_t getrunid();
};

typedef int64_t filesize_t;
typedef uint64_t signature_t;

const signature_t headersignature = 0x00005555AAAAFFFF;
const signature_t datablocksignature = 0x0000FFFF5555AAAA;
const signature_t filesignature = 0xFFFFAAAA55550000;

class Lmfile {
private:
  std::ifstream ifs;
  filesize_t filesize;
  TimestampClass firsttimestamp_ns{0};
  uint8_t verbositylevel{0};
  void readheadersignature();
  void readbuffersignature();
  void readfilesignature();
  uint64_t read64bit();

public:
  Lmfile(const std::string mypath, uint8_t verbositylevel);
  virtual ~Lmfile();
  void convertlistmodefile();
  filesize_t getposition();
  std::vector<uint16_t> getbufferheader();
  void jumpbehindfileheader();
  void setverbosity(uint8_t beverbose);
  uint8_t getverbosity();
  uint64_t getsortedevent();
  filesize_t getfilesize();
  filesize_t getbytestillEOF();
};

} /* namespace mfile */

#endif /* LMFILE_H_ */
