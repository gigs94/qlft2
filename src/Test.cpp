#include <FieldTest.hpp>
#include <CsvParserTest.cpp>
#include <IntCompressTest.cpp>
#include <DeltaCompressTest.cpp>
#include <StringCompressTest.hpp>

int main ( ) {
  FieldTest fieldTest;
  CsvParserTest csvParserTest;
  IntCompressTest intCompressTest;
  DeltaCompressTest deltaCompressTest;
  StringCompressTest stringCompressTest;

  Test::TextOutput output(Test::TextOutput::Verbose);

  int rtn = fieldTest.run(output) ? 1 : 0;
  rtn = rtn |  csvParserTest.run(output) ? 1 : 0;
  rtn = rtn |  intCompressTest.run(output) ? 1 : 0;
  rtn = rtn |  deltaCompressTest.run(output) ? 1 : 0;
  rtn = rtn |  stringCompressTest.run(output) ? 1 : 0;
  return rtn;
}
