// This is start of the header guard and can be any unique name.  By convention, we use the name of the header file.
#ifndef histogram_H
#define histogram_H
#include <array>

class histogram{
private:
  std::array<long long, 20000> buckets = { { } };
  long long maxbuckets; 
  long long binwidth;
  long long fullperiod_ns;
  
public:
  histogram(const long long setmaxbuckets, const long long setbinwidth);
  ~histogram();
  
  void put(long long &Eventts);
  void reset();
  void printheader();
  void print();
};

#endif // This is the end of the header guard
