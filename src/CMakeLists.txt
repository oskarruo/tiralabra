add_subdirectory(huffman)

add_subdirectory(lz78)

add_subdirectory(utils)

add_executable(main main.cpp huffman/huffman.cpp lz78/lz78.cpp utils/bitio.cpp)