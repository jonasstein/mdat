//==============================================
// Name        : mdathistogram.cpp
// Author      : Jonas Stein
// Description : Main program to generate
//               histogram like structured files
//               from extracted list mode data.
//
// Options:
//  -h [ --help ]               Help screen
//  -f [ --filename ] arg       Name of the input file
//  -d [ --detector ] arg (=0)  Channel of detector signals  (0..3, default = 0)
//  -s [ --sync ] arg (=1)      Channel of sync signals      (0..3, default = 1)
//  -S [ --semaphore ] arg (=2) Channel of semaphore signals (0..3, default = 2)
//  -m [ --monitor ] arg (=3)   Channel of monitor signals   (0..3, default = 3)
//  -b [ --bins ] arg (=100)    Number of bins in the histogram  (default = 100)
//  -i [ --info ]               Calculate and print only statistical information
//  -N [ --nomonitor ]          Disable monitor statistics in the histogram
//  --histogram                 Calculate the histogram
//==============================================

#include "Histogram.h"
#include "Timestamps.h"
#include <algorithm> //std::min
#include <algorithm>
#include <boost/program_options.hpp>
#include <cstdint>
#include <fstream>  // std::ifstream
#include <iostream> // std::cout
#include <iostream>
#include <istream>
#include <iterator>
#include <stdio.h>
#include <string> // std::string, std::stoull
#include <vector>

using Channel_t = uint16_t;
using Mode_t    = uint16_t;
using Counter_t = uint64_t;
using Bins_t    = uint32_t;

enum class Modeselector_t : uint16_t { infomode = 1, histogrammode = 2 };
Modeselector_t mode{Modeselector_t::infomode};

namespace po = boost::program_options;

int main(int argc, char *argv[]) {

    Channel_t ArgChDet{7};
    Channel_t ArgChSync{7};
    Channel_t ArgChSemaphore{7};
    Channel_t ArgChMonitor{7};

    Bins_t      ArgBins{100};
    std::string ArgFilename{};

    po::options_description desc{"Options"};
    desc.add_options()("help,h", "Help screen");
    desc.add_options()("filename,f", po::value<std::string>(&ArgFilename),
                       "Name of the input file");
    desc.add_options()("detector,d",
                       po::value<Channel_t>(&ArgChDet)->default_value(0),
                       "Channel of detector signals  (0..3, default = 0)");
    desc.add_options()("sync,s",
                       po::value<Channel_t>(&ArgChSync)->default_value(1),
                       "Channel of sync signals      (0..3, default = 1)");
    desc.add_options()("semaphore,S",
                       po::value<Channel_t>(&ArgChSemaphore)->default_value(2),
                       "Channel of semaphore signals (0..3, default = 2)");
    desc.add_options()("monitor,m",
                       po::value<Channel_t>(&ArgChMonitor)->default_value(3),
                       "Channel of monitor signals   (0..3, default = 3)");
    desc.add_options()("bins,b",
                       po::value<Bins_t>(&ArgBins)->default_value(100),
                       "Number of bins in the histogram  (default = 100)");
    desc.add_options()("info,i",
                       "Calculate and print only statistical information");
    desc.add_options()("nomonitor,N",
                       "Disable monitor statistics in the histogram");
    desc.add_options()("histogram", "Calculate the histogram");

    po::command_line_parser parser{argc, argv};
    parser.options(desc).allow_unregistered().style(
        po::command_line_style::default_style);
    po::parsed_options parsed_options = parser.run();

    po::variables_map vm{};
    store(parsed_options, vm);
    notify(vm);

    if (vm.count("help")) {
        std::cout << "mkhistogram by Jonas Stein (2016-2019)\n\n"
                  << desc << std::endl;
        return (EXIT_SUCCESS);
    } else if (vm.count("info"))
        mode = Modeselector_t::infomode;
    else if (vm.count("histogram"))
        mode = Modeselector_t::histogrammode;

    const bool MonitorStatisticEnabled{(vm.count("nomonitor") == 0)};

    std::cout << "# Read file " << ArgFilename << "\n"
              << "# Histogram with " << static_cast<unsigned int>(ArgBins)
              << " bins\n"
              << "# Configuration of the input channels:\n"
              << "# ChDet:       " << static_cast<unsigned int>(ArgChDet)
              << "\n"
              << "# ChSync:      " << static_cast<unsigned int>(ArgChSync)
              << "\n"
              << "# ChSemaphore: " << static_cast<unsigned int>(ArgChSemaphore)
              << "\n"
              << "# ChMonitor:   " << static_cast<unsigned int>(ArgChMonitor)
              << std::endl;

    std::ifstream ifs(ArgFilename, std::ifstream::in);

    if (!ifs) {
        std::cerr << "ERROR: Could not open file." << std::endl;
        return (1);
    }

    TimestampClass StartOffset_ns{0};
    TimestampClass currentts_ns{0};
    uint16_t       trigid{0};
    uint16_t       dataid{0};
    uint32_t       data{0}; // counter 0 .. 2^19-1

    TimestampClass ChSyncSumts_ns{0};

    TimestampClass avg_sync_ns{0};
    TimestampClass lastsync_ns{0};
    TimestampClass timesincelastsync_ns{0};

    TimestampClass MaxdSYNC_ns{0x0};
    TimestampClass MindSYNC_ns{0xFFFFFFFFFFFFFFFF};

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
            if ((ArgChDet == dataid) && (0 < QtySyncEvents))
                QtyDetEvents++;
            else if ((ArgChMonitor == dataid) && (0 < QtySyncEvents))
                QtyMonitorEvents++;
            else if ((ArgChSemaphore == dataid) && (0 < QtySyncEvents))
                QtySemaphoreEvents++;
            else if ((ArgChSync == dataid) && (0 == QtySyncEvents)) {
                QtySyncEvents++;
                lastsync_ns = currentts_ns;
            } else if ((ArgChSync == dataid) && (0 < QtySyncEvents)) {
                QtySyncEvents++;
                timesincelastsync_ns = currentts_ns - lastsync_ns;
                ChSyncSumts_ns       = ChSyncSumts_ns + timesincelastsync_ns;
                lastsync_ns          = currentts_ns;
                if (Modeselector_t::infomode == mode) {
                    std::cout << timesincelastsync_ns << " ns duration between "
                              << lastsync_ns << " ns and " << currentts_ns
                              << " ns" << std::endl;
                }
                MindSYNC_ns = std::min(MindSYNC_ns, timesincelastsync_ns);
                MaxdSYNC_ns = std::max(MaxdSYNC_ns, timesincelastsync_ns);
            }

        } // end of if (7 == trigid)
    }
    ifs.clear(); // reset EOF flag

    if (QtySyncEvents < 2) {
        std::cerr << "WARNING: Found only " << QtySyncEvents
                  << " SYNC signals on channel " << ArgChSync << "\n"
                  << "WARNING: Expected at least 2 SYNC signals." << std::endl;
    } else {
        avg_sync_ns = ChSyncSumts_ns / (QtySyncEvents - 1);

        std::cout << "# Start offset ts: " << StartOffset_ns << "\n"
                  << "# Number of events by category:\n"
                  << "# Detector:  " << QtyDetEvents << "\n"
                  << "# Sync:      " << QtySyncEvents << "\n"
                  << "# Semaphore: " << QtySemaphoreEvents << "\n"
                  << "# Monitor:   " << QtyMonitorEvents << "\n"
                  << "# avg SYNC period: " << avg_sync_ns
                  << " ns = " << integermilliseconds(avg_sync_ns) << " ms\n"
                  << "# min SYNC period: " << MindSYNC_ns
                  << " ns = " << integermilliseconds(MindSYNC_ns) << " ms\n"
                  << "# max SYNC period: " << MaxdSYNC_ns
                  << " ns = " << integermilliseconds(MaxdSYNC_ns) << " ms \n"
                  << "# MonitorStatistic: ";
        if (MonitorStatisticEnabled)
            std::cout << "enabled\n";
        else
            std::cout << "disabled\n";
    }

    if (Modeselector_t::histogrammode == mode) {
        ifs.seekg(0, ifs.beg); // go to file start again

        lastsync_ns = 0; // set time t0

        histo::Histogram histoDet(ArgBins, avg_sync_ns / ArgBins);
        histo::Histogram histoMon(ArgBins, avg_sync_ns / ArgBins);

        std::cout << histoDet.binsstring() << std::endl;
        TimestampClass timesincesync_ns{0};

        bool bufferempty{true};

        while (ifs >> currentts_ns >> trigid >> dataid >> data) {
            if (currentts_ns < StartOffset_ns) {
                std::cerr << "ERROR: Event with timestamp " << currentts_ns
                          << " was earlier than the start time "
                          << StartOffset_ns << std::endl;
            };

            currentts_ns     = currentts_ns - StartOffset_ns;
            timesincesync_ns = currentts_ns - lastsync_ns;

            if (7 == trigid) {
                bufferempty = false;

                if (ArgChDet == dataid)
                    histoDet.put(timesincesync_ns); // found a
                                                    // detector event
                else if (ArgChMonitor == dataid)
                    histoMon.put(timesincesync_ns); // found a
                                                    // monitor event
                else if (ArgChSync == dataid)
                    lastsync_ns = currentts_ns; // found a SYNC event
                else if (ArgChSemaphore == dataid) {
                    std::cout << histoDet.frequencystring() << std::endl;
                    if (MonitorStatisticEnabled) {
                        std::cout << histoMon.frequencystring() << std::endl;
                    }
                    histoDet.clear();
                    histoMon.clear();
                    bufferempty = true;
                }
            } // end of if (7 == trigid)
        }     // end of while
        if (!bufferempty) {
            std::cout << histoDet.frequencystring() << std::endl;
            if (MonitorStatisticEnabled) {
                std::cout << histoMon.frequencystring() << std::endl;
            }
            histoDet.clear();
            histoMon.clear();
            bufferempty = true;
        }
    }

    ifs.close();
    return (EXIT_SUCCESS);
}
