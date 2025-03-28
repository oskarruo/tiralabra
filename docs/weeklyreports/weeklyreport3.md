# Weekly report, week 2

This week I wrote the Huffman compression and decompression algorithms and tests for them.
I also implemented writer/reader classes for writing and reading bits in .bin files and changed the lz78 algorithms to use them.
I also added a pre-commit hook that lints and formats the code automatically.
I learned to handle the writing and reading of bits. 
It was a bit tricky to write at first and to get it work with the algorithms, but eventually I got it working. 
I tried to get started with writing tests for the main file, but due to time constraints and problems setting the tests up I decided to postpone this to hopefully next week.
The test coverage is currently only 64% because of this, but based on my manual testing it *should* work.
Also the huffman algorithm is quite slow at the moment.
I will have to look into it and try to make it faster.

## Used hours

| Day | Time used | Description |
| ----- | ------------- | ------ |
| 24.3.  | 5h            | Reading source material, writing the huffman algorithms |
| 27.3.  | 4h            | Writing the bitio reader/writer classes and seting the algorithms to use them |
| 28.3.  | 5h            | Fixing bugs, writing tests, writing documentation |
| Total | 14h         |        |