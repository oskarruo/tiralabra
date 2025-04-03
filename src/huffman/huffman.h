#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <filesystem>
#include <list>
#include <string>
using namespace std;
namespace fs = std::filesystem;

// Compresses a file using Huffman coding and writes to a .bin file.
void huff_compress(fs::path input_filename, string output_filename);

// Decompresses a .bin file using Huffman coding and writes to a .txt file.
void huff_decompress(fs::path input_filename, string output_filename);

#endif