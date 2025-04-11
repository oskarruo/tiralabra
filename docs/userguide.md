# User guide

## Prerequisites

The following tools are needed to build the program:

- [CMake](https://cmake.org/download/) (version 3.15 and up)
- A C++ compiler
  - **GCC** is recommended, as the project has been tested on it. Specifically the unit tests or coverage may not function properly with other compilers such as MSVC

Optional:
- [gcovr](https://gcovr.com/en/stable/installation.html)
   - To create local coverage reports. Note that coverage is also available via Codecov.

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

**Note: as stated earlier, it is recommended to use GCC. If you're using another compiler (e.g., MSVC), you can specify GCC by adding -G "MinGW Makefiles" to the cmake .. command (assuming you have MinGW installed).**

## Running tests

After the project has built, the tests can be run in the build dir with:

```
ctest
```

(Optional) A local html coverage report can be created with:

```
gcovr -r .. --filter ../src/ --exclude '../test/.*' --html --html-details --gcov-ignore-parse-errors=suspicious_hits.warn -o coverage_report.html
```

**Note: as specified earlier, gcovr is needed for this.**

## Using the program

If built with GCC, the program will be located in the src directory of the build dir.

The program works on the command line followingly:

```
main.exe [options] input_file.txt/.bin
```
or 
```
./main [options] input_file.txt/.bin
```
where the options are:
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