#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <iostream>
namespace fs = boost::filesystem;
namespace po = boost::program_options;

#include "Commandline.h"

namespace commandline {

Commandline::Commandline() {}

Commandline::~Commandline() {}

void commandline::setup() {
    po::options_description options("Program Options");
    options.add_options()("help,h", "Display help menu.")(
        "version,V", "Display program version number")(
        "time,t", po::value<double>(&myUserTime)->default_value(100.0),
        "User-specified time")("file,f", po::value<std::string>(&myInputFile),
                               "User-specified input file")(
        "value", po::value<int>(&myUserValue)->default_value(43),
        "User-specified value")("gflag,G",
                                po::bool_switch(&myGFlag)->default_value(false),
                                "Toggle G-flag")(
        "rflag,R", po::bool_switch(&myRFlag)->default_value(false),
        "Toggle R-flag")("jflag,J",
                         po::bool_switch(&myJFlag)->default_value(false),
                         "Toggle J-flag");

    myOptions.add(options);
}

void Commandline::parse(int argc, char *argv[]) {}

} // namespace commandline
