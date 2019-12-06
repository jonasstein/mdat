//==============================================
// Name        : timeshift.cpp
// Author      : Jonas Stein
// Description : Receive a list mode file from
//               the standard input stream
//               take the offset timestamp from
//               the first line and subtract the
//               offset from all time stamps.
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

    if (std::cin >> currentts_ns >> trigid >> dataid >> data) {
    	// read the timestamp from the first line
        StartOffset_ns = currentts_ns;
        std::cout << " " << currentts_ns - StartOffset_ns << " " << trigid
                  << " " << dataid << " " << data << "\n";

        // shift the timestamp for all other lines
        while (std::cin >> currentts_ns >> trigid >> dataid >> data) {
            std::cout << " " << currentts_ns - StartOffset_ns << " " << trigid
                      << " " << dataid << " " << data << "\n";
        }

    } else {
        std::cerr << "Error: Could not read data from pipe."
                  << "\n";
        exit(3);
    }
    return (EXIT_SUCCESS);
}
