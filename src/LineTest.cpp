#include <cpptest.h>
#include <Line.h>
#include <fstream>

class LineTest : public Test::Suite { 

    void ConstructLine() {
       std::ifstream infile{"ebat.csv"};
       _line = new Line{infile};
    }

    void LineReadValues() {
       TEST_ASSERT_EQUALS("USD.JPY",_line->stringValue());
       TEST_ASSERT_EQUALS("x",_line->stringValue());
       TEST_ASSERT_EQUALS("A",_line->stringValue());
       TEST_ASSERT_EQUALS(0,_line->intValue());
       TEST_ASSERT_EQUALS(6311300000,_line->longValue());
       TEST_ASSERT_EQUALS(6311300770,_line->longValue());

       // ??? For some reason when this was written like the rest of the 
       // ??? asserts in this block, it called _getFloat twice and even
       // ??? pulling x out of the equation didn't work, so there is something
       // ??? hosed in the cpptest macro wrt floats.... just be aware.
       float x=_line->floatValue();
       float expectedValue{97.96};
       TEST_ASSERT_EQUALS(expectedValue,x);
       TEST_ASSERT_EQUALS(0,_line->intValue());
    }

    void ReadMultipleLines() {
       _line->moveToNextLine();
       TEST_ASSERT_EQUALS("USD.JPY",_line->stringValue());
       TEST_ASSERT_EQUALS("x",_line->stringValue());
       TEST_ASSERT_EQUALS("A",_line->stringValue());
       TEST_ASSERT_EQUALS(0,_line->intValue());
       TEST_ASSERT_EQUALS(6311300000,_line->longValue());
       TEST_ASSERT_EQUALS(6311300770,_line->longValue());

       // ??? For some reason when this was written like the rest of the 
       // ??? asserts in this block, it called _getFloat twice and even
       // ??? pulling x out of the equation didn't work, so there is something
       // ??? hosed in the cpptest macro wrt floats.... just be aware.
       float x=_line->floatValue();
       float expectedValue{97.91};
       TEST_ASSERT_EQUALS(expectedValue,x);
       TEST_ASSERT_EQUALS(10,_line->intValue());
    }

    public: 
      LineTest( ) {
         TEST_ADD(LineTest::ConstructLine);
         TEST_ADD(LineTest::LineReadValues);
         TEST_ADD(LineTest::ReadMultipleLines);
      } 

      ~LineTest( ) {
          delete _line;
      }

    private:
       Line* _line;


}; 
