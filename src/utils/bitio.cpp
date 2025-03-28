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
  // Open the file, initialize a string for the current byte, and set the bit
  // count to 0.
  out.open(filename, ios::binary);
  byte = "";
  bit_count = 0;
}

// The destructor class closes the file if it is open.
Writer::~Writer() {
  if (out.is_open()) {
    out.close();
  }
}

/**
 * @brief Writes an integer to the output.
 *
 * @param value The integer to be written.
 * @param bit_length The number of bits to be written.
 */
void Writer::write_int(int value, int bit_length) {
  // Convert the integer to a binary string and get the amount of bits needed.
  string binary = bitset<32>(value).to_string();
  binary = binary.substr(32 - bit_length);

  // Iterate through the binary string and add bits to the byte.
  for (char bit : binary) {
    byte += bit;
    bit_count++;

    // Add byte to output when full.
    if (bit_count == 8) {
      char write_byte = static_cast<char>(bitset<8>(byte).to_ulong());
      out.write(&write_byte, 1);
      byte = "";
      bit_count = 0;
    }
  }
}

/**
 * @brief Writes a character to the output.
 *
 * @param value The character to be written.
 */
void Writer::write_char(char value) {
  // Convert the character to a binary string.
  string binary = bitset<8>(static_cast<unsigned char>(value)).to_string();

  // Iterate through the binary string and add bits to the byte.
  for (char bit : binary) {
    byte += bit;
    bit_count++;

    // Add byte to output when full.
    if (bit_count == 8) {
      char write_byte = static_cast<char>(bitset<8>(byte).to_ulong());
      out.write(&write_byte, 1);
      byte = "";
      bit_count = 0;
    }
  }
}

/**
 * @brief Writes a string of a binary number to the output.
 *
 * @param binary The binary string to be written.
 */
void Writer::write_binary_string(string binary) {
  // Iterate through the string and write each bit using write_int.
  for (char bit : binary) {
    write_int(bit, 1);
  }
}

/**
 * @brief Writes a single bit to the output.
 *
 * @param bit The bit to be written.
 */
void Writer::write_bit_char(char bit) {
  // Write the bit using write_int.
  write_int(bit, 1);
}

/**
 * @brief Flushes the remaining bits to the output.
 */
void Writer::flush() {
  // If there are remaining bits, add 0s to the byte until it is full, then
  // write to output.
  if (bit_count > 0) {
    while (bit_count < 8) {
      byte += '0';
      bit_count++;
    }
    char write_byte = static_cast<char>(bitset<8>(byte).to_ulong());
    out.write(&write_byte, 1);
  }
}

/**
 * @brief A reader class that is used to read bits from a .bin file.
 *
 * @param filename The path of the .bin file for input.
 */
Reader::Reader(fs::path filename) {
  // Open the file, initialize a string for the current byte, set the bit count
  // to 0, and set the eof bool to false.
  in.open(filename, ios::binary);
  byte = "";
  bit_count = 0;
  end = false;
}

// The destructor class closes the file if it is open.
Reader::~Reader() {
  if (in.is_open()) {
    in.close();
  }
}

/**
 * @brief Reads an integer from the input.
 *
 * @param bit_length The number of bits to be read.
 * @return int The integer read from the input.
 */
int Reader::read_int(int bit_length) {
  // Read bytes from the input until the current byte in memory has enough bits
  // to read the integer.
  while (byte.length() < bit_length) {
    char byte_read;
    if (!in.read(&byte_read, sizeof(byte_read))) {
      end = true;
      return 0;
    }

    // Convert the byte to a binary string and add it to the current byte in
    // memory.
    string byte_str =
        bitset<8>(static_cast<unsigned char>(byte_read)).to_string();
    byte += byte_str;
  }

  // Get the value of the integer from the current byte in memory and remove the
  // bits read from the byte.
  string value_str = byte.substr(0, bit_length);
  byte = byte.substr(bit_length);

  // Return the integer value.
  return static_cast<int>(bitset<32>(value_str).to_ulong());
}

/**
 * @brief Reads a single bit from the input.
 *
 * @return int The bit read from the input.
 */
int Reader::read_bit() { return read_int(1); }

/**
 * @brief Reads a character from the input.
 *
 * @return char The character read from the input.
 */
char Reader::read_char() {
  // Read bytes from the input until the current byte in memory has enough bits
  // to read the character.
  while (byte.length() < 8) {
    char byte_read;
    if (!in.read(&byte_read, sizeof(byte_read))) {
      end = true;
      return '\0';
    }

    // Convert the byte to a binary string and add it to the current byte in
    // memory.
    string byte_str =
        bitset<8>(static_cast<unsigned char>(byte_read)).to_string();
    byte += byte_str;
  }

  // Get the value of the character from the current byte in memory and remove
  // the bits read from the byte.
  string value_str = byte.substr(0, 8);
  byte = byte.substr(8);

  // Return the character value.
  return static_cast<char>(bitset<8>(value_str).to_ulong());
}

/**
 * @brief Returns the number of bits left in the input.
 *
 * @return int The number of bits left in the input.
 */
int Reader::bits_left() {
  // Get the number of bits in the current byte in memory and the number of bits
  // left in the input.
  int buffer_bits = byte.length();
  in.seekg(0, ios::cur);
  size_t current_pos = in.tellg();
  in.seekg(0, ios::end);
  size_t end_pos = in.tellg();
  in.seekg(current_pos, ios::beg);
  size_t remaining_bytes = end_pos - current_pos;
  return buffer_bits + (remaining_bytes * 8);
}

/**
 * @brief Returns whether the end of the input has been reached.
 *
 * @return bool Whether the end of the input has been reached.
 */
bool Reader::is_end() { return end; }