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
The unit tests can be replicated by building the project with CMake (using GCC) and then running CTest in the build directory.

## Efficiency testing

The program's efficiency is tested manually by running the program on .txt file eBooks containing natural language from the Project Gutenberg library.
The test inputs are converted into ASCII only, as the program does not support UTF-8 characters.
The tests are run on a PC with i7-10700k CPU and Samsung 970 EVO Plus SSD.

The selected test inputs are:

| Number | Title | Size |
|---- | ----- | ------------- |
| 1 | [Harrods for Everything by Harrods Ltd.](https://www.gutenberg.org/ebooks/61985)  | 6 098 928 bytes |
| 2 | [Henley's Twentieth Century Formulas, Recipes and Processes by Gardner Dexter Hiscox](https://www.gutenberg.org/ebooks/53143) | 3 647 553 bytes |
| 3 | [Terminal Compromise by Winn Schwartau](https://www.gutenberg.org/ebooks/79)  | 1 352 182 bytes  |
| 4 | [Soldering, Brazing and Welding by Bernard E. Jones](https://www.gutenberg.org/ebooks/52074) | 232 944 bytes |
| 5 | [Hitchhiker's Guide to the Internet by Ed Krol](https://www.gutenberg.org/ebooks/39) | 70 552 bytes |

### Results: LZ78

| File Number | Compression time | Compressed size | Decompression time |
|-------------|------------------|------------------|-------------------|
| 1 | 0.341998 s | 2 542 288 bytes (41.1750% of original) | 0.384091 s |
| 2 | 0.210535 s | 1 554 548 bytes (42.3823% of original) | 0.247716 s |
| 3 | 0.080851 s | 677 074 bytes (50.0663% of original) | 0.107590 s |
| 4 | 0.012414 s | 119 289 bytes (51.1526% of original) | 0.015879 s |
| 5 | 0.004702 s | 38 534 bytes (54.4843% of original) | 0.005996 s |

### Results: Huffman

| File Number | Compression time | Compressed size | Decompression time |
|-------------|------------------|------------------|-------------------|
| 1 | 0.150733 s | 3 558 160 bytes (57.6281% of original) | 0.219807 s |
| 2 | 0.094061 s | 2 077 634 bytes (56.6434% of original) | 0.131722 s |
| 3 | 0.036302 s | 774 466 bytes (57.2680% of original) | 0.049493 s |
| 4 | 0.006366 s | 131 618 bytes (56.4394% of original) | 0.008785 s |
| 5 | 0.002350 s | 39 964 bytes (56.5062% of original) | 0.003145 s |

### Analysis

![image](test_graphs.png)

Based on the test results it seems that the Huffman coding algorithm is faster in both compression and decompression.
This time difference gap seems to grow for both compression and decompression.

In terms of compression efficiency, the LZ78 gets better results.
As the input size grows larger, the LZ78 algorithm gets more size reduction.
Notably, it seems that the input size has no effect on the compression efficiency of the Huffman coding algorithm.
Some of the larger files actually get worse results than smaller ones. 