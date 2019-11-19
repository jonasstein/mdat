#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_
#include "Globaltypes.h"
#include <cstdint> // int8_t
#include <sstream>
#include <vector>

namespace histo
{

template <typename T, typename A>

std::string vectortostring (std::vector<T, A> const &v)
{
    std::stringstream ss;

    ss << v[0];
    if (v.size () > 1)
    {
        for (auto i = 1; i < v.size (); i++)
        {
            ss << ",";
            ss << v[i];
        }
    }
    return ss.str ();
}

class Histogram
{
    private:
    std::vector<uint64_t> frequency;
    std::vector<TimestampClass> bins;
    uint64_t binwidth_ns;
    uint64_t NumberOfBins;
    TimestampClass switchingperiod;

    public:
    Histogram (uint64_t TheNumberOfBins, TimestampClass binwidth);
    virtual ~Histogram ();
    void put (TimestampClass &Event);
    void clear ();
    std::vector<uint64_t> getfrequency ();
    std::vector<TimestampClass> getbins ();
};

} /* namespace histo */

#endif /* HISTOGRAM_H_ */
