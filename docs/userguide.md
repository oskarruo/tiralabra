# User guide

## Prerequisites

The following tools are needed to build the program:

- [CMake](https://cmake.org/download/) (version 3.15 and up)
- A C++ compiler
  - **GCC** is recommended, as the project has been tested on it. Specifically the unit tests or coverage may not function properly with other compilers such as MSVC

Optional:
- [python](https://www.python.org/downloads/)
  
  The following libraries are needed for running automated efficiency tests
  - requests
  - unidecode
  - matplotlib
  
  The following are needed for creating test coverage reports
  - gcovr

The libraries can be installed with:
```
pip install requests unidecode matplotlib gcovr
```


## Installation

Clone the project:

```
git clone https://github.com/oskarruo/tiralabra
```

In the project directory create a build folder, configure the project and build it:

```
mkdir build
cd build
cmake ..
cmake --build .
```

**Note: as stated earlier, it is recommended to use GCC. If you have other compilers (e.g., MSVC), you can specify GCC by adding -G "MinGW Makefiles" to the cmake .. command (assuming you have MinGW installed).**

## Running unit tests

After the project has built, the tests can be run in the build dir with:

```
ctest
```

## (OPTIONAL) Creating an html coverage report:

A local html coverage report can be created with

```
gcovr -r .. --filter ../src/ --exclude '../test/.*' --html --html-details --gcov-ignore-parse-errors=suspicious_hits.warn -o coverage_report.html
```

**Note: as specified earlier, python and gcovr are needed for this.**

## (OPTIONAL) Running the automated efficiency tests 

After the project has built and an executable exists in the build/src folder, the efficiency tests can be run automatically by running
```
python efficiency_tests.py
```
in the tests/efficiencytesting folder. This will print a markdown format table and plots of the results.

**Note: as specified earlier, python, requests, unidecode, and matplotlib are needed for this.**

## Using the program

If built with GCC, the program will be located in the src directory of the build dir.

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

The operation compress/decompress is inferred by the program based on the file extension.

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