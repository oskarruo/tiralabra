#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "huffman/huffman.h"
#include "lz78/lz78.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "utils/getopt.h"
#undef byte
#else
#include <unistd.h>
#endif

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " [options] file" << endl;
    return 1;
  }

  int opt;
  string output_filename, algorithm;
  bool compress = false, decompress = false;

  while ((opt = getopt(argc, argv, "a:cdo:")) != -1) {
    switch (opt) {
      case 'a':
        algorithm = optarg;
        if (algorithm != "lz78" && algorithm != "huffman") {
          cerr << "Invalid algorithm" + algorithm << endl;
          return 1;
        }
        break;
      case 'c':
        compress = true;
        if (decompress) {
          cerr << "Cannot compress and decompress at the same time" << endl;
          return 1;
        }
        break;
      case 'd':
        decompress = true;
        if (compress) {
          cerr << "Cannot compress and decompress at the same time" << endl;
          return 1;
        }
        break;
      case 'o':
        output_filename = optarg;
        break;
      case '?':
        cerr << "Unknown or missing argument for option: " << char(optopt)
             << std::endl;
        return 1;
    }
  }

  if (optind >= argc) {
    cerr << "No input file specified" << endl;
    return 1;
  }

  fs::path input_filename = argv[optind];
  if (compress) {
    if (input_filename.string().substr(input_filename.string().length() - 4) !=
        ".txt") {
      cerr << "Input file must be a text file" << endl;
      return 1;
    }
  } else if (decompress) {
    if (input_filename.string().substr(input_filename.string().length() - 4) !=
        ".bin") {
      cerr << "Input file must be a binary file" << endl;
      return 1;
    }
  }

  if (algorithm.empty()) {
    cout << "No algorithm specified, using huffman" << endl;
    algorithm = "huffman";
  }
  if (!compress && !decompress) {
    cout << "No operation specified, defaulting to compress" << endl;
    compress = true;
  }

  if (output_filename.empty()) {
    if (compress) {
      output_filename = input_filename.stem().string() + "_compressed.bin";
    } else if (decompress) {
      output_filename = input_filename.stem().string() + "_decompressed.txt";
    }
  }

  if (algorithm == "lz78") {
    if (compress) {
      lz_compress(input_filename, output_filename);
    } else if (decompress) {
      lz_decompress(input_filename, output_filename);
    }
  } else if (algorithm == "huffman") {
    if (compress) {
      huff_compress(input_filename, output_filename);
    } else if (decompress) {
      huff_decompress(input_filename, output_filename);
    }
  }

  return 0;
}