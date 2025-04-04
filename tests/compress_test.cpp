#include "../src/compress.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "../src/huffman/huffman.h"
#include "../src/lz78/lz78.h"
#include "../src/utils/bitio.h"

namespace fs = std::filesystem;

/**
 * @brief A test class for testing the main function.
 */
class CompressionTest : public ::testing::Test {
 protected:
  string incompatible_filename = "test_incompatible.cpp";
  string input_filename = "test_input.txt";
  string compressed_filename = "test_compressed.bin";
  string decompressed_filename = "test_decompressed.txt";

  // Create test files before each test.
  void SetUp() override {
    ofstream input_file(input_filename);
    input_file << "test input";
    input_file.close();
    ofstream incompatible_file(incompatible_filename);
    incompatible_file << "test input";
    incompatible_file.close();
  }

  // Remove test files after each test.
  void TearDown() override {
    fs::remove(compressed_filename);
    fs::remove(decompressed_filename);
    fs::remove(input_filename);
    fs::remove(incompatible_filename);
  }
};

// Test for case with no arguments.
TEST_F(CompressionTest, MissingArguments) {
  testing::internal::CaptureStderr();
  const char* argv[] = {"program"};
  int argc = sizeof(argv) / sizeof(argv[0]);

  ASSERT_EQ(cc(argc, const_cast<char**>(argv)), 1);
  std::string output = testing::internal::GetCapturedStderr();
  ASSERT_TRUE(output.find("Usage: ") != std::string::npos);
}

// Test for case with invalid algorithm.
TEST_F(CompressionTest, InvalidAlgorithm) {
  testing::internal::CaptureStderr();
  const char* argv[] = {"program", "-a", "lol", input_filename.c_str()};
  int argc = sizeof(argv) / sizeof(argv[0]);

  ASSERT_EQ(cc(argc, const_cast<char**>(argv)), 1);
  std::string output = testing::internal::GetCapturedStderr();
  ASSERT_TRUE(output.find("Invalid algorithm: lol") != std::string::npos);
}

// Test for case with no input file specified.
TEST_F(CompressionTest, NoInputFile) {
  testing::internal::CaptureStderr();
  const char* argv[] = {"program", "-o", compressed_filename.c_str()};
  int argc = sizeof(argv) / sizeof(argv[0]);

  ASSERT_EQ(cc(argc, const_cast<char**>(argv)), 1);
  std::string output = testing::internal::GetCapturedStderr();
  ASSERT_TRUE(output.find("No input file specified") != std::string::npos);
}

// Test for case with non-existent input file.
TEST_F(CompressionTest, InputFileDoesNotExist) {
  testing::internal::CaptureStderr();
  const char* argv[] = {"program", "-o", compressed_filename.c_str(),
                        "fake.txt"};
  int argc = sizeof(argv) / sizeof(argv[0]);

  ASSERT_EQ(cc(argc, const_cast<char**>(argv)), 1);
  std::string output = testing::internal::GetCapturedStderr();
  ASSERT_TRUE(output.find("Input file does not exist: ") != std::string::npos);
}

// Test for case with invalid input file extension.
TEST_F(CompressionTest, IncompatibleFileExtension) {
  testing::internal::CaptureStderr();
  const char* argv[] = {"program", "-o", compressed_filename.c_str(),
                        incompatible_filename.c_str()};
  int argc = sizeof(argv) / sizeof(argv[0]);

  ASSERT_EQ(cc(argc, const_cast<char**>(argv)), 1);
  std::string output = testing::internal::GetCapturedStderr();
  ASSERT_TRUE(output.find("Invalid input file extension: .cpp") !=
              std::string::npos);
}

// Test for main function running the LZ algorithm.
TEST_F(CompressionTest, CompressDecompressLZ) {
  const char* argv[] = {"program", "-o", compressed_filename.c_str(),
                        input_filename.c_str()};
  int argc = sizeof(argv) / sizeof(argv[0]);
  ASSERT_EQ(cc(argc, const_cast<char**>(argv)), 0);
  ASSERT_TRUE(fs::exists(compressed_filename));

  const char* argv_decompress[] = {"program", "-o",
                                   decompressed_filename.c_str(),
                                   compressed_filename.c_str()};
  int argc_decompress = sizeof(argv_decompress) / sizeof(argv_decompress[0]);
  ASSERT_EQ(cc(argc_decompress, const_cast<char**>(argv_decompress)), 0);
  ASSERT_TRUE(fs::exists("test_compressed_decompressed.txt"));

  std::ifstream decompressed("test_compressed_decompressed.txt");
  std::string decompressed_content;
  std::getline(decompressed, decompressed_content);
  decompressed.close();
  ASSERT_EQ(decompressed_content, "test input");
}

// Test for main function running the Huffman algorithm.
TEST_F(CompressionTest, CompressDecompressHuffman) {
  const char* argv[] = {"program",
                        "-a",
                        "huffman",
                        "-o",
                        compressed_filename.c_str(),
                        input_filename.c_str()};
  int argc = sizeof(argv) / sizeof(argv[0]);
  ASSERT_EQ(cc(argc, const_cast<char**>(argv)), 0);
  ASSERT_TRUE(fs::exists(compressed_filename));

  const char* argv_decompress[] = {"program",
                                   "-a",
                                   "huffman",
                                   "-o",
                                   decompressed_filename.c_str(),
                                   compressed_filename.c_str()};
  int argc_decompress = sizeof(argv_decompress) / sizeof(argv_decompress[0]);
  ASSERT_EQ(cc(argc_decompress, const_cast<char**>(argv_decompress)), 0);
  ASSERT_TRUE(fs::exists("test_compressed_decompressed.txt"));

  std::ifstream decompressed("test_compressed_decompressed.txt");
  std::string decompressed_content;
  std::getline(decompressed, decompressed_content);
  decompressed.close();
  ASSERT_EQ(decompressed_content, "test input");
}