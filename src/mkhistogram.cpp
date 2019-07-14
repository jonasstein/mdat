#include "histolong.hpp"
#include <assert.h> /* assert */
#include <cstdint>
#include <fstream>  // std::ifstream
#include <iostream> // std::cout
#include <istream>
#include <stdio.h>
#include <string> // std::string, std::stoull

void printhelp() {
  std::cout << "mkhistogram by Jonas Stein (2016-2019) \n"
            << "Usage: mkhistogram <ChDet> <ChSync> <ChSemaphore> <ChMonitor> <filename> <bins> <mode> \n"
            << "Only ChMonitor 0..3 will be printed \n"
            << "mode = 1 infomode, 2 histogram" << std::endl;
}

int main(int argc, char *argv[]) {

  if (argc != 8) {
    std::cerr << "Error wrong number of arguments. Expected 7, got " << argc - 1
              << ". Stopped." << std::endl;
    printhelp();
    exit(3);
  }

  const long long INFOMODE = 1;
  const long long HISTOGRAMMODE = 2;
  const long long MAX64INT = 9223372036854775807;

  // read parameter
  std::string ArgThisProgram(argv[0]);
  long long ArgChDet = atol(argv[1]);
  long long ArgChSync = atol(argv[2]);
  long long ArgChSemaphore = atol(argv[3]);
  long long ArgChMonitor = atol(argv[4]);
  std::string ArgFilename(argv[5]);
  long long ArgBins = atol(argv[6]);
  long long ArgMode =
      atol(argv[7]); // 1=get info about periods, 2=generate histogram

  std::cerr << "Read file " << ArgFilename << std::endl;
  std::cerr << "Generate histogram with " << ArgBins << " bins" << std::endl;
  std::cerr << "ChDet:" << ArgChDet << " ChSync:" << ArgChSync
            << " ChSemaphore:" << ArgChSemaphore
            << " ChMonitor:" << ArgChMonitor << std::endl;

  std::ifstream ifs;
  ifs.open(ArgFilename, std::ifstream::in);

  if (!ifs) // file couldn't be opened
  {
    std::cerr << "ERROR: Could not open file." << std::endl;
    exit(1);
  }

  // read .csv, read line, if not comment,
  // if SYNC, then save new SYNC, inc SYNCcounter
  // calculate mean(SYNC(k))
  // go to start of file
  // read line, if not comment,
  // if not SEMAPHORE read CURRENTts and printf
  // if SEMAPHORE then histo.print(); histo.reset()

  long long StartOffsetts = 0;
  long long CURRENTts = 0;
  long long SYNCtsSUM = 0;
  long long SYNCtsQty = 0;
  long long SYNCtsMEAN = 0;
  long long LastSYNCts = 0;
  long long MindSYNCts = MAX64INT;
  long long MaxdSYNCts = 0;

  long long TrigID = 0;
  long long DataID = 0;
  long long Data = 0;

  bool FirstPrintOut = true;

  // calculate mean time between SYNC
  while (!ifs.eof()) {
    ifs >> CURRENTts >> TrigID >> DataID >> Data;
    if (0 == StartOffsetts) {
      StartOffsetts = CURRENTts;
    }
    CURRENTts -= StartOffsetts;

    if ((7 == TrigID) && (DataID == ArgChSync)) { // found a SYNC event
      if (LastSYNCts > 0) {
        if (ArgMode == INFOMODE) {
          std::cout << CURRENTts - LastSYNCts << " ns period between "
                    << LastSYNCts << " ns and " << CURRENTts << " ns"
                    << std::endl;
          MindSYNCts = std::min(MindSYNCts, CURRENTts - LastSYNCts);
          MaxdSYNCts = std::max(MaxdSYNCts, CURRENTts - LastSYNCts);
        }
      }
      SYNCtsSUM += (CURRENTts - LastSYNCts);
      SYNCtsQty++;
      LastSYNCts = CURRENTts;
    }
  }

  if (0 == SYNCtsQty) {
    std::cerr << "WARNING: No SYNC signals found on channel " << ArgChSync << "!"
              << std::endl;
  } else {
    SYNCtsMEAN = SYNCtsSUM / SYNCtsQty;

    std::cout << "# Start offset ts: " << StartOffsetts << std::endl;
    std::cout << "# SYNC event found: " << SYNCtsQty << std::endl;
    std::cout << "# avg SYNC period: " << SYNCtsMEAN
              << " ns = " << (float)(SYNCtsMEAN) / 1000000 << " ms "
              << std::endl;
    std::cout << "# min SYNC period: " << MindSYNCts
              << " ns = " << (float)(MindSYNCts) / 1000000 << " ms "
              << std::endl;
    std::cout << "# max SYNC period: " << MaxdSYNCts
              << " ns = " << (float)(MaxdSYNCts) / 1000000 << " ms "
              << std::endl;
  }

  long long buffer;
  std::string::size_type sz = 0; // alias of size_t
  ifs.clear();

  if (HISTOGRAMMODE == ArgMode) {
    ifs.seekg(0, ifs.beg); // go to file start again

    LastSYNCts = 0; // set time t0

    histogram *histoDet;
    histoDet = new histogram(ArgBins, SYNCtsMEAN / ArgBins);

    histogram *histoMon;
    histoMon = new histogram(ArgBins, SYNCtsMEAN / ArgBins);

    while (!ifs.eof()) {
      ifs >> CURRENTts >> TrigID >> DataID >> Data;

      assert(CURRENTts >= StartOffsetts);
      CURRENTts -= StartOffsetts;

      if ((7 == TrigID) && (DataID == ArgChDet)) { // found a detector event
        buffer = (CURRENTts - LastSYNCts);
        histoDet->put(buffer);
      }

      if ((7 == TrigID) && (DataID == ArgChMonitor) &&
          (ArgChMonitor < 4)) { // found a monitor event
        buffer = (CURRENTts - LastSYNCts);
        histoMon->put(buffer);
      }

      if ((7 == TrigID) && (DataID == ArgChSync)) { // found a SYNC event
        LastSYNCts = CURRENTts;
      }

      if ((7 == TrigID) &&
          (DataID == ArgChSemaphore)) { // found a SEMAPHORE event (new
                                        // histogram/new scan)
        if (FirstPrintOut) {
          histoDet->printheader();
          if (ArgChMonitor < 4) {
            histoMon->printheader();
          } // suppress output of empty monitor histograms
          FirstPrintOut = false;
        }
        histoDet->print();
        histoDet->reset();
        if (ArgChMonitor < 4) {
          histoMon->printheader();
          histoMon->reset();
        }
      }
    }

    histoDet->print();
    delete (histoDet);
    if (ArgChMonitor < 4) {
      histoMon->printheader();
    } // suppress output of empty monitor histograms
    delete (histoMon);
  }

  ifs.close();
  return (EXIT_SUCCESS);
}