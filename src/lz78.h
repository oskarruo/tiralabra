#ifndef LZ78_H
#define LZ78_H

#include <string>
#include <list>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

int lz_compress(string input, string output_filename);

int lz_decompress(fs::path input_filename, string output_filename);

#endif