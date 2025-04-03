#ifndef BITIO_H
#define BITIO_H

#include <bitset>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace fs = std::filesystem;

class Writer {
  ofstream out;
  uint8_t byte;
  int bit_count;
  vector<char> buffer;

 public:
  Writer(const string filename);

  ~Writer();

  void write_bit(int bit);

  void write_int(int value, int bit_length);

  void write_char(char value);

  void write_binary_string(const string& binary);

  void flush();
};

class Reader {
  ifstream in;
  string byte;
  int bit_count;
  bool end;

 public:
  Reader(fs::path filename);

  ~Reader();

  int read_int(int bit_length);

  int read_bit();

  char read_char();

  int bits_left();

  bool is_end();
};

#endif