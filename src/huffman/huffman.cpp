#include "huffman.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>

#include "../utils/bitio.h"
using namespace std;
namespace fs = std::filesystem;

// A node structure for the Huffman tree.
struct Node {
  // The frequency of the character or the sum of the frequencies of the
  // children.
  optional<int> freq;
  // The character if the node is a leaf, nullopt otherwise.
  optional<char> c;
  // A pointer to the left child.
  Node* l;
  // A pointer to the right child.
  Node* r;

  /**
  Constructors for the node. First is used in compression, third in
  decompression, and second in both.
  */
  Node(char s, int f) : c(s), freq(f), l(nullptr), r(nullptr) {}
  Node(int f) : c(nullopt), freq(f), l(nullptr), r(nullptr) {}
  Node() : c(nullopt), freq(nullopt), l(nullptr), r(nullptr) {}
};

// A comparator for the priority queue. Compares the frequencies of the nodes.
struct CompareCharacter {
  bool operator()(Node* n1, Node* n2) {
    int f1 = n1->freq.value();
    int f2 = n2->freq.value();
    return f1 > f2;
  }
};

/**
 * @brief A recursive function to traverse the Huffman tree and get the codes
 * for each character.
 *
 * @param node The current node.
 * @param code The code for the current node.
 * @param codes The dictionary for codes.
 * @param writer The writer object.
 * @param bit_count The amount of meaningful bits in the output.
 */
void traverse_tree(Node* node, string code, unordered_map<char, string>& codes,
                   Writer& writer, int& bit_count) {
  // If the node is a leaf, write the character and its code.
  if (node->c != nullopt) {
    codes[node->c.value()] = code;
    writer.write_bit_char(1);
    writer.write_char(node->c.value());
    bit_count += 9;
    delete node;
  }
  // If the node is not a leaf, write 0 and traverse the children.
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

/**
 * @brief Gets the code dictionary by calling traverse_tree and returns it.
 *
 * @param root The root node of the Huffman tree.
 * @param writer The writer object.
 * @param bit_count The amount of meaningful bits in the output.
 * @return unordered_map<char, string> The dictionary of codes.
 */
unordered_map<char, string> get_codes(Node* root, Writer& writer,
                                      int& bit_count) {
  unordered_map<char, string> codes;
  traverse_tree(root, "", codes, writer, bit_count);
  return codes;
};

/**
 * @brief The main function for compressing a file using Huffman coding.
 *
 * @param input_filename The input file path to be compressed.
 * @param output_filename The output name for the compressed file.
 */
void huff_compress(fs::path input_filename, string output_filename) {
  // Read the input file.
  string input = "";
  string line;
  ifstream input_file(input_filename);
  if (input_file.is_open()) {
    while (getline(input_file, line)) {
      input += line + "\n";
    }
    input_file.close();
  }

  // Count the frequency of each character.
  unordered_map<char, int> characters;
  int bit_count = 5;
  for (char c : input) {
    if (c == '\0') continue;
    if (characters.find(c) == characters.end()) {
      characters[c] = 1;
    } else
      characters[c] += 1;
  }

  // Create a priority queue for the characters and create initial isolated
  // nodes.
  priority_queue<Node*, vector<Node*>, CompareCharacter> q;
  for (auto c : characters) {
    Node* leaf = new Node(c.first, c.second);
    q.emplace(leaf);
  }

  // Create the Huffman tree.
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

  // Get the codes for each character.
  Node* root = q.top();
  Writer writer(output_filename);
  writer.write_bit_char(1);
  unordered_map<char, string> codes = get_codes(root, writer, bit_count);

  /*
  Calculate the padding that will be used in the end and write it to the output
  as a 4 bit integer.
  */
  for (char c : input) {
    bit_count += codes[c].size();
  }
  writer.write_int((8 - (bit_count % 8)) % 8, 4);

  // Write the code of each character to the output.
  for (char c : input) {
    string code = codes[c];
    writer.write_binary_string(code);
  }
  // Flush the writer.
  writer.flush();
}

/**
 * @brief Deconstructs the Huffman tree from the reader object.
 *
 * @param reader The reader object.
 * @return Node* The root node of the Huffman tree.
 */
Node* deconstruct_tree(Reader& reader) {
  /*
  If bit is 1, read character and return a new isolated node.
  If bit is 0, read the left and right children and return a new node with no
  value around them.
  */
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

/**
 * @brief Decodes a character from the reader object using the Huffman tree.
 *
 * @param root The root node of the Huffman tree.
 * @param reader The reader object.
 * @return char The decoded character.
 */
char decode_char(Node* root, Reader& reader) {
  // Traverse the tree until a leaf is reached.
  Node* current = root;
  while (current->c == nullopt) {
    if (reader.read_bit() == 0) {
      current = current->l;
    } else {
      current = current->r;
    }
  }
  // Return the character.
  return current->c.value();
}

/**
 * @brief The main function for decompressing a file using Huffman coding.
 *
 * @param input_filename The input file path to be decompressed.
 * @param output_filename The output name for the decompressed file.
 */
void huff_decompress(fs::path input_filename, string output_filename) {
  // Initialize the reader object, the code dictionary, and deconstruct the
  // Huffman tree.
  Reader reader(input_filename);
  reader.read_bit();
  unordered_map<string, char> codes;
  Node* root = deconstruct_tree(reader);

  // Read the 4 bits that tell the padding and create the output file.
  int padding = reader.read_int(4);
  ofstream output_file(output_filename);

  // Decode the characters until the padding is reached.
  while (reader.bits_left() > padding) {
    char c = decode_char(root, reader);
    output_file << c;
  }
}