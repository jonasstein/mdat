#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_
#include "Globaltypes.h"
#include <cstdint>  // int8_t
#include <iostream> // std::cout
#include <sstream>
#include <vector>

namespace histo {

class Histogram {
  private:
    std::vector<uint64_t> frequency;
    std::vector<TimestampClass> bins;
    uint64_t binwidth_ns;
    uint64_t NumberOfBins;
    TimestampClass switchingperiod;

  public:
    Histogram(uint64_t TheNumberOfBins, TimestampClass binwidth);
    virtual ~Histogram();
    void put(TimestampClass &Event);
    void clear();
    std::vector<uint64_t> getfrequency();
    std::vector<TimestampClass> getbins();
    std::string frequencystring();
    std::string binsstring();
};

} /* namespace histo */

#endif /* HISTOGRAM_H_ */
