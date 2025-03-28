# Testing report

## Test coverage

The live test coverage report can be found from codecov:

[![codecov](https://codecov.io/github/oskarruo/tiralabra/graph/badge.svg?token=XWZXH8K9VF)](https://codecov.io/github/oskarruo/tiralabra)

## Testing

The testing is currently done using unit tests and manual tests. 
Currently the lz78, huffman, and bitio files are tested. 
Main function tests will be added in the future. 
The compression algorithms are tested with a short 10 length string input, and a longer 3 547 byte size file with natural language. 
The tests compare each line of the original and decompressed files and fails if they don't match. 
The unit tests can be replicated by building the project with CMake (using MinGW Makefiles) and then running CTest in the build directory.

Testing the efficiency of the algorithms will be done in the future.
