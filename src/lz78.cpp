#include "lz78.h"
#include "bitio.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <cmath>
using namespace std;
namespace fs = std::filesystem;

/**
 * @brief Compresses input using LZ78 algorithm and writes to a file.
 * 
 * @param input The input string to be compressed.
 * @param output_filename The name of the output .txt file.
 */
void lz_compress(string input, string output_filename) {
    unordered_map<string, int> dictionary;
    string prefix = "";
    int index = 1;
    int index_bits = 1;
    Writer writer(output_filename);

    /* 
    Iterate through the input string.
    If the prefix + current character is already in the dictionary, update the prefix.
    Otherwise, write the code word of the prefix and the current character to the output file and save it to the dictionary.
    */
    for (char c : input) {
        string prefix_c = prefix + c;
        if (dictionary.find(prefix_c) != dictionary.end()) {
            prefix = prefix_c;
        } else {
            int cwprefix;
            if (prefix == "") cwprefix = 0;
            else cwprefix = dictionary[prefix];
            dictionary[prefix_c] = index;
            index++;
            prefix = "";
            writer.write_int(cwprefix, index_bits);
            writer.write_char(c);
            index_bits = ceil(log2(index));
        }
    }
    
    // If the prefix is not empty, write the code word of the prefix to the output file.
    if (!prefix.empty()) {
        int cwprefix = dictionary[prefix];
        writer.write_int(cwprefix, index_bits);
    }

    writer.flush();
}

/**
 * @brief Decompresses input using LZ78 algorithm and writes to a file.
 * 
 * @param input_filename The input .bin file to be decompressed.
 * @param output_filename The name of the output .txt file.
 */
void lz_decompress(fs::path input_filename, string output_filename) {
    unordered_map<int, tuple<int, string>> message;
    int message_index = 0;
    Reader reader(input_filename);
    int index_bits = 1;

    while (true) {
        int prefix_index;
        char character;

        prefix_index = reader.read_int(index_bits);

        character = reader.read_char();
        if (reader.is_end()) {
            message[message_index] = make_tuple(prefix_index, "");
            break;
        }

        message[message_index] = make_tuple(prefix_index, string(1, character));
        message_index++;
        index_bits = ceil(log2(message_index + 1));
    }

    ofstream output_file(output_filename);
    unordered_map<int, string> dictionary;
    int index = 1;

    for (int i = 0; i < message.size(); i++) {
        int cw = get<0>(message[i]);
        string character = get<1>(message[i]);
        string text;

        if (cw == 0) {
            text = "";
        } else {
            text = dictionary[cw];
        }

        output_file << text;
        output_file << character;
        dictionary[index] = text + character;
        index++;
    }

    output_file.close();
}