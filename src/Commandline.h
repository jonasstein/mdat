#ifndef SRC_COMMANDLINE_H_
#define SRC_COMMANDLINE_H_

#include <boost/program_options.hpp>
#include <string>
namespace po = boost::program_options;

namespace commandline {

class Commandline {
  public:
    struct parameters {
        uint8_t getChDet;
    };

    Commandline();
    virtual ~Commandline();

    void parse(int argc, char *argv[]);

    uint8_t getChDet();

  private:
};

} /* namespace commandline */

#endif /* SRC_COMMANDLINE_H_ */
