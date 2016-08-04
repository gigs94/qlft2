

INCLUDES = $(wildcard src/*.h)
TEST_INCLUDES = $(wildcard test/*.hpp)


all: main

tester: $(INCLUDES) $(TEST_INCLUDES) test/test.cpp 
	clang++ -pedantic -std=c++11 -o tester test/test.cpp -I/usr/local/include -I./src -I./test -lcpptest -lboost_serialization

test: tester

decompressor:  $(INCLUDES) src/ebat_decompressor.cpp
	clang++ -pedantic -std=c++11 -o decompressor src/ebat_decompressor.cpp -I./src -lboost_serialization

compressor:  $(INCLUDES) src/ebat_compressor.cpp
	clang++ -pedantic -std=c++11 -o compressor src/ebat_compressor.cpp -I./src -lboost_serialization

main: compressor decompressor

