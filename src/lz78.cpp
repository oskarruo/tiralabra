#include "lz78.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

void write_binary(ofstream &out, int value) {
    out.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

void write_char(ofstream &out, char value) {
    out.write(&value, sizeof(value));
}

int lz_compress(string input, string output_filename) {
    ofstream output_file(output_filename, ios::binary);
    unordered_map<string, int> dictionary;
    unordered_map<int, tuple<int, string>> message_dict;
    string prefix = "";
    int index = 1;
    int length = input.length();
    int message_index = 0;

    for (char c : input) {
        string prefix_c = prefix + c;
        if (dictionary.find(prefix_c) != dictionary.end()) {
            prefix = prefix_c;
        } else {
            int cwprefix;
            if (prefix == "") {
                cwprefix = 0;
            } else {
                cwprefix = dictionary[prefix];
            }
            message_dict[message_index] = make_tuple(cwprefix, c);
            dictionary[prefix_c] = index;
            index++, message_index++;
            prefix = "";
            write_binary(output_file, cwprefix);
            write_char(output_file, c);
        }
    }
    
    if (!prefix.empty()) {
        int cwprefix = dictionary[prefix];
        message_dict[message_index] = make_tuple(cwprefix, "");
        write_binary(output_file, cwprefix);
    }
    output_file.close();

    return 0;
}

int lz_decompress(fs::path input_filename, string output_filename) {
    unordered_map<int, tuple<int, string>> message;
    int message_index = 0;
    ifstream input_file(input_filename, ios::binary);

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

    input_file.close();

    string output;
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

        output += text + character;
        dictionary[index] = text + character;
        index++;
    }

    ofstream output_file(output_filename);
    output_file << output;
    output_file.close();

    return 0;
}