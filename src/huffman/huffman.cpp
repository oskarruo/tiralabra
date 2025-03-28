#include "huffman.h"
#include "../utils/bitio.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <queue>
#include <string>
#include <optional>
using namespace std;
namespace fs = std::filesystem;

struct Node {
    optional<int> freq;
    optional<char> c;
    Node* l;
    Node* r;

    Node(char s, int f) : c(s), freq(f), l(nullptr), r(nullptr) {}
    Node(int f) : c(nullopt), freq(f), l(nullptr), r(nullptr) {}
    Node() : c(nullopt), freq(nullopt), l(nullptr), r(nullptr) {}
};

struct CompareCharacter {
    bool operator()(Node* n1, Node* n2) {
        int f1 = n1->freq.value();
        int f2 = n2->freq.value();
        return f1 > f2;
    }
};

void traverse_tree(Node* node, string code, unordered_map<char, string>& codes, Writer& writer, int& bit_count) {
    if (node->c != nullopt) {
        codes[node->c.value()] = code;
        writer.write_bit_char(1);
        writer.write_char(node->c.value());
        bit_count += 9;
        delete node;
    }
    else {
        writer.write_bit_char(0);
        bit_count += 1;
        if (node->l != nullptr) {
            traverse_tree(node->l, code + "0", codes, writer, bit_count);
        }
        if (node->r != nullptr) {
            traverse_tree(node->r, code + "1", codes, writer, bit_count);
        }
        delete node;
    }
}

unordered_map<char, string> get_codes(Node* root, Writer& writer, int& bit_count) {
    unordered_map<char, string> codes;
    traverse_tree(root, "", codes, writer, bit_count);
    return codes;
};

void huff_compress(fs::path input_filename, string output_filename) {
    string input = "";
    string line;
    ifstream input_file(input_filename);
    if (input_file.is_open()) {
        while(getline(input_file, line)) {
            input += line + "\n";
          }
        input_file.close();
    }

    unordered_map<char, int> characters;
    int bit_count = 4;
    for (char c : input) {
        if (c == '\0') continue;
        if (characters.find(c) == characters.end()) {
            characters[c] = 1;
        } else characters[c] += 1;
    }

    priority_queue<Node*, vector<Node*>, CompareCharacter> q;
    for (auto c : characters) {
        Node* leaf = new Node(c.first, c.second);
        q.emplace(leaf);
    }

    while (q.size() > 1) {
        Node* min1 = q.top();
        q.pop();
        Node* min2 = q.top();
        q.pop();
        int freq1 = min1->freq.value();
        int freq2 = min2->freq.value();
        Node* empty = new Node(freq1 + freq2);
        empty->l = min1;
        empty->r = min2;
        q.emplace(empty);
    }

    Node* root = q.top();
    Writer writer(output_filename);
    unordered_map<char, string> codes = get_codes(root, writer, bit_count);

    for (char c : input) {
        bit_count += codes[c].size();
    }
    writer.write_int((8 - (bit_count % 8)) % 8, 4);

    for (char c : input) {
        string code = codes[c];
        writer.write_binary_string(code);
    }
    writer.flush();
}

Node* deconstruct_tree(Reader& reader) {
    if (reader.read_bit() == 1) {
        char c = reader.read_char();
        return new Node(c, 0);
    } else {
        Node* left = deconstruct_tree(reader);
        Node* right = deconstruct_tree(reader);
        Node* node = new Node();
        node->l = left;
        node->r = right;
        return node;
    }
}

char decode_char(Node* root, Reader& reader) {
    Node* current = root;
    while (current->c == nullopt) {
        if (reader.read_bit() == 0) {
            current = current->l;
        } else {
            current = current->r;
        }
    }
    return current->c.value();
}

void huff_decompress(fs::path input_filename, string output_filename) {
    Reader reader(input_filename);
    unordered_map<string, char> codes;
    Node* root = deconstruct_tree(reader);

    int padding = reader.read_int(4);
    ofstream output_file(output_filename);
    
    while (reader.bits_left() > padding) {
        char c = decode_char(root, reader);
        output_file << c;
    }
}