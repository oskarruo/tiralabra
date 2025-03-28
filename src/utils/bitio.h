#ifndef BITIO_H
#define BITIO_H

#include <fstream>
#include <filesystem>
#include <string>
#include <iostream>
#include <bitset>

using namespace std;

namespace fs = std::filesystem;

class Writer {
    ofstream out;
    string byte;
    int bit_count;

public:
    Writer(const string filename);

    ~Writer();

    void write_int(int value, int bit_length);

    void write_char(char value);

    void write_binary_string(string binary);

    void write_bit_char(char bit);

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