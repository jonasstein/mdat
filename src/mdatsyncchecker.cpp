#include "Timestamps.h"
#include <boost/program_options.hpp>
#include <cstdint>
#include <fstream>  // std::ifstream
#include <iostream> // std::cout
#include <istream>
#include <string> // std::string, std::stoull

namespace po = boost::program_options;

int main(int argc, char *argv[]) {

    using Channel_t = uint16_t;
    using Counter_t = uint64_t;

    Channel_t ArgTrigID{7};
    Channel_t ArgDataID{1};
    Channel_t ArgKeep{9};
    Counter_t ArgStart{7};
    Counter_t ArgEnd{7};

    std::string ArgFilename{};

    po::options_description desc{"Options"};
    desc.add_options()("help,h", "Help screen");
    desc.add_options()("filename,f", po::value<std::string>(&ArgFilename),
                       "Name of the input file");
    desc.add_options()("trigid,t",
                       po::value<Channel_t>(&ArgTrigID)->default_value(1),
                       "TrigID of the events to check (0..7)");
    desc.add_options()("dataid,d",
                       po::value<Channel_t>(&ArgDataID)->default_value(1),
                       "DataID of the events to check (0..7)");
    desc.add_options()("milli,m", "Print the time span between two events in "
                                  "milli seconds instead of seconds");
    desc.add_options()("nano,n", "Print the time span between two events in "
                                 "nano seconds instead of seconds");
    po::command_line_parser parser{argc, argv};
    parser.options(desc).allow_unregistered().style(
        po::command_line_style::default_style);
    po::parsed_options parsed_options = parser.run();

    po::variables_map vm{};
    store(parsed_options, vm);
    notify(vm);

    if (vm.count("help")) {
        std::cout << "mdatslice by Jonas Stein (2016-2019)\n\n"
                  << desc << std::endl;
        return (EXIT_SUCCESS);
    }

    constexpr uint64_t sinano_inv{1'000'000'000};
    constexpr uint64_t similli_inv{1'000};
    uint64_t           sifactor{1};
    std::string        siunit{"ns"};

    if (vm.count("milli")) {
        sifactor = sinano_inv / similli_inv;
        siunit   = "ms";
    }

    if (vm.count("nano")) {
        sifactor = sinano_inv;
        siunit   = "ns";
    }

    std::ifstream ifs(ArgFilename, std::ifstream::in);

    if (!ifs) {
        std::cerr << "ERROR: Could not open file." << std::endl;
        return (1);
    }

    TimestampClass currentts_ns{0};
    TimestampClass lastts_ns{0};
    uint16_t       trigid{0};
    uint16_t       dataid{0};
    uint32_t       data{0}; // counter 0 .. 2^19-1

    bool isinframe{false};
    bool iskeepme{false};
    bool ismarker{false};
    bool isstartmarker{false};
    bool startmarkerwasfound{false};
    bool endmarkerwasfound{false};

    Counter_t occurence{0};

    occurence = 0;

    std::cout << "Timestamp_(ns) Timespan_(" << siunit << ") \n";

    while (ifs >> currentts_ns >> trigid >> dataid >> data) {
        ismarker  = ((ArgTrigID == trigid) && (ArgDataID == dataid));
        isinframe = (ArgStart <= occurence) && (occurence < ArgEnd);
        iskeepme  = (7 == trigid) && (dataid == ArgKeep);

        if (ismarker && !startmarkerwasfound) {
            isstartmarker       = true;
            startmarkerwasfound = true;
        } else
            isstartmarker = false;

        if (ismarker) {
            occurence++;
            std::cout << currentts_ns << " "
                      << (currentts_ns - lastts_ns) / sifactor << "\n";
            lastts_ns = currentts_ns;
        }
    }

    ifs.close();
    return (EXIT_SUCCESS);
}
