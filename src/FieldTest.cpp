#include <cpptest.h>
#include <Field.h>
#include <fstream>

class FieldTest : public Test::Suite { 

    void ConstructField() {
       std::ifstream infile("ebat.csv");
       Field field(infile, stringFT);  
    }

    public: 
      void CompressTest( ) {
         TEST_ADD(FieldTest::ConstructField);
      } 

}; 
    

int main ( ) { 
  FieldTest fieldTest;
  Test::TextOutput output(Test::TextOutput::Terse);
  return fieldTest.run(output) ? 1 : 0;
}
