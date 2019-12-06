//==============================================
// Name        : timeshift.cpp
// Author      : Jonas Stein
// Description : Record the offset time in a
//               decoded list mode file and
//               subtract the offset from all
//               time stamps.
//==============================================

#include "Timestamps.h"
#include <fstream>  // std::ifstream
#include <iostream> // std::cout

int main(int argc, char *argv[]) {

    TimestampClass StartOffset_ns{0};
    TimestampClass currentts_ns{0};
    uint16_t       trigid{0};
    uint16_t       dataid{0};
    uint32_t       data{0}; // counter 0 .. 2^19-1

    auto ArgFilename{argv[1]};

    std::ifstream ifs(ArgFilename, std::ifstream::in);

    if (!ifs) {
        std::cerr << "ERROR: Could not open file." << std::endl;
        exit(1);
    }

    if (ifs >> currentts_ns >> trigid >> dataid >> data) {
        StartOffset_ns = currentts_ns;
        std::cout << " " << currentts_ns - StartOffset_ns << " " << trigid
                  << " " << dataid << " " << data << "\n";
    } else
        std::cerr << "Error: Could not open file."
                  << "\n";

    while (ifs >> currentts_ns >> trigid >> dataid >> data) {
        std::cout << " " << currentts_ns - StartOffset_ns << " " << trigid
                  << " " << dataid << " " << data << "\n";
    }
}
