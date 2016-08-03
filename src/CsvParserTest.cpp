#include <cpptest.h>
#include <CsvParser.h>
#include <fstream>

class CsvParserTest : public Test::Suite { 

    void ConstructCsvParser() {
       std::ifstream infile{"ebat.csv"};
       _parser = new CsvParser{infile};
    }

    void CsvParserReadValues() {
       TEST_ASSERT_EQUALS("USD.JPY",_parser->stringValue());
       TEST_ASSERT_EQUALS("x",_parser->stringValue());
       TEST_ASSERT_EQUALS("A",_parser->stringValue());
       TEST_ASSERT_EQUALS(0,_parser->intValue());
       TEST_ASSERT_EQUALS(6311300000,_parser->longValue());
       TEST_ASSERT_EQUALS(6311300770,_parser->longValue());

       // ??? For some reason when this was written like the rest of the 
       // ??? asserts in this block, it called _getFloat twice and even
       // ??? pulling x out of the equation didn't work, so there is something
       // ??? hosed in the cpptest macro wrt floats.... just be aware.
       float x=_parser->floatValue();
       float expectedValue{97.96};
       TEST_ASSERT_EQUALS(expectedValue,x);
       TEST_ASSERT_EQUALS(0,_parser->intValue());
    }

    void ReadMultipleCsvParsers() {
       _parser->moveToNextLine();
       TEST_ASSERT_EQUALS("USD.JPY",_parser->stringValue());
       TEST_ASSERT_EQUALS("x",_parser->stringValue());
       TEST_ASSERT_EQUALS("A",_parser->stringValue());
       TEST_ASSERT_EQUALS(0,_parser->intValue());
       TEST_ASSERT_EQUALS(6311300000,_parser->longValue());
       TEST_ASSERT_EQUALS(6311300770,_parser->longValue());

       // ??? For some reason when this was written like the rest of the 
       // ??? asserts in this block, it called _getFloat twice and even
       // ??? pulling x out of the equation didn't work, so there is something
       // ??? hosed in the cpptest macro wrt floats.... just be aware.
       float x=_parser->floatValue();
       float expectedValue{97.91};
       TEST_ASSERT_EQUALS(expectedValue,x);
       TEST_ASSERT_EQUALS(10,_parser->intValue());
    }

    public: 
      CsvParserTest( ) {
         TEST_ADD(CsvParserTest::ConstructCsvParser);
         TEST_ADD(CsvParserTest::CsvParserReadValues);
         TEST_ADD(CsvParserTest::ReadMultipleCsvParsers);
      } 

      virtual ~CsvParserTest( ) {
          delete _parser;
      }

    private:
       CsvParser* _parser;


}; 
