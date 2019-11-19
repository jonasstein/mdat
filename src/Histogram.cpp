#include "Histogram.h"
#include "Globaltypes.h"
#include <cstdint> // int8_t
#include <sstream>
#include <vector>

namespace histo
{

Histogram::Histogram (uint64_t setNumberOfBins, TimestampClass setbinwidth_ns)
: NumberOfBins (setNumberOfBins), binwidth_ns (setbinwidth_ns)
{
    frequency.resize (setNumberOfBins);
    bins.reserve (setNumberOfBins);
    switchingperiod = setNumberOfBins * setbinwidth_ns;

    for (auto i = 0; i < bins.size (); ++i)
    {
        bins[i] = i * setbinwidth_ns;
    }
}

Histogram::~Histogram ()
{
}

void Histogram::put (TimestampClass &Event)
{
    TimestampClass timeOnHistoScale;
    uint64_t index = 0;
    timeOnHistoScale = Event % (NumberOfBins * binwidth_ns);
    index = timeOnHistoScale / binwidth_ns;
    frequency[index]++;
}

void Histogram::clear ()
{
    bins.clear ();
    frequency.clear ();
}

std::vector<uint64_t> Histogram::getfrequency ()
{
    return frequency;
}
std::vector<TimestampClass> Histogram::getbins ()
{
    return bins;
}

} /* namespace histo */
