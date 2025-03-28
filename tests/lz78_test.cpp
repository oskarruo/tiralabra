#include "../src/lz78/lz78.h"
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class LZ78FileTest : public ::testing::Test {
protected:
    string compressed_filename = "test_compressed.bin";
    string decompressed_filename = "test_decompressed.txt";

    void TearDown() override {
        fs::remove(compressed_filename);
        fs::remove(decompressed_filename);
    }
};

TEST_F(LZ78FileTest, TestLZ78Compress) {
    lz_compress("test input", compressed_filename);

    ASSERT_TRUE(fs::exists(compressed_filename));

    ifstream compressed_file(compressed_filename, ios::binary);
    ASSERT_TRUE(compressed_file.is_open());

    compressed_file.close();
}

TEST_F(LZ78FileTest, TestLZ78Decompress) {
    lz_compress("test input", compressed_filename);

    lz_decompress(compressed_filename, decompressed_filename);

    ASSERT_TRUE(fs::exists(decompressed_filename));

    ifstream decompressed_file(decompressed_filename);
    ASSERT_TRUE(decompressed_file.is_open());

    string decompressed_content;
    getline(decompressed_file, decompressed_content);
    ASSERT_EQ(decompressed_content, "test input");

    decompressed_file.close();
}
