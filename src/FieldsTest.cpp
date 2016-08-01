#include <cpptest.h>
#include <Fields.h>
#include <Line.h>
#include <fstream>
#include <vector>

class FieldsTest : public Test::Suite { 

    void ConstructFields() {
       std::ifstream infile{"ebat.csv"};
       _line = new Line{infile};
       Fields field;
    }

    void AddField() {
       Fields field;
       field.addField("test", FieldType::stringT);
    }

    void UseFieldsToGetValuesOutOfEbat() {
       Fields field;
       field.addField("stockName", FieldType::stringT);
       field.addField("exchange", FieldType::stringT);
       field.addField("side", FieldType::stringT);
       field.addField("condition", FieldType::stringT);
       field.addField("time", FieldType::longT);
       field.addField("reptime", FieldType::longT);
       field.addField("price", FieldType::floatT);
       field.addField("size", FieldType::intT);

       std::vector<Field> fields = field.getLine(*_line);

       TEST_ASSERT_EQUALS("USD.JPY",fields[0].s);
       TEST_ASSERT_EQUALS("x",fields[1].s);
       TEST_ASSERT_EQUALS("A",fields[2].s);
       TEST_ASSERT_EQUALS("0",fields[3].s);
       TEST_ASSERT_EQUALS(6311300000,fields[4].l);
       TEST_ASSERT_EQUALS(6311300770,fields[5].l);

       // ??? For some reason when this was written like the rest of the 
       // ??? asserts in this block, it called _getFloat twice and even
       // ??? pulling x out of the equation didn't work, so there is something
       // ??? hosed in the cpptest macro wrt floats.... just be aware.
       float x=fields[6].f;
       float expectedValue{97.96};
       TEST_ASSERT_EQUALS(expectedValue,x);
       TEST_ASSERT_EQUALS(0,fields[7].i);
    }
/*
    void ReadMultipleFieldss() {
       // ??? so how do we get to the next line now?   we have a throw statement in case we get to the last line in the input file but for the next row can we just continue?   Let's test it and see.
       Fields line = getNextFields();
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
    } */

    public: 
      FieldsTest( ) {
         TEST_ADD(FieldsTest::ConstructFields);
         TEST_ADD(FieldsTest::UseFieldsToGetValuesOutOfEbat);
         //TEST_ADD(FieldsTest::FieldsReadValues);
         //TEST_ADD(FieldsTest::ReadMultipleFieldss);
      } 

      ~FieldsTest() {
          delete _line;
      }

      Line* _line;
}; 
