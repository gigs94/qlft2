all: main

tester: src/CsvParserTest.cpp src/CsvParser.h src/FieldTest.hpp src/Field.h src/Test.cpp src/IntCompressTest.cpp src/DeltaCompressTest.cpp src/DeltaCompress.h src/IntCompress.h src/StringCompress.h src/StringCompressTest.hpp
	clang++ -pedantic -std=c++11 -o tester src/Test.cpp -I/usr/local/include -I./src  -lcpptest -lboost_serialization

test: tester

stock_processor: 
	clang++ -pedantic -std=c++11 -o ebat_processor src/main.cpp -I./src -lboost_serialization

main: stock_processor

