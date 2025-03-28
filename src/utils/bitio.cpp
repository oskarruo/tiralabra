#include "bitio.h"
#include <fstream>
#include <filesystem>
#include <string>
#include <iostream>
#include <bitset>

using namespace std;

namespace fs = std::filesystem;

Writer::Writer(const string filename) {
    out.open(filename, ios::binary);
    byte = "";
    bit_count = 0;
}

Writer::~Writer() {
    if (out.is_open()) {
        out.close();
    }
}

void Writer::write_int(int value, int bit_length) {
    string binary = bitset<32>(value).to_string().substr(32 - bit_length);
    
    for (char bit : binary) {
        byte += bit;
        bit_count++;

        if (bit_count == 8) {
            char write_byte = static_cast<char>(bitset<8>(byte).to_ulong());
            out.write(&write_byte, 1);
            byte = "";
            bit_count = 0;
        }
    }
}

void Writer::write_char(char value) {
    string binary = bitset<8>(static_cast<unsigned char>(value)).to_string();
    
    for (char bit : binary) {
        byte += bit;
        bit_count++;

        if (bit_count == 8) {
            char write_byte = static_cast<char>(bitset<8>(byte).to_ulong());
            out.write(&write_byte, 1);
            byte = "";
            bit_count = 0;
        }
    }
}

void Writer::write_binary_string(string binary) {
    for (char bit : binary) {
        write_int(bit, 1);
    }
}

void Writer::write_bit_char(char bit) {
    write_int(bit, 1);
}

void Writer::flush() {
    if (bit_count > 0) {
        while (bit_count < 8) {
            byte += '0';
            bit_count++;
        }
        char write_byte = static_cast<char>(bitset<8>(byte).to_ulong());
        out.write(&write_byte, 1);
    }
}



Reader::Reader(fs::path filename) {
    in.open(filename, ios::binary);
    byte = "";
    bit_count = 0;
    end = false;
}

Reader::~Reader() {
    if (in.is_open()) {
        in.close();
    }
}

int Reader::read_int(int bit_length) {
    while (byte.length() < bit_length) {
        char byte_read;
        if (!in.read(&byte_read, sizeof(byte_read))) {
            end = true;
            return 0;
        }

        string byte_str = bitset<8>(static_cast<unsigned char>(byte_read)).to_string();
        byte += byte_str;
    }

    string value_str = byte.substr(0, bit_length);
    byte = byte.substr(bit_length);

    return static_cast<int>(bitset<8>(value_str).to_ulong());
}

int Reader::read_bit() {
    return read_int(1);
}

char Reader::read_char() {
    while (byte.length() < 8) {
        char byte_read;
        if (!in.read(&byte_read, sizeof(byte_read))) {
            end = true;
            return '\0';
        }

        string byte_str = bitset<8>(static_cast<unsigned char>(byte_read)).to_string();
        byte += byte_str;
    }

    string value_str = byte.substr(0, 8);
    byte = byte.substr(8);

    return static_cast<char>(bitset<8>(value_str).to_ulong());
}

int Reader::bits_left() {
    int buffer_bits = byte.length();
    in.seekg(0, ios::cur);
    size_t current_pos = in.tellg();
    in.seekg(0, ios::end);
    size_t end_pos = in.tellg();
    in.seekg(current_pos, ios::beg);
    size_t remaining_bytes = end_pos - current_pos;
    return buffer_bits + (remaining_bytes * 8);
}

bool Reader::is_end() {
    return end;
}