#include <cpptest.h>
#include <Field.h>
#include <CsvParser.h>
#include <fstream>
#include <vector>

class FieldTest : public Test::Suite { 

    void ConstructRow() {
       std::ifstream infile{"ebat.csv"};
       _parser = new CsvParser{infile};
       Row field;
    }

    void AddField() {
       Row field;
       field.addField("test", FieldType::stringT);
    }

    void UseRowToGetValuesOutOfEbat() {
       Row field;
       field.addField("stockName", FieldType::stringT);
       field.addField("exchange", FieldType::stringT);
       field.addField("side", FieldType::stringT);
       field.addField("condition", FieldType::stringT);
       field.addField("time", FieldType::longT);
       field.addField("reptime", FieldType::longT);
       field.addField("price", FieldType::floatT);
       field.addField("size", FieldType::intT);

       std::vector<Field> fields = field.parseLine(*_parser);

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

    public: 
      FieldTest( ) {
         TEST_ADD(FieldTest::ConstructRow);
         TEST_ADD(FieldTest::UseRowToGetValuesOutOfEbat);
      } 

      ~FieldTest() {
          delete _parser;
      }

      CsvParser* _parser;
}; 
