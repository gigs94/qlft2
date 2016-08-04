
# ebat file compression #

Compresses an ebat file and decompresses it.   Uses column based optimation algorithms and does not use any compression algorithms like LZ77.


## Build ##

to build and run the test cases:
make test && ./tester

to build and run the compressor:
make && ./compressor && ./decompressor

## Compressor ##
Usage: ./compressor [-i <filename>] [-o <filename>] [-c <block size>]
   defaults:  -i ebat.csv -o ebat.compressed -c 500

## Decompressor ##
Usage: ./decompressor [-i <filename>] [-o <filename>]
   defaults:  -i ebat.compressed -o ebat.new.csv


