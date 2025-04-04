include(FetchContent)

FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.16.0
)

FetchContent_MakeAvailable(gtest)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(lz78_test
    ../src/lz78/lz78.cpp
    ../src/utils/bitio.cpp
    lz78_test.cpp
)

add_executable(huffman_test
    ../src/huffman/huffman.cpp
    ../src/utils/bitio.cpp
    huffman_test.cpp
)

add_executable(compress_missingarguments_test
    ../src/lz78/lz78.cpp
    ../src/huffman/huffman.cpp
    ../src/utils/bitio.cpp
    ../src/compress.cpp
    compress_test.cpp
)

add_executable(compress_invalidalgorithm_test
    ../src/lz78/lz78.cpp
    ../src/huffman/huffman.cpp
    ../src/utils/bitio.cpp
    ../src/compress.cpp
    compress_test.cpp
)

add_executable(compress_noinputfile_test
    ../src/lz78/lz78.cpp
    ../src/huffman/huffman.cpp
    ../src/utils/bitio.cpp
    ../src/compress.cpp
    compress_test.cpp
)

add_executable(compress_inputfiledoesnotexist_test
    ../src/lz78/lz78.cpp
    ../src/huffman/huffman.cpp
    ../src/utils/bitio.cpp
    ../src/compress.cpp
    compress_test.cpp
)

add_executable(compress_compressdecompresslz_test
    ../src/lz78/lz78.cpp
    ../src/huffman/huffman.cpp
    ../src/utils/bitio.cpp
    ../src/compress.cpp
    compress_test.cpp
)

add_executable(compress_compressdecompresshuffman_test
    ../src/lz78/lz78.cpp
    ../src/huffman/huffman.cpp
    ../src/utils/bitio.cpp
    ../src/compress.cpp
    compress_test.cpp
)

target_link_libraries(lz78_test gtest gtest_main)

target_link_libraries(huffman_test gtest gtest_main)

target_link_libraries(compress_missingarguments_test gtest gtest_main)

target_link_libraries(compress_invalidalgorithm_test gtest gtest_main)

target_link_libraries(compress_noinputfile_test gtest gtest_main)

target_link_libraries(compress_inputfiledoesnotexist_test gtest gtest_main)

target_link_libraries(compress_compressdecompresslz_test gtest gtest_main)

target_link_libraries(compress_compressdecompresshuffman_test gtest gtest_main)

add_custom_command(
    TARGET lz78_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/tests/test.txt
        ${CMAKE_CURRENT_BINARY_DIR}/test.txt
)

add_custom_command(
    TARGET lz78_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/tests/big_test.txt
        ${CMAKE_CURRENT_BINARY_DIR}/big_test.txt
)

add_test(
    NAME lz78_tests
    COMMAND lz78_test
)

add_test(
    NAME huffman_tests
    COMMAND huffman_test
)

add_test(NAME compress_missingarguments_test COMMAND compress_missingarguments_test --gtest_filter=CompressionTest.MissingArguments)

add_test(NAME compress_invalidalgorithm_test COMMAND compress_invalidalgorithm_test --gtest_filter=CompressionTest.InvalidAlgorithm)

add_test(NAME compress_noinputfile_test COMMAND compress_noinputfile_test --gtest_filter=CompressionTest.NoInputFile)

add_test(NAME compress_inputfiledoesnotexist_test COMMAND compress_inputfiledoesnotexist_test --gtest_filter=CompressionTest.InputFileDoesNotExist)

add_test(NAME compress_compressdecompresslz_test COMMAND compress_compressdecompresslz_test --gtest_filter=CompressionTest.CompressDecompressLZ)

add_test(NAME compress_compressdecompresshuffman_test COMMAND compress_compressdecompresshuffman_test --gtest_filter=CompressionTest.CompressDecompressHuffman)