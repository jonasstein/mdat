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
                       "TrigID of the events which mark start and end (0..7)");
    desc.add_options()("dataid,d",
                       po::value<Channel_t>(&ArgDataID)->default_value(1),
                       "DataID of the events which mark start and end (0..7)");
    desc.add_options()("keep,k",
                       po::value<Channel_t>(&ArgKeep)->default_value(9),
                       "DataID of trigger events to keep");
    desc.add_options()("start,m",
                       po::value<Counter_t>(&ArgStart)->default_value(1),
                       "Slice starts at the m-th event with the given DataID");
    desc.add_options()("end,n", po::value<Counter_t>(&ArgEnd)->default_value(2),
                       "Slice ends at the n-th event with the given DataID");
    desc.add_options()("autoend,a", "Slice ends automatically at the last event.");
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

    std::ifstream ifs(ArgFilename, std::ifstream::in);


    if (!ifs) {
        std::cerr << "ERROR: Could not open file." << std::endl;
        return (1);
    }

    TimestampClass currentts_ns{0};
    uint16_t       trigid{0};
    uint16_t       dataid{0};
    uint32_t       data{0}; // counter 0 .. 2^19-1

    bool isinframe{false};
    bool iskeepme{false};
    bool ismarker{false};
    bool isstartmarker{false};
    bool startmarkerwasfound{false};
    bool isendmarker{false};
    bool endmarkerwasfound{false};

    Counter_t occurence{0};

    while (ifs >> currentts_ns >> trigid >> dataid >> data) {
            ismarker = ((ArgTrigID == trigid) && (ArgDataID == dataid));
            if (ismarker)
            	occurence++;
    }
    ifs.clear(); // reset EOF flag
    ifs.seekg(0, ifs.beg); // go to file start again

    if (vm.count("autoend")) {
    	ArgEnd = occurence;
    }

    if (ArgStart > ArgEnd){
        	std::cerr << "ERROR: Cannot proceed with Start < End.\n";
        	return (1);
        }


    occurence = 0;

    while (ifs >> currentts_ns >> trigid >> dataid >> data) {
        ismarker = ((ArgTrigID == trigid) && (ArgDataID == dataid));
        isinframe   = (ArgStart <= occurence) && (occurence < ArgEnd);
        iskeepme = (7 == trigid ) && (dataid == ArgKeep);

        if (ismarker)
                	occurence++;

        if (ismarker && !startmarkerwasfound && ArgStart == occurence)
        {
        	isstartmarker=true;
        	startmarkerwasfound=true;
        }
        else isstartmarker=false;

        if (ismarker && !endmarkerwasfound && ArgEnd == occurence){
           	isendmarker=true;
           	endmarkerwasfound=true;
        }
        else isendmarker=false;

        if ((isinframe || iskeepme) || isstartmarker || isendmarker) {
            std::cout << currentts_ns << " " << trigid << " " << dataid << " "
                      << data << "\n";
        }
    }

    ifs.close();
    return (EXIT_SUCCESS);
}
