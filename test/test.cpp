//#include <FieldTest.hpp>
#include <CsvParserTest.hpp>
#include <IntCompressTest.hpp>
#include <DeltaCompressTest.hpp>
#include <DeltaBlockTest.hpp>
//#include <FloatCompressTest.hpp>
#include <StringCompressTest.hpp>
#include <PackDupsTest.hpp>

int main ( ) {
  //FieldTest fieldTest;    // moved this to unused since we ended up going a different direction
  CsvParserTest csvParserTest;
  IntCompressTest intCompressTest;
  DeltaCompressTest deltaCompressTest;
  DeltaBlockTest deltaBlockTest;
  //FloatCompressTest floatCompressTest;    // moved this to unused since we ended up going a different direction
  StringCompressTest stringCompressTest;
  PackDupsTest packDupsTest;

  Test::TextOutput output(Test::TextOutput::Verbose);

  int rtn;
  // rtn = rtn | fieldTest.run(output) ? 1 : 0;
  rtn = rtn |  csvParserTest.run(output) ? 1 : 0;
  rtn = rtn |  intCompressTest.run(output) ? 1 : 0;
  rtn = rtn |  deltaCompressTest.run(output) ? 1 : 0;
  rtn = rtn |  deltaBlockTest.run(output) ? 1 : 0;
  //rtn = rtn |  floatCompressTest.run(output) ? 1 : 0;
  rtn = rtn |  stringCompressTest.run(output) ? 1 : 0;
  rtn = rtn |  packDupsTest.run(output) ? 1 : 0;
  return rtn;
}
