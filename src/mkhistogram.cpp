#include "Histogram.h"
#include "Globaltypes.h"
#include <algorithm> //std::min
#include <cstdint>
#include <fstream> // std::ifstream
#include <iostream> // std::cout
#include <istream>
#include <stdio.h>
#include <string> // std::string, std::stoull

using Channel_t = uint8_t;
using Mode_t = uint8_t;
using TimeStamp_t = uint64_t;
using Counter_t = uint64_t;

void printhelp ()
{
    std::cout
    << "mkhistogram by Jonas Stein (2016-2019) \n"
    << "Usage: mkhistogram <ChDet> <ChSync> "
       "<ChSemaphore> <ChMonitor> "
       "<filename> <bins> <mode> \n"
    << "Only ChMonitor 0..3 will be printed \n"
    << "mode = 1 infomode, 2 histogram" << std::endl;
}

int main (int argc, char *argv[])
{

    if (argc != 8)
    {
        std::cerr << "Error wrong number of arguments. "
                     "Expected 7, got "
                  << argc - 1 << ". Stopped." << std::endl;
        printhelp ();
        exit (3);
    }

    // read parameter
    std::string ArgThisProgram (argv[0]);
    Channel_t ArgChDet = std::min (atoi (argv[1]), 7);
    Channel_t ArgChSync = 1; // strtoull(argv[2],NULL,10);

    Channel_t ArgChSemaphore = std::min (atoi (argv[3]), 7);
    Channel_t ArgChMonitor = std::min (atoi (argv[4]), 7);
    std::string ArgFilename (argv[5]);
    uint64_t ArgBins = atol (argv[6]);
    Mode_t ArgMode = std::min (atoi (argv[7]), 2); // 1=get info about periods, 2=generate histogram

    const Mode_t INFOMODE = 1;
    const Mode_t HISTOGRAMMODE = 2;

    std::cerr << "Read file " << ArgFilename << std::endl;
    std::cerr << "Generate histogram with "
              << static_cast<unsigned int> (ArgBins)
              << " bins" << std::endl;
    std::cerr
    << "ChDet:" << static_cast<unsigned int> (ArgChDet)
    << " ChSync:" << static_cast<unsigned int> (ArgChSync)
    << " ChSemaphore:" << static_cast<unsigned int> (ArgChSemaphore)
    << " ChMonitor:" << static_cast<unsigned int> (ArgChMonitor)
    << std::endl;

    std::ifstream ifs;
    ifs.open (ArgFilename, std::ifstream::in);

    if (!ifs)
    {
        std::cerr << "ERROR: Could not open file." << std::endl;
        exit (1);
    }

    // read .csv, read line, if not comment,
    // if SYNC, then save new SYNC, inc SYNCcounter
    // calculate mean(SYNC(k))
    // go to start of file
    // read line, if not comment,
    // if not SEMAPHORE read CURRENTts and printf
    // if SEMAPHORE then histo.print(); histo.reset()

    TimeStamp_t StartOffsetts = 0;
    TimeStamp_t CURRENTts = 0;
    uint16_t TrigID = 0;
    uint16_t DataID = 0;
    uint16_t Data = 0;

    TimeStamp_t SYNCtsSUM = 0;
    Counter_t SYNCtsQty = 0;
    TimeStamp_t SYNCtsMEAN = 0;
    TimeStamp_t LastSYNCts = 0;
    TimeStamp_t MindSYNCts = 0xffff'ffff'ffff'ffff;
    TimeStamp_t MaxdSYNCts = 0;

    bool FirstPrintOut = true;

    // calculate mean time between SYNC
    while (ifs >> CURRENTts >> TrigID >> DataID >> Data)
    {
        if (0 == StartOffsetts)
        {
            StartOffsetts = CURRENTts;
        }
        CURRENTts -= StartOffsetts;

        if ((7 == TrigID) && (DataID == ArgChSync))
        { // found a SYNC event
            if (LastSYNCts > 0)
            {
                if (ArgMode == INFOMODE)
                {
                    std::cout << CURRENTts - LastSYNCts
                              << " ns period between " << LastSYNCts
                              << " ns and " << CURRENTts
                              << " ns" << std::endl;
                    MindSYNCts =
                    std::min (MindSYNCts, CURRENTts - LastSYNCts);
                    MaxdSYNCts =
                    std::max (MaxdSYNCts, CURRENTts - LastSYNCts);
                }
            }
            SYNCtsSUM += (CURRENTts - LastSYNCts);
            SYNCtsQty++;
            LastSYNCts = CURRENTts;
        }
    }
    ifs.clear (); // reset EOF flag

    if (SYNCtsQty < 2)
    {
        std::cerr
        << "WARNING: Found only "
        << static_cast<unsigned int> (SYNCtsQty)
        << " SYNC signals on channel "
        << static_cast<unsigned int> (ArgChSync) << "\n"
        << "WARNING: Expected at least 2 SYNC signals."
        << std::endl;
    }
    else
    {
        SYNCtsMEAN = SYNCtsSUM / (SYNCtsQty - 1);

        std::cout
        << "# Start offset ts: "
        << static_cast<unsigned int> (StartOffsetts) << "\n"
        << "# SYNC event found: "
        << static_cast<unsigned int> (SYNCtsQty) << "\n"
        << "# avg SYNC period: "
        << static_cast<unsigned int> (SYNCtsMEAN)
        << " ns = " << (float)(SYNCtsMEAN) / 1000000 << " ms\n"
        << "# min SYNC period: "
        << static_cast<unsigned int> (MindSYNCts)
        << " ns = " << (float)(MindSYNCts) / 1000000 << " ms\n"
        << "# max SYNC period: "
        << static_cast<unsigned int> (MaxdSYNCts)
        << " ns = " << (float)(MaxdSYNCts) / 1000000
        << " ms" << std::endl;
    }

    std::string::size_type sz = 0; // alias of size_t

    if (HISTOGRAMMODE == ArgMode)
    {
        ifs.seekg (0, ifs.beg); // go to file start again

        LastSYNCts = 0; // set time t0

        histo::Histogram histoDet(ArgBins, SYNCtsMEAN / ArgBins);
        histo::Histogram histoMon(ArgBins, SYNCtsMEAN / ArgBins);

        while (ifs >> CURRENTts >> TrigID >> DataID >> Data)
        {

            if (CURRENTts < StartOffsetts)
            {
                std::cerr
                << "ERROR: Event with timestamp "
                << static_cast<unsigned int> (CURRENTts)
                << " was earlier than the start time "
                << static_cast<unsigned int> (StartOffsetts)
                << std::endl;
            };
            CURRENTts -= StartOffsetts;

            TimeStamp_t buffer = 0;

            if ((7 == TrigID) && (DataID == ArgChDet))
            { // found a detector event
                buffer = (CURRENTts - LastSYNCts);
                histoDet.put(buffer);
            }

            if ((7 == TrigID) && (DataID == ArgChMonitor) &&
                (ArgChMonitor < 4))
            { // found a monitor event
                buffer = (CURRENTts - LastSYNCts);
                histoMon.put(buffer);
            }

            if ((7 == TrigID) && (DataID == ArgChSync))
            { // found a SYNC event
                LastSYNCts = CURRENTts;
            }

            if ((7 == TrigID) && (DataID == ArgChSemaphore))
            { // found a SEMAPHORE event (new
              // histogram/new scan)
                if (FirstPrintOut)
                {
                	std::cout << histo::vectortostring(histoDet.getbins()) << std::endl;
                    if (ArgChMonitor < 4)
                    {
                    	std::cout << histo::vectortostring(histoMon.getbins()) << std::endl;
                    } // suppress output of empty monitor histograms
                    FirstPrintOut = false;
                }
                std::cout << histo::vectortostring(histoDet.getfrequency()) << std::endl;
                histoDet.clear();
                if (ArgChMonitor < 4)
                {
                	std::cout << histo::vectortostring(histoMon.getbins()) << std::endl;
                    histoMon.clear();
                }
            }
        }

        std::cout << histo::vectortostring(histoDet.getfrequency()) << std::endl;

        if (ArgChMonitor < 4)
        {
        	std::cout << histo::vectortostring(histoMon.getbins()) << std::endl;
        } // suppress output of empty monitor histograms

    }

    ifs.close ();
    return (EXIT_SUCCESS);
}
