#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_
#include <sstream>
#include <cstdint> // int8_t
#include <vector>
#include "Globaltypes.h"

namespace histo {

template<typename T, typename A>

std::string vectortostring(std::vector<T,A> const & v);

class Histogram {
private:
	std::vector<uint64_t> frequency;
	std::vector<TimestampClass> bins;
	  uint64_t binwidth_ns;
	  uint64_t NumberOfBins;
	  TimestampClass switchingperiod;

public:
	Histogram(uint64_t TheNumberOfBins,TimestampClass binwidth);
	virtual ~Histogram();
	void put(TimestampClass &Event);
	void clear();
	std::vector<uint64_t> getfrequency();
	std::vector<TimestampClass> getbins();
};

} /* namespace histo */

#endif /* HISTOGRAM_H_ */
