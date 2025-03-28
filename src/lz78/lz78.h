#ifndef LZ78_H
#define LZ78_H

#include <filesystem>
#include <list>
#include <string>
using namespace std;
namespace fs = std::filesystem;

void lz_compress(fs::path input_filename, string output_filename);

void lz_decompress(fs::path input_filename, string output_filename);

#endif