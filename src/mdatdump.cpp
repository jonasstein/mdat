//==============================================
// Name        : dumpmdat.cpp
// Author      : Jonas Stein
// Description : Extract events with timestamps
//               from binary mdat files
//==============================================
#include "Lmfile.h"
#include "Mdatevent.h"
#include "QMRConfig.h"
#include "errorcodes.h"
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>

void printhelp() {
    std::cout << "Version: " << QMR_VERSION_STRING << "\n"
              << "Usage: dumpmdat <filename> \n\n";
    fprintf(stdout, "dumpmdat was compiled at %s on %s \n", __TIME__, __DATE__);
}

int main(int argc, char *argv[]) {
    std::stringstream msgerr(); // thread save cout

    if (argc != 2) {
        fprintf(stderr, "%s\n", error_004_arguments.c_str());
        fprintf(stderr, "\nExpected 1, got %d. Stopped\n", argc - 1);
        printhelp();
        exit(3);
    } else {
        std::string ArgFilename(argv[1]);

        if (!std::filesystem::exists(ArgFilename)) {
            throw std::runtime_error{error_008_filenotfound};
        } else {
            mfile::Lmfile limo(ArgFilename, 0);

            limo.convertlistmodefile();
            return (EXIT_SUCCESS);
        }
    }
}
