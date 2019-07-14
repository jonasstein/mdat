#ifndef LMFILE_H_
#define LMFILE_H_

#include <string>
#include <cstdint>
#include <fstream>

namespace mevent {

class Lmfile {
private:
	std::ifstream ifs;
	uintmax_t filesize = 0;
	uint64_t firsttimestamp_ns = 0;

public:
	Lmfile( const std::string mypath );
	virtual ~Lmfile();
	uint32_t parsefileheader();
	uint64_t Lmfile::read64bit();


};

} /* namespace mevent */

#endif /* LMFILE_H_ */