#ifndef SRC_ERRORCODES_H_
#define SRC_ERRORCODES_H_

#include <string_view>

constexpr char error_001_noheader[] = "Error 001: list mode file header not found.";
constexpr char error_002_headernottwolines[] = "Error 002: Atypical header length. Header was expected to be 2 lines long.";
constexpr char error_003_filetoosmall[] = "Error 003: File is too small. An empty list mode file should have 133 Byte of meta data.";
constexpr char error_004_arguments[] = "Error 004: Wrong number of arguments.";
constexpr char error_005_noheadersig[] = "Error 005: Expected header signature after the file header. Broken file format.";
constexpr char error_006_nodatasig[] = "Error 006: Expected data block signature after data block. Broken file format.";
constexpr char error_007_noeofsig[] = "Error 007: Expected file signature at the end of the list mode file. Broken file format.";
constexpr char error_008_filenotfound[] = "Error 008: File not found.";

#endif /* SRC_ERRORCODES_H_ */
