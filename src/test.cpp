#include <FieldTest.hpp>
#include <CsvParserTest.hpp>
#include <IntCompressTest.hpp>
#include <DeltaCompressTest.hpp>
#include <StringCompressTest.hpp>
#include <PackDupsTest.hpp>

int main ( ) {
  FieldTest fieldTest;
  CsvParserTest csvParserTest;
  IntCompressTest intCompressTest;
  DeltaCompressTest deltaCompressTest;
  StringCompressTest stringCompressTest;
  PackDupsTest packDupsTest;

  Test::TextOutput output(Test::TextOutput::Verbose);

  int rtn = fieldTest.run(output) ? 1 : 0;
  rtn = rtn |  csvParserTest.run(output) ? 1 : 0;
  rtn = rtn |  intCompressTest.run(output) ? 1 : 0;
  rtn = rtn |  deltaCompressTest.run(output) ? 1 : 0;
  rtn = rtn |  stringCompressTest.run(output) ? 1 : 0;
  rtn = rtn |  packDupsTest.run(output) ? 1 : 0;
  return rtn;
}