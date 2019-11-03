#ifndef LMFILE_H_
#define LMFILE_H_

#include <string>
#include <cstdint>
#include <fstream>

namespace mevent {

class Lmbuffer{
private:
	uint16_t bufferlengthinwords {0};
	uint16_t buffertype {0};
	uint16_t headerlengthinwords {0};
	uint16_t buffernumber {0};
	uint16_t runid {0};
	uint8_t mcpdid {0};
	uint8_t status {0};
	uint64_t headertimestamp {0};
	uint64_t parameter0 {0};
	uint64_t parameter1 {0};
	uint64_t parameter2 {0};
	uint64_t parameter3 {0};

public:
	Lmbuffer();
	virtual ~Lmbuffer();

};


class Lmfile {
private:
	std::ifstream ifs;
	uintmax_t filesize = 0;
	uint64_t firsttimestamp_ns = 0;

public:
	Lmfile( const std::string mypath );
	virtual ~Lmfile();
	uint32_t parsefileheader();
	uint64_t read64bit();


};

} /* namespace mevent */

#endif /* LMFILE_H_ */
