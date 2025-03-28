#include "lz78.h"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "../utils/bitio.h"
using namespace std;
namespace fs = std::filesystem;

/**
 * @brief Compresses input using LZ78 algorithm and writes to a file.
 *
 * @param input The input string to be compressed.
 * @param output_filename The name of the output .txt file.
 */
void lz_compress(fs::path input_filename, string output_filename) {
  // Read the input file into a string.
  string input = "";
  string line;
  ifstream input_file(input_filename);
  if (input_file.is_open()) {
    while (getline(input_file, line)) {
      input += line + "\n";
    }
    input_file.close();
  }

  // Initialize the dictionary, prefix, index, index_bits (how many bits are
  // needed for the index), and the writer.
  unordered_map<string, int> dictionary;
  string prefix = "";
  int index = 1;
  int index_bits = 1;
  Writer writer(output_filename);
  writer.write_bit_char(0);

  /*
  Iterate through the input string.
  If the prefix + current character is already in the dictionary, update the
  prefix. Otherwise, write the code word of the prefix and the current character
  to the output file and save it to the dictionary.
  Update the index and index_bits.
  */
  for (char c : input) {
    string prefix_c = prefix + c;
    if (dictionary.find(prefix_c) != dictionary.end()) {
      prefix = prefix_c;
    } else {
      int cwprefix;
      if (prefix == "")
        cwprefix = 0;
      else
        cwprefix = dictionary[prefix];
      dictionary[prefix_c] = index;
      index++;
      prefix = "";
      writer.write_int(cwprefix, index_bits);
      writer.write_char(c);
      index_bits = ceil(log2(index));
    }
  }

  // If the prefix is not empty, write the code word of the prefix to the output
  // file.
  if (!prefix.empty()) {
    int cwprefix = dictionary[prefix];
    writer.write_int(cwprefix, index_bits);
  }

  // Flush the remaining bits to the output file.
  writer.flush();
}

/**
 * @brief Decompresses input using LZ78 algorithm and writes to a file.
 *
 * @param input_filename The input .bin file to be decompressed.
 * @param output_filename The name of the output .txt file.
 */
void lz_decompress(fs::path input_filename, string output_filename) {
  /**
  Initialize the dictionary for the encoded message, the message index, the
  reader, and the index_bits (how many bits are needed for the index).
  */
  unordered_map<int, tuple<int, string>> message;
  int message_index = 0;
  Reader reader(input_filename);
  reader.read_bit();
  int index_bits = 1;

  // Loop through the input file
  while (true) {
    // Read the prefix index and the character from the input file.
    int prefix_index = reader.read_int(index_bits);
    char character = reader.read_char();

    // If the reader is at the end of the file, break the loop.
    if (reader.is_end()) {
      message[message_index] = make_tuple(prefix_index, "");
      break;
    }

    // Save the prefix index and the character to the message dictionary and
    // update the index_bits.
    message[message_index] = make_tuple(prefix_index, string(1, character));
    message_index++;
    index_bits = ceil(log2(message_index + 1));
  }

  // Initialize the dictionary and the output file.
  ofstream output_file(output_filename);
  unordered_map<int, string> dictionary;
  int index = 1;

  // Loop through the message dictionary and write the decoded message to the
  // output file.
  for (int i = 0; i < message.size(); i++) {
    // Get the code word, character, and text from the message.
    int cw = get<0>(message[i]);
    string character = get<1>(message[i]);
    string text;

    // If the code word is 0, the text is empty. Otherwise, get the text from
    // the dictionary.
    if (cw == 0) {
      text = "";
    } else {
      text = dictionary[cw];
    }

    // Write the text + character to the output file and save it to the
    // dictionary.
    output_file << text;
    output_file << character;
    dictionary[index] = text + character;
    index++;
  }

  // Close the output file.
  output_file.close();
}