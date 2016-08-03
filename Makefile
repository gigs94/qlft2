

INCLUDES = $(wildcard src/*.h)
TEST_INCLUDES = $(wildcard src/*.hpp)


all: main

tester: $(INCLUDES) $(TEST_INCLUDES) src/test.cpp 
	clang++ -pedantic -std=c++11 -o tester src/Test.cpp -I/usr/local/include -I./src  -lcpptest -lboost_serialization

test: tester

decompressor:  $(INCLUDES) src/ebat_decompressor.cpp
	clang++ -pedantic -std=c++11 -o decompressor src/ebat_decompressor.cpp -I./src -lboost_serialization

compressor:  $(INCLUDES) src/ebat_compressor.cpp
	clang++ -pedantic -std=c++11 -o compressor src/ebat_compressor.cpp -I./src -lboost_serialization

main: compressor decompressor

