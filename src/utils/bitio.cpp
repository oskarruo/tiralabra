#include "bitio.h"

#include <bitset>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace fs = std::filesystem;

/**
 * @brief A writer class that is used to write bits to a .bin file.
 *
 * @param filename The name of the .bin file for output.
 */
Writer::Writer(const string filename) {
  // Open the file, initialize the byte to 0, the bit count to 0, and reserve
  // space in the buffer.
  out.open(filename, ios::binary);
  byte = 0;
  bit_count = 0;
  buffer.reserve(4096);
}

// The destructor class flushes the buffer and closes the file if it is open.
Writer::~Writer() {
  flush();
  if (out.is_open()) {
    out.close();
  }
}

/**
 * @brief Writes a single bit to the output.
 *
 * @param bit The bit to be written.
 */
void Writer::write_bit(int bit) {
  // Add the bit to the byte and increment the bit count.
  byte = (byte << 1) | (bit & 1);
  bit_count++;

  // If the byte is full (8 bits), write it to the buffer and reset the bit
  // count.
  if (bit_count == 8) {
    buffer.push_back(static_cast<char>(byte));
    bit_count = 0;
    byte = 0;

    // If the buffer is full (4KB), flush it to the output file.
    if (buffer.size() >= 4096) {
      flush();
    }
  }
}

/**
 * @brief Writes an integer to the output.
 *
 * @param value The integer to be written.
 * @param bit_length The number of bits to be written.
 */
void Writer::write_int(int value, int bit_length) {
  for (int i = bit_length - 1; i >= 0; --i) {
    write_bit((value >> i) & 1);
  }
}

/**
 * @brief Writes a character to the output.
 *
 * @param value The character to be written.
 */
void Writer::write_char(char value) {
  for (int i = 7; i >= 0; --i) {
    write_bit((value >> i) & 1);
  }
}

/**
 * @brief Writes a string of a binary number to the output.
 *
 * @param binary The binary string to be written.
 */
void Writer::write_binary_string(const string& binary) {
  // Iterate through the binary string and write each bit to the output.
  for (char bit : binary) {
    write_bit(bit - '0');
  }
}

/**
 * @brief Flushes the remaining bits to the output.
 */
void Writer::flush() {
  // Add padding bits to the byte if there are any remaining bits.
  if (bit_count > 0) {
    while (bit_count < 8) {
      byte <<= 1;
      bit_count++;
    }
    buffer.push_back(static_cast<char>(byte));
    bit_count = 0;
  }

  // Write the buffer to the output file and clear it.
  if (!buffer.empty()) {
    out.write(buffer.data(), buffer.size());
    buffer.clear();
  }
}

/**
 * @brief A reader class that is used to read bits from a .bin file.
 *
 * @param filename The path of the .bin file for input.
 */
Reader::Reader(fs::path filename) {
  /**
   * Open the file, initialize the byte to 0, the bit count to 0,
   * set end to false, and initialize the buffer.
   * */
  in.open(filename, ios::binary);
  byte = 0;
  bit_count = 0;
  end = false;
  next_end = false;
  buffer_index = 0;
  next_buffer.resize(4096);
  in.read(next_buffer.data(), next_buffer.size());
  next_buffer.resize(in.gcount());
  refill_buffer();
}

// The destructor class closes the file if it is open.
Reader::~Reader() {
  if (in.is_open()) {
    in.close();
  }
}

/**
 * @brief Reads a single bit from the input.
 *
 * @return int The bit read from the input.
 */
int Reader::read_bit() {
  // If the bit count is 0, refill the buffer if necessary.
  if (bit_count == 0) {
    if (buffer_index >= buffer.size()) {
      refill_buffer();
      if (buffer.empty()) {
        return 0;
      }
    }
    // Read the next byte from the buffer.
    byte = static_cast<uint8_t>(buffer[buffer_index++]);
    bit_count = 8;
  }

  // Read the bit from the byte and decrement the bit count.
  bit_count--;
  return (byte >> bit_count) & 1;
}

/**
 * @brief Reads an integer from the input.
 *
 * @param bit_length The number of bits to be read.
 * @return int The integer read from the input.
 */
int Reader::read_int(int bit_length) {
  // Read the specified number of bits and return the integer value.
  int value = 0;
  for (int i = 0; i < bit_length; i++) {
    value = (value << 1) | read_bit();
  }
  return value;
}

/**
 * @brief Reads a character from the input.
 *
 * @return char The character read from the input.
 */
char Reader::read_char() {
  // Read 8 bits and return the character value.
  unsigned char c = 0;
  for (int i = 0; i < 8; i++) {
    c = (c << 1) | read_bit();
  }
  return static_cast<char>(c);
}

/**
 * @brief Refills the buffer by reading a 4 KB chunk from the input file.
 */
void Reader::refill_buffer() {
  // Set the buffer to the next buffer and reset the buffer index.
  buffer = next_buffer;
  buffer_index = 0;

  // If the buffer is not empty try to read the next buffer.
  if (!buffer.empty()) {
    next_buffer.clear();
    next_buffer.resize(4096);
    in.read(next_buffer.data(), next_buffer.size());
    next_buffer.resize(in.gcount());

    // If the next buffer is empty, set end to true.
    if (next_buffer.empty()) {
      next_end = true;
    }
  } else {
    // If the buffer is empty, set end to true.
    end = true;
  }
}

/**
 * @brief Returns the number of bits left in the current buffer.
 *
 * @return int The number of bits left in the current buffer.
 */
int Reader::bits_left() {
  return (buffer.size() - buffer_index) * 8 + bit_count;
}

/**
 * @brief Returns whether the end of the input has been reached.
 *
 * @return bool Whether the end of the input has been reached.
 */
// This method is used in specifically in the LZ decompression (as opposed to
// is_next_end).
bool Reader::is_end() { return end; }

/**
 * @brief Returns whether the next buffer is the end of the input.
 *
 * @return bool Whether the next buffer is the end of the input.
 */
// This method is used in specifically in the Huffman decompression (as opposed
// to is_end).
bool Reader::is_next_end() { return next_end; }