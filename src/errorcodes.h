#ifndef SRC_ERRORCODES_H_
#define SRC_ERRORCODES_H_

#include <string>

static constexpr std::string error_001_noheader {"Error 001: list mode file header not found."};
static constexpr std::string error_002_headernottwolines {"Error 002: Atypical header length. Header was expected to be 2 lines long."};
static constexpr std::string error_003_filetoosmall {"Error 003: File is too small. An empty list mode file has 133 Byte of meta data."};

#endif /* SRC_ERRORCODES_H_ */
