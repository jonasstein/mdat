// This is start of the header guard and can be any unique name.  By convention, we use the name of the header file.
#ifndef histogram_H
#define histogram_H
#include <array>

class histogram
{
    private:
    std::array<uint64_t, 20000> buckets = { {} };
    uint64_t maxbuckets;
    uint64_t binwidth;
    uint64_t fullperiod_ns;

    public:
    histogram (const uint64_t setmaxbuckets, const uint64_t setbinwidth);
    ~histogram ();

    void put (uint64_t &Eventts);
    void reset ();
    void printheader ();
    void print ();
};

#endif // This is the end of the header guard
