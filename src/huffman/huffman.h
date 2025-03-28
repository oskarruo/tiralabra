#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <list>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

void huff_compress(fs::path input_filename, string output_filename);

void huff_decompress(fs::path input_filename, string output_filename);

#endif