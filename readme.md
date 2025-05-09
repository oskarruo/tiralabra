# Compression Comparison

A project comparing the efficiency of LZ78 and Huffman coding compression/decompression algorithms.

[![Tests](https://github.com/oskarruo/tiralabra/actions/workflows/test-and-coverage-linux.yaml/badge.svg)](https://github.com/oskarruo/tiralabra/actions/workflows/test-and-coverage-linux.yaml)
[![codecov](https://codecov.io/github/oskarruo/tiralabra/graph/badge.svg?token=XWZXH8K9VF)](https://codecov.io/github/oskarruo/tiralabra)

## Documentation
- [Project specification](/docs/specifications.md)
- [Testing report](/docs/testingreport.md)
- [Implementation report](/docs/implementationreport.md)
- [User guide](/docs/userguide.md)

## Weekly reports
- [Week 1](/docs/weeklyreports/weeklyreport1.md)
- [Week 2](/docs/weeklyreports/weeklyreport2.md)
- [Week 3](/docs/weeklyreports/weeklyreport3.md)
- [Week 4](/docs/weeklyreports/weeklyreport4.md)
- [Week 5](/docs/weeklyreports/weeklyreport5.md)
- [Week 6](/docs/weeklyreports/weeklyreport6.md)

## Installation and usage

**A more detailed guide can be found in the [docs folder](/docs/userguide.md)**

### Downloading

You can download the pre-compiled versions of the program for your platform:
- **For Linux:** [Download](https://github.com/oskarruo/tiralabra/releases/download/final/main)
- **For Windows:** [Download](https://github.com/oskarruo/tiralabra/releases/download/final/main.exe)

Alternatively, you can build the program from source:

### Building prerequisites

The following tools are needed to build the program:

- [CMake](https://cmake.org/download/) (version 3.15 and up)
- A C++ compiler (**GCC** is recommended, and these instructions are specifically written for it)

### Building

Clone the project:

```
git clone https://github.com/oskarruo/tiralabra
```

In the project directory create a build folder, configure the project and build it:

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

If built with GCC, the program will be in build/src

### Using the program

The program works on the command line followingly:

```
main.exe [options] input_filepath
```
or 
```
./main [options] input_filepath
```
**An input file to be compressed must be a .txt file.**

**An input file to be decompressed must be a .bin file.**

The options are:
```
-a "lz78"(default)/"huffman"  
    Specifies the compression algorithm. If omitted, LZ78 is used by default.
    Note: This argument is not needed when decompressing, as the algorithm is inferred automatically.

-o output_filename  
    Specifies the output file name.
    Defaults:
      - Compression → original_compressed.bin
      - Decompression → original_decompressed.txt
```
The program will write the output file to the same directory where the program is in.