#include "Lmfile.h"
#include "Bitslicer.h"
#include "Mdatevent.h"
#include "errorcodes.h"

#include <cstdint> // int8_t
#include <filesystem>
#include <fstream>  // std::ifstream
#include <iostream> // std::cout
#include <istream>
#include <string>
#include <vector>

namespace mfile {

Lmbuffer::Lmbuffer(std::vector<uint16_t> rawbuffer) {
    this->bufferlengthinwords =
        bitslicer::byteswap(rawbuffer[0]);
    this->buffertype = bitslicer::byteswap(rawbuffer[1]);
    this->headerlengthinwords =
        bitslicer::byteswap(rawbuffer[2]);
    this->buffernumber = bitslicer::byteswap(rawbuffer[3]);
    this->runid        = bitslicer::byteswap(rawbuffer[4]);
    this->mcpdid =
        (bitslicer::byteswap(rawbuffer[5]) & (0xFF00)) >> 8;
    this->status =
        bitslicer::byteswap(rawbuffer[5]) & (0x00FF);

    uint16_t htsLow  = bitslicer::byteswap(rawbuffer[6]);
    uint16_t htsMid  = bitslicer::byteswap(rawbuffer[7]);
    uint16_t htsHigh = bitslicer::byteswap(rawbuffer[8]);

    this->headertimestamp_ns =
        bitslicer::LowMidHigh(htsLow, htsMid, htsHigh) *
        100;
}

Lmbuffer::~Lmbuffer() {}

uint16_t Lmbuffer::getbufferlengthinwords() {
    return (this->bufferlengthinwords);
}

uint16_t Lmbuffer::getheaderlengthinwords() {
    return (this->headerlengthinwords);
}

TimestampClass Lmbuffer::getheadertimestamp_ns() {
    return (this->headertimestamp_ns);
}

uint16_t Lmbuffer::getrunid() { return (this->runid); }

Lmfile::Lmfile(std::string const mypath,
               uint8_t           myverbositylevel)
    : ifs(mypath,
          std::ifstream::ate | std::ifstream::binary),
      filesize(0), firsttimestamp_ns(0),
      verbositylevel(myverbositylevel) {

    filesize = std::filesystem::file_size(mypath);

    if (filesize < 134) {
        std::cerr << "Size of file: " << filesize
                  << " Bytes. \n";
        throw std::runtime_error{error_003_filetoosmall};
    }
    ifs.seekg(0, ifs.beg);
}

Lmfile::~Lmfile() {}

uint64_t Lmfile::read64bit() {
    uint64_t sequenceRAW = 0;
    ifs.read(reinterpret_cast<char *>(&sequenceRAW), 8);
    return (bitslicer::byteswap64(sequenceRAW));
}

uint64_t Lmfile::getsortedevent() {
    uint16_t Low{0};
    uint16_t Mid{0};
    uint16_t High{0};

    ifs.read(reinterpret_cast<char *>(&Low), 2);
    ifs.read(reinterpret_cast<char *>(&Mid), 2);
    ifs.read(reinterpret_cast<char *>(&High), 2);

    return (bitslicer::LowMidHigh(
        bitslicer::byteswap(Low), bitslicer::byteswap(Mid),
        bitslicer::byteswap(High)));
}

filesize_t Lmfile::getposition() { return (ifs.tellg()); }

void Lmfile::convertlistmodefile() {
    this->jumpbehindfileheader();
    std::cerr << "\n getposition:" << this->getposition()
              << "\n"
              << std::endl;
    this->readheadersignature();
    std::cerr << "\n getposition:" << this->getposition()
              << "\n"
              << std::endl;
    std::vector<uint16_t> bhwords;
    uint16_t              numberofevents{0};
    TimestampClass        bufferoffset_ns{0};

    while (getbytestillEOF() > (40 + 8 + 8)) {
        bhwords = this->getbufferheader();

        mfile::Lmbuffer mybuffer{bhwords};
        numberofevents =
            (mybuffer.getbufferlengthinwords() -
             mybuffer.getheaderlengthinwords()) /
            3;
        bufferoffset_ns = mybuffer.getheadertimestamp_ns();

        for (uint16_t i = 0; i < numberofevents; i++) {
            mevent::Mdatevent myevent = mevent::Mdatevent(
                this->getsortedevent(), bufferoffset_ns);
            std::cout << myevent.printevent() << std::endl;
        }
        this->readbuffersignature();
    }
    this->readfilesignature();
}

std::vector<uint16_t> Lmfile::getbufferheader() {
    std::vector<uint16_t> mybuf(21);
    uint16_t              aword{0};

    for (int i = 0; i < mybuf.size(); ++i) {
        ifs.read(reinterpret_cast<char *>(&aword), 2);
        mybuf[i] = aword;
    }
    return (mybuf);
}

void Lmfile::jumpbehindfileheader() {
    // read first 2 lines of the file
    // parse second line with length of header
    // return length of header in number of lines
    std::string thisline;
    std::getline(ifs, thisline);

    if (thisline != "mesytec psd listmode data")
        throw std::runtime_error{error_001_noheader};

    std::getline(ifs,
                 thisline); // header length: nnnnn lines
    uint64_t    numberofheaderlines = thisline.find(": ");
    std::string sustri              = thisline.substr(
        numberofheaderlines + 1, numberofheaderlines + 4);

    uint64_t fileHeaderLength =
        std::stoi(sustri, nullptr, 10);

    std::string commentline;
    for (auto i = 2; i < fileHeaderLength; ++i) {
        std::getline(ifs, commentline);
    }
}

void Lmfile::readheadersignature() {
    const signature_t headersignature = 0x00005555AAAAFFFF;
    uint64_t          sequenceRAW     = Lmfile::read64bit();
    if (headersignature != sequenceRAW)
        throw std::runtime_error{error_005_noheadersig};
}

void Lmfile::readbuffersignature() {
    const signature_t buffersignature = 0x0000FFFF5555AAAA;
    uint64_t          sequenceRAW     = Lmfile::read64bit();
    if (buffersignature != sequenceRAW)
        throw std::runtime_error{error_006_nodatasig};
}

void Lmfile::readfilesignature() {
    const signature_t filesignature = 0xFFFFAAAA55550000;
    uint64_t          sequenceRAW   = Lmfile::read64bit();
    if (filesignature != sequenceRAW)
        throw std::runtime_error{error_007_noeofsig};
}

void Lmfile::setverbosity(uint8_t myverbositylevel) {
    this->verbositylevel = myverbositylevel;
}

uint8_t Lmfile::getverbosity() {
    return (this->verbositylevel);
}

filesize_t Lmfile::getfilesize() {
    return (this->filesize);
}

filesize_t Lmfile::getbytestillEOF() {
    return (this->filesize - ifs.tellg());
}

} /* namespace mfile */
