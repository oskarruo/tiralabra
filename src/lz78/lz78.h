#ifndef LZ78_H
#define LZ78_H

#include <filesystem>
#include <list>
#include <string>
using namespace std;
namespace fs = std::filesystem;

// Compresses input using LZ78 algorithm and writes to a file.
void lz_compress(fs::path input_filename, string output_filename);

// Decompresses input using LZ78 algorithm and writes to a file.
void lz_decompress(fs::path input_filename, string output_filename);

#endif