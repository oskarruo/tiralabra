#ifndef BITIO_H
#define BITIO_H

#include <bitset>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace fs = std::filesystem;

/**
 * @brief A writer class that is used to write bits to a .bin file.
 */
class Writer {
  // Output file stream.
  ofstream out;
  // Current byte being written.
  uint8_t byte;
  // Number of bits written to the current byte.
  int bit_count;
  // Buffer to store the bytes before writing to the file.
  string buffer;

 public:
  // Constructor that opens the file and initializes variables.
  Writer(const string filename);

  // Destructor that flushes the buffer and closes the file.
  ~Writer();

  // Writes a single bit to the output.
  void write_bit(int bit);

  // Writes an integer to the output with a specified bit length.
  void write_int(int value, int bit_length);

  // Writes a character to the output.
  void write_char(char value);

  // Writes a string of a binary number to the output.
  void write_binary_string(const string& binary);

  // Flushes the remaining bits to the output.
  void flush();
};

/**
 * @brief A reader class that is used to read bits from a .bin file.
 */
class Reader {
  // Input file stream.
  ifstream in;
  // Current byte being read.
  uint8_t byte;
  // Number of bits left in the current byte.
  int bit_count;
  // Buffer to store the bytes read from the file.
  string buffer;
  // Buffer to store the next bytes read from the file.
  string next_buffer;
  // Index of the current byte in the buffer.
  size_t buffer_index;
  // Bool to indicate if the end of the file has been reached.
  bool end;
  // Bool to indicate if the next buffer would be the end of the file.
  bool next_end;

 public:
  // Constructor that opens the file and initializes variables.
  Reader(fs::path filename);

  // Destructor that closes the file.
  ~Reader();

  // Reads a single bit from the input.
  int read_bit();

  // Reads an integer from the input with a specified bit length.
  int read_int(int bit_length);

  // Reads a character from the input.
  char read_char();

  // Returns the number of bits left in the current buffer.
  int bits_left();

  // Returns if the end of the file has been reached.
  bool is_end();

  // Returns if the next buffer would be the end of the file.
  bool is_next_end();

  // Refills the buffer.
  void refill_buffer();
};

#endif