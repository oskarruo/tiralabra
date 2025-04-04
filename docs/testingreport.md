# Testing report

## Test coverage

The live test coverage report can be found from codecov:

[![codecov](https://codecov.io/github/oskarruo/tiralabra/graph/badge.svg?token=XWZXH8K9VF)](https://codecov.io/github/oskarruo/tiralabra)

## Testing

The program is tested automatically with unit tests and manually with efficiency testing

## Unit testing

The compression algorithms are unit tested with varying input sizes from 1 byte to ~1MB. 
The tests compare each line of the original and decompressed files and fails if they don't match.
The main function of the program is unit tested with possible error cases.
The main tests also verify that the main function interacts correctly with the compression functions.
So they could be considered as end-to-end tests as well. 
The unit tests can be replicated by building the project with CMake (using MinGW Makefiles) and then running CTest in the build directory.

## Efficiency testing

The program's efficiency is tested manually by running the program on .txt file eBooks containing natural language from the Project Gutenberg library.
The test inputs are converted into ASCII only, as the program does not support UTF-8 characters.

The selected test inputs are:

| Number | Title | Size |
|---- | ----- | ------------- |
| 1 | [Harrods for Everything by Harrods Ltd.](https://www.gutenberg.org/ebooks/61985)  | 6 098 928 bytes |
| 2 | [Henley's Twentieth Century Formulas, Recipes and Processes by Gardner Dexter Hiscox](https://www.gutenberg.org/ebooks/53143) | 3 647 553 bytes |
| 3 | [Terminal Compromise by Winn Schwartau](https://www.gutenberg.org/ebooks/79)  | 1 352 182 bytes  |
| 4 | [Soldering, Brazing and Welding by Bernard E. Jones](https://www.gutenberg.org/ebooks/52074) | 232 944 bytes |
| 5 | [Hitchhiker's Guide to the Internet by Ed Krol](https://www.gutenberg.org/ebooks/39) | 70 552 bytes |

### LZ78

| File Number | Compression time | Compressed size | Decompression time |
|---- | ----- | ------------- | ---- |
| 1 | 1.63081 s | 2 523 983 bytes (41.384% of original) | 0.963513 s |
| 2 | 0.923856 s | 1 549 077 bytes (42.4689% of original) | 0.590499 s |
| 3 | 0.328629 s | 676 879 bytes (50.0583% of original)  | 0.264959 s |
| 4 | 0.0526217 s | 119 188 bytes (51.1659% of original) | 0.0494239 s |
| 5 | 0.0168942 s | 38 446 bytes (54.4931% of original) | 0.0160961 s |

### Huffman

| File Number | Compression time | Compressed size | Decompression time |
|---- | ----- | ------------- | ---- |
| 1 | 0.814294 s | 3 503 596 bytes (57.4461% of original) | 0.485306 s |
| 2 | 0.497773 s | 2 054 791 bytes (56.3334% of original) | 0.294376 s |
| 3 | 0.18845 s | 774 266 bytes (57.2605% of original) | 0.11193 s |
| 4 | 0.0324285 s | 131 391 bytes (56.4045% of original) | 0.0191856 s |
| 5 | 0.0104685 s | 39 845 bytes (56.4761% of original) | 0.0062247 s |