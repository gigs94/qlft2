#include <cpptest.h>
#include <Field.h>
#include <fstream>

class FieldTest : public Test::Suite { 

    void ConstructField() {
       std::ifstream infile("ebat.csv");
       Field field(infile, stringFT);  
    }

    void FieldReadValues() {
       std::ifstream infile("ebat.csv");
       Field field(infile, stringFT);

       TEST_ASSERT_EQUALS("USD.JPY",field.stringValue());
    }

    public: 
      FieldTest( ) {
         TEST_ADD(FieldTest::ConstructField);
         TEST_ADD(FieldTest::FieldReadValues);
      } 

}; 
    

int main ( ) { 
  FieldTest fieldTest;
  Test::TextOutput output(Test::TextOutput::Verbose);
  return fieldTest.run(output) ? 1 : 0;
}
