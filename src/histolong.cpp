#include "histolong.hpp"
#include <cstdint>
#include <iostream> // std::cout

histogram::histogram (const uint64_t setmaxbuckets, const uint64_t setbinwidth)
{
    maxbuckets = setmaxbuckets;
    binwidth = setbinwidth;
    fullperiod_ns = binwidth * maxbuckets;
}

histogram::~histogram ()
{
}

void histogram::put (uint64_t &Eventts)
{
    uint64_t timeOnHistoScale;
    uint64_t index = 0;
    timeOnHistoScale = Eventts % (maxbuckets * binwidth);
    index = (uint64_t) ((timeOnHistoScale * maxbuckets) / fullperiod_ns);
    buckets[index]++;
}

void histogram::reset ()
{
    for (uint64_t n = 0; n < maxbuckets; n++)
    {
        buckets[n] = 0;
    }
}

void histogram::printheader ()
{
    for (uint64_t a = 0; a < maxbuckets; a++)
    {
        if (a < maxbuckets - 1)
        {
            std::cout << a * binwidth << ",";
        }
        else
            std::cout << a * binwidth << std::endl;
    }
}

void histogram::print ()
{
    for (uint64_t a = 0; a < maxbuckets; a++)
    {
        if (a < maxbuckets - 1)
        {
            std::cout << buckets[a] << ",";
        }
        else
            std::cout << buckets[a] << std::endl;
    }
}
