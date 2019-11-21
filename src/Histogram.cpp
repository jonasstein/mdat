#include "Histogram.h"
#include "Globaltypes.h"
#include <cstdint> // int8_t
#include <sstream>
#include <vector>

namespace histo {

Histogram::Histogram(uint64_t setNumberOfBins, TimestampClass setbinwidth_ns)
    : NumberOfBins(setNumberOfBins), binwidth_ns(setbinwidth_ns) {
    frequency.resize(setNumberOfBins);
    bins.resize(setNumberOfBins);
    switchingperiod = setNumberOfBins * setbinwidth_ns;

    for (auto i = 0; i < bins.size(); ++i) {
        bins[i] = i * setbinwidth_ns;
    }
}

Histogram::~Histogram() {}

void Histogram::put(TimestampClass &Event) {
    TimestampClass timeOnHistoScale{0};
    uint64_t index = 0;
    timeOnHistoScale = Event % (NumberOfBins * binwidth_ns);
    index = timeOnHistoScale / binwidth_ns;
    frequency[index]++;
}

void Histogram::clear() {
    bins.clear();
    frequency.clear();
}

std::vector<uint64_t> Histogram::getfrequency() { return (frequency); }
std::vector<TimestampClass> Histogram::getbins() { return (bins); }

std::string Histogram::frequencystring() {
    auto v = frequency;
    std::stringstream ss{""};

    if (v.size() > 0) ss << v[0];

    if (v.size() > 1) {
        for (auto i = 1; i < v.size(); i++) {
            ss << ",";
            ss << v[i];
        }
    }
    return (ss.str());
}

std::string Histogram::binsstring() {
    auto v = bins;
    std::stringstream ss{""};
    ss << v[0];
    if (v.size() > 1) {
        for (auto i = 1; i < v.size(); i++) {
            ss << ",";
            ss << v[i];
        }
    }
    return (ss.str());
}
} /* namespace histo */
