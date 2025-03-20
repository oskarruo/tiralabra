#include "lz78.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

/**
 * @brief Writes an integer to a .bin file.
 * 
 * @param out The output file stream.
 * @param value The integer to be written. 
 */
void write_binary(ofstream &out, int value) {
    out.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

/**
 * @brief Writes a character to a .bin file.
 * 
 * @param out The output file stream.
 * @param value The character to be written. 
 */
void write_char(ofstream &out, char value) {
    out.write(&value, sizeof(value));
}

/**
 * @brief Compresses input using LZ78 algorithm and writes to a file.
 * 
 * @param input The input string to be compressed.
 * @param output_filename The name of the output .txt file.
 */
void lz_compress(string input, string output_filename) {
    // Open the output file, create the dictionary and initialize variables.
    ofstream output_file(output_filename, ios::binary);
    unordered_map<string, int> dictionary;
    string prefix = "";
    int index = 1;

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
            write_binary(output_file, cwprefix);
            write_char(output_file, c);
        }
    }
    
    // If the prefix is not empty, write the code word of the prefix to the output file.
    if (!prefix.empty()) {
        int cwprefix = dictionary[prefix];
        write_binary(output_file, cwprefix);
    }
    output_file.close();
}

/**
 * @brief Decompresses input using LZ78 algorithm and writes to a file.
 * 
 * @param input_filename The input .bin file to be decompressed.
 * @param output_filename The name of the output .txt file.
 */
void lz_decompress(fs::path input_filename, string output_filename) {
    // Open the input file, create a dictionary for decompressing the binary to a message, and initialize variables.
    ifstream input_file(input_filename, ios::binary);
    unordered_map<int, tuple<int, string>> message;
    int message_index = 0;

    // Read the binary file and save the code words and characters to the message dictionary.
    while (true) {
        int prefix_index;
        char character;

        input_file.read(reinterpret_cast<char*>(&prefix_index), sizeof(prefix_index));
        if (input_file.eof()) break;

        input_file.read(&character, sizeof(character));
        if (input_file.eof()) break;

        message[message_index] = make_tuple(prefix_index, string(1, character));
        message_index++;
    }

    // Close the input file.
    input_file.close();

    // Initialize variables and a dictionary for decompressing the message.
    unordered_map<int, string> dictionary;
    string output;
    int index = 1;

    // Iterate through the message dictionary and decompress the message.
    for (int i = 0; i < message.size(); i++) {
        int cw = get<0>(message[i]);
        string character = get<1>(message[i]);
        string text;

        if (cw == 0) {
            text = "";
        } else {
            text = dictionary[cw];
        }

        output += text + character;
        dictionary[index] = text + character;
        index++;
    }

    // Write the decompressed message to the output file.
    ofstream output_file(output_filename);
    output_file << output;
    output_file.close();
}