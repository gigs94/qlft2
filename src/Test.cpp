// ???  I should definitely NOT include cpp files...
#include <FieldsTest.cpp>
#include <LineTest.cpp>
#include <IntCompressTest.cpp>
#include <DeltaCompressTest.cpp>
#include <StringCompressTest.hpp>

int main ( ) {
  FieldsTest fieldsTest;
  LineTest lineTest;
  IntCompressTest intCompressTest;
  DeltaCompressTest deltaCompressTest;
  StringCompressTest stringCompressTest;

  Test::TextOutput output(Test::TextOutput::Verbose);

  int rtn = fieldsTest.run(output) ? 1 : 0;
  rtn = rtn |  lineTest.run(output) ? 1 : 0;
  rtn = rtn |  intCompressTest.run(output) ? 1 : 0;
  rtn = rtn |  deltaCompressTest.run(output) ? 1 : 0;
  rtn = rtn |  stringCompressTest.run(output) ? 1 : 0;
  return rtn;
}
