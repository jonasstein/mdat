#ifndef LMFILE_H_
#define LMFILE_H_

#include <string>
#include <cstdint>  // int8_t
#include <fstream>

namespace mfile {

uint16_t byteswap(uint16_t word);
uint64_t LowMidHigh(uint16_t Low, uint16_t Mid, uint16_t High);

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
	Lmbuffer(uint16_t const rawbuffer[20]);
	virtual ~Lmbuffer();
	Lmbuffer parsebuffer();
};



typedef int64_t filesize_t;

const uint64_t headersignature     = 0x00005555AAAAFFFF;
const uint64_t datablocksignature  = 0x0000FFFF5555AAAA;
const uint64_t filesignature       = 0xFFFFAAAA55550000;

class Lmfile {
private:
	std::ifstream ifs;
	filesize_t filesize;
	uint64_t firsttimestamp_ns {0};
	uint8_t verbositylevel {0};
	void readheadersignature();
	void readdatablocksignature();
	void readfilesignature();
	uint64_t read64bit();
	bool verbose;


public:
	Lmfile( const std::string mypath );
	virtual ~Lmfile();
    void convertlistmodefile();
    void printposition();
	void parsefileheader();
    void parsedatablock();
    void setverbosity(bool beverbose);
    bool getverbosity();



};

} /* namespace mfile */

#endif /* LMFILE_H_ */
