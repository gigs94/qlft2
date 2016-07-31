// ???  I should definitely NOT include cpp files...
#include <FieldsTest.cpp>
#include <LineTest.cpp>

int main ( ) {
  FieldsTest fieldsTest;
  LineTest lineTest;

  Test::TextOutput output(Test::TextOutput::Verbose);

  int rtn = fieldsTest.run(output) ? 1 : 0;
  rtn = rtn |  lineTest.run(output) ? 1 : 0;
  return rtn;
}
