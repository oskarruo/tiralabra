#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "huffman/huffman.h"
#include "lz78/lz78.h"
#include "utils/bitio.h"
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

/**
 * @brief The main function for the program that calls the
 * compression/decompression functions.
 *
 * @param argc Number of command line arguments.
 * @param argv Command line arguments.
 * @return int Exit status of the program.
 */
int cc(int argc, char* argv[]) {
  // If the user does not provide any arguments, print the usage message and
  // exit.
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " [options] file" << endl;
    return 1;
  }

  // Initialize variables for option count and arguments.
  int opt;
  string output_filename, algorithm;
  bool compress = false, decompress = false;

  // Parse command line arguments using getopt.
  while ((opt = getopt(argc, argv, "a:o:")) != -1) {
    switch (opt) {
      case 'a':
        // Set the algorithm to use for compression/decompression.
        algorithm = optarg;
        if (algorithm != "lz78" && algorithm != "huffman") {
          cerr << "Invalid algorithm: " + algorithm << endl;
          return 1;
        }
        break;
      case 'o':
        // Set the output filename for the compressed/decompressed file.
        output_filename = optarg;
        break;
    }
  }

  // Check that the user has provided an input file.
  if (optind >= argc) {
    cerr << "No input file specified" << endl;
    return 1;
  }

  // Get the input filename and extension from the command line arguments.
  fs::path input_filename = argv[optind];
  string input_extension = input_filename.extension().string();
  // Check if the input file exists
  if (!fs::exists(input_filename)) {
    cerr << "Input file does not exist: " << input_filename << endl;
    return 1;
  }
  // Automatically set the algorithm based on the input file extension.
  // If the input is a .bin file check the first bit which indicates the
  // algorithm used.
  if (input_extension == ".txt") {
    compress = true;
  } else if (input_extension == ".bin") {
    decompress = true;
    Reader reader(input_filename);
    int first_bit = reader.read_bit();
    if (first_bit == 0) {
      algorithm = "lz78";
    } else {
      algorithm = "huffman";
    }
  } else {
    // If the input file is not a .txt or .bin file, print an error message and
    // exit.
    cerr << "Invalid input file extension: " << input_extension << endl;
    return 1;
  }

  // If the user has not specified an algorithm, set it to lz78 by default.
  if (algorithm.empty()) {
    cout << "No algorithm specified, using lz78" << endl;
    algorithm = "lz78";
  }

  // If the user has not specified an output filename, set a default name.
  if (output_filename.empty()) {
    if (compress) {
      output_filename = input_filename.stem().string() + "_compressed.bin";
    } else if (decompress) {
      output_filename = input_filename.stem().string() + "_decompressed.txt";
    }
  }

  // Call the appropriate compression/decompression function.
  if (algorithm == "lz78") {
    if (compress) {
      auto start = chrono::high_resolution_clock::now();
      lz_compress(input_filename, output_filename);
      auto end = chrono::high_resolution_clock::now();
      chrono::duration<double> elapsed = end - start;
      cout << "Compression time: " << elapsed.count() << " seconds" << endl;
    } else if (decompress) {
      auto start = chrono::high_resolution_clock::now();
      lz_decompress(input_filename, output_filename);
      auto end = chrono::high_resolution_clock::now();
      chrono::duration<double> elapsed = end - start;
      cout << "Decompression time: " << elapsed.count() << " seconds" << endl;
    }
  } else if (algorithm == "huffman") {
    if (compress) {
      auto start = chrono::high_resolution_clock::now();
      huff_compress(input_filename, output_filename);
      auto end = chrono::high_resolution_clock::now();
      chrono::duration<double> elapsed = end - start;
      cout << "Compression time: " << elapsed.count() << " seconds" << endl;
    } else if (decompress) {
      auto start = chrono::high_resolution_clock::now();
      huff_decompress(input_filename, output_filename);
      auto end = chrono::high_resolution_clock::now();
      chrono::duration<double> elapsed = end - start;
      cout << "Decompression time: " << elapsed.count() << " seconds" << endl;
    }
  }

  // Finish execution.
  return 0;
}