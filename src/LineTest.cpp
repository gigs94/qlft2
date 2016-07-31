#include <cpptest.h>
#include <Line.h>
#include <fstream>

class LineTest : public Test::Suite { 

    void ConstructLine() {
       infile = std::move(std::ifstream("ebat.csv"));
    }

    void LineReadValues() {
       Line line = getNextLine();
       TEST_ASSERT_EQUALS("USD.JPY",line.stringValue());
       TEST_ASSERT_EQUALS("x",line.stringValue());
       TEST_ASSERT_EQUALS("A",line.stringValue());
       TEST_ASSERT_EQUALS(0,line.intValue());
       TEST_ASSERT_EQUALS(6311300000,line.longValue());
       TEST_ASSERT_EQUALS(6311300770,line.longValue());

       // ??? For some reason when this was written like the rest of the 
       // ??? asserts in this block, it called _getFloat twice and even
       // ??? pulling x out of the equation didn't work, so there is something
       // ??? hosed in the cpptest macro wrt floats.... just be aware.
       float x=line.floatValue();
       float expectedValue{97.96};
       TEST_ASSERT_EQUALS(expectedValue,x);
       TEST_ASSERT_EQUALS(0,line.intValue());
    }

    void ReadMultipleLines() {
       // ??? so how do we get to the next line now?   we have a throw statement in case we get to the last line in the input file but for the next row can we just continue?   Let's test it and see.
       Line line = getNextLine();
       TEST_ASSERT_EQUALS("USD.JPY",line.stringValue());
       TEST_ASSERT_EQUALS("x",line.stringValue());
       TEST_ASSERT_EQUALS("A",line.stringValue());
       TEST_ASSERT_EQUALS(0,line.intValue());
       TEST_ASSERT_EQUALS(6311300000,line.longValue());
       TEST_ASSERT_EQUALS(6311300770,line.longValue());

       // ??? For some reason when this was written like the rest of the 
       // ??? asserts in this block, it called _getFloat twice and even
       // ??? pulling x out of the equation didn't work, so there is something
       // ??? hosed in the cpptest macro wrt floats.... just be aware.
       float x=line.floatValue();
       float expectedValue{97.91};
       TEST_ASSERT_EQUALS(expectedValue,x);
       TEST_ASSERT_EQUALS(10,line.intValue());
    }

    public: 
      LineTest( ) {
         TEST_ADD(LineTest::ConstructLine);
         TEST_ADD(LineTest::LineReadValues);
         TEST_ADD(LineTest::ReadMultipleLines);
      } 

    private:
       std::ifstream infile;

       Line getNextLine() {
           return std::move(Line(infile));
       }

}; 
    

int main ( ) { 
  LineTest lineTest;
  Test::TextOutput output(Test::TextOutput::Verbose);
  return lineTest.run(output) ? 1 : 0;
}
