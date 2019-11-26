#include "Histogram.h"
#include "Timestamps.h"
#include <algorithm> //std::min
#include <cstdint>
#include <fstream>  // std::ifstream
#include <iostream> // std::cout
#include <istream>
#include <stdio.h>
#include <string> // std::string, std::stoull

using Channel_t = uint32_t;
using Mode_t = uint32_t;
using Counter_t = uint64_t;

enum class Modeselector_t : uint16_t { infomode = 1, histogrammode = 2 };
Modeselector_t mode{Modeselector_t::infomode};

void printhelp() {
    std::cout << "mkhistogram by Jonas Stein (2016-2019) \n"
              << "Usage: mkhistogram <ChDet> <ChSync> "
                 "<ChSemaphore> <ChMonitor> "
                 "<filename> <bins> <mode> \n"
              << "Only ChMonitor 0..3 will be printed \n"
              << "mode = 1 infomode, 2 histogram \n"
              << "The monitor channel will be disabled automatically, if "
                 "ChMonitor > 3."
              << std::endl;
}

int main(int argc, char *argv[]) {

    if (argc != 8) {
        std::cerr << "Error wrong number of arguments. "
                     "Expected 7, got "
                  << argc - 1 << ". Stopped." << std::endl;
        printhelp();
        exit(3);
    }

    // read parameter
    std::string ArgThisProgram(argv[0]);
    const Channel_t ArgChDet{std::min(atoi(argv[1]), 7u)};
    const Channel_t ArgChSync = {std::min(atoi(argv[2]), 7u)};
    const Channel_t ArgChSemaphore{std::min(atoi(argv[3]), 7u)};
    const Channel_t ArgChMonitor = {std::min(atoi(argv[4]), 7u)};
    std::string ArgFilename(argv[5]);
    const uint64_t argbins = atoll(argv[6]);
    const Mode_t ArgMode = std::min(atoi(argv[7]), 2u);

    switch (ArgMode) {
    case 1:
        mode = Modeselector_t::infomode;
        break;
    case 2:
        mode = Modeselector_t::histogrammode;
    }

    const bool MonitorStatisticEnabled{ArgChMonitor < 4u};

    std::cout << "Read file " << ArgFilename << "\n"
              << "# Histogram with " << static_cast<unsigned int>(argbins)
              << " bins"
              << "\n"
              << "# Configuration of the input channels: \n"
              << "# ChDet:" << static_cast<unsigned int>(ArgChDet) << "\n"
              << "# ChSync:" << static_cast<unsigned int>(ArgChSync) << "\n"
              << "# ChSemaphore:" << static_cast<unsigned int>(ArgChSemaphore)
              << "\n"
              << "# ChMonitor:" << static_cast<unsigned int>(ArgChMonitor)
              << "\n"
              << std::endl;

    std::ifstream ifs(ArgFilename, std::ifstream::in);

    if (!ifs) {
        std::cerr << "ERROR: Could not open file." << std::endl;
        exit(1);
    }

    TimestampClass StartOffset_ns{0};
    TimestampClass currentts_ns{0};
    uint16_t trigid{0};
    uint16_t dataid{0};
    uint16_t data{0};

    TimestampClass ChSyncSumts_ns{0};

    TimestampClass avg_sync_ns{0};
    TimestampClass lastsync_ns{0};
    TimestampClass timesincelastsync_ns{0};
    TimestampClass MindSYNC_ns{0xffff'ffff'ffff'ffff};
    TimestampClass MaxdSYNC_ns{0};

    Counter_t QtyDetEvents{0};
    Counter_t QtySyncEvents{0};
    Counter_t QtySemaphoreEvents{0};
    Counter_t QtyMonitorEvents{0};

    // calculate mean time between SYNC
    while (ifs >> currentts_ns >> trigid >> dataid >> data) {
        if (0 == StartOffset_ns) {
            StartOffset_ns = currentts_ns;
        }
        currentts_ns = currentts_ns - StartOffset_ns;

        if (7 == trigid) {
            if (ArgChDet == dataid)
                QtyDetEvents++;
            else if (ArgChMonitor == dataid)
                QtyMonitorEvents++;
            else if (ArgChSemaphore == dataid)
                QtySemaphoreEvents++;
            else if (ArgChSync == dataid) {
                QtySyncEvents++;
                timesincelastsync_ns = currentts_ns - lastsync_ns;
                ChSyncSumts_ns = ChSyncSumts_ns + timesincelastsync_ns;
                lastsync_ns = currentts_ns;
                if (lastsync_ns > 0) {
                    if (Modeselector_t::infomode == mode) {
                        std::cout << currentts_ns - lastsync_ns
                                  << " ns period between " << lastsync_ns
                                  << " ns and " << currentts_ns << " ns"
                                  << std::endl;
                        MindSYNC_ns =
                            std::min(MindSYNC_ns, timesincelastsync_ns);
                        MaxdSYNC_ns =
                            std::max(MaxdSYNC_ns, timesincelastsync_ns);
                    }
                }
            }

        } // end of if (7 == trigid)
    }
    ifs.clear(); // reset EOF flag

    if (QtySyncEvents < 2) {
        std::cerr << "WARNING: Found only "
                  << static_cast<unsigned int>(QtySyncEvents)
                  << " SYNC signals on channel "
                  << static_cast<unsigned int>(ArgChSync) << "\n"
                  << "WARNING: Expected at least 2 SYNC signals." << std::endl;
    } else {
        avg_sync_ns = ChSyncSumts_ns / (QtySyncEvents - 1);

        std::cout
            << "# Start offset ts: "
            << static_cast<unsigned int>(StartOffset_ns) << "\n"
            << "# Number of events by category:\n"
            << "# Detector:  " << static_cast<unsigned int>(QtyDetEvents)
            << "\n"
            << "# Sync:      " << static_cast<unsigned int>(QtySyncEvents)
            << "\n"
            << "# Semaphore: " << static_cast<unsigned int>(QtySemaphoreEvents)
            << "\n"
            << "# Monitor:   " << static_cast<unsigned int>(QtyMonitorEvents)
            << "\n"

            << "# avg SYNC period: " << static_cast<unsigned int>(avg_sync_ns)
            << " ns = " << integermilliseconds(avg_sync_ns) << " ms\n"
            << "# min SYNC period: " << static_cast<unsigned int>(MindSYNC_ns)
            << " ns = " << integermilliseconds(MindSYNC_ns) << " ms\n"
            << "# max SYNC period: " << static_cast<unsigned int>(MaxdSYNC_ns)
            << " ns = " << integermilliseconds(MaxdSYNC_ns) << " ms"
            << std::endl;
    }

    if (Modeselector_t::histogrammode == mode) {
        ifs.seekg(0, ifs.beg); // go to file start again

        lastsync_ns = 0; // set time t0

        histo::Histogram histoDet(argbins, avg_sync_ns / argbins);
        histo::Histogram histoMon(argbins, avg_sync_ns / argbins);

        std::cout << histoDet.binsstring() << std::endl;
        TimestampClass timesincesync_ns{0};

        while (ifs >> currentts_ns >> trigid >> dataid >> data) {
            if (currentts_ns < StartOffset_ns) {
                std::cerr << "ERROR: Event with timestamp "
                          << static_cast<unsigned int>(currentts_ns)
                          << " was earlier than the start time "
                          << static_cast<unsigned int>(StartOffset_ns)
                          << std::endl;
            };

            currentts_ns = currentts_ns - StartOffset_ns;
            timesincesync_ns = currentts_ns - lastsync_ns;

            if (7u == trigid) {
                if (ArgChDet == dataid)
                    histoDet.put(timesincesync_ns); // found a detector event
                else if (ArgChMonitor == dataid)
                    histoMon.put(timesincesync_ns); // found a monitor event
                else if (ArgChSync == dataid)
                    lastsync_ns = currentts_ns; // found a SYNC event
                else if (ArgChSemaphore == dataid) {
                    std::cout << histoDet.frequencystring() << std::endl;
                    if (MonitorStatisticEnabled) {
                        std::cout << histoMon.frequencystring() << std::endl;
                    }
                    std::cerr << "\nSEMAPHORE on POS: " << ifs.tellg()
                              << " found. \n";
                    histoDet.clear();
                    histoMon.clear();
                }
            } // end of if (7 == trigid)

        } // end of while
    }

    ifs.close();
    return (EXIT_SUCCESS);
}
