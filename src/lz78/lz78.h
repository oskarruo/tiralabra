#ifndef LZ78_H
#define LZ78_H

#include <string>
#include <list>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

void lz_compress(string input, string output_filename);

void lz_decompress(fs::path input_filename, string output_filename);

#endif