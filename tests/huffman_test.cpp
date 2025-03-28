#include "../src/huffman/huffman.h"
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class HuffmanFileTest : public ::testing::Test {
protected:
    string input_filename = "test_input.txt";
    string compressed_filename = "test_compressed.bin";
    string decompressed_filename = "test_decompressed.txt";

    void SetUp() override {
        ofstream input_file(input_filename);
        input_file << "test input";
        input_file.close();
    }

    void TearDown() override {
        fs::remove(compressed_filename);
        fs::remove(decompressed_filename);
    }
};

TEST_F(HuffmanFileTest, TestHuffmanCompress) {
    huff_compress(input_filename, compressed_filename);

    ASSERT_TRUE(fs::exists(compressed_filename));

    ifstream compressed_file(compressed_filename, ios::binary);
    ASSERT_TRUE(compressed_file.is_open());

    compressed_file.close();
}

TEST_F(HuffmanFileTest, TestHuffmanDecompress) {
    huff_compress(input_filename, compressed_filename);

    huff_decompress(compressed_filename, decompressed_filename);

    ASSERT_TRUE(fs::exists(decompressed_filename));

    ifstream decompressed_file(decompressed_filename);
    ASSERT_TRUE(decompressed_file.is_open());

    string decompressed_content;
    getline(decompressed_file, decompressed_content);
    ASSERT_EQ(decompressed_content, "test input");

    decompressed_file.close();
}

TEST_F(HuffmanFileTest, TestHuffmanCompressDecompressBiggerFile) {
    huff_compress("test.txt", compressed_filename);

    huff_decompress(compressed_filename, decompressed_filename);

    ASSERT_TRUE(fs::exists(decompressed_filename));

    ifstream decompressed_file(decompressed_filename);
    ifstream original_file("test.txt");
    ASSERT_TRUE(decompressed_file.is_open());
    ASSERT_TRUE(original_file.is_open());

    string original_content, decompressed_content;

    while (getline(original_file, original_content)) {
        ASSERT_TRUE(getline(decompressed_file, decompressed_content));
        ASSERT_EQ(decompressed_content, original_content);
    }

    ASSERT_FALSE(getline(decompressed_file, decompressed_content));

    decompressed_file.close();
    original_file.close();
}
