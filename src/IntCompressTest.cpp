#include <cpptest.h>
#include <IntCompress.h>
#include <vector>
#include <iostream>

class IntCompressTest : public Test::Suite { 

    void ConstructIntCompress() {
       std::vector<uint64_t> values{6311300770,6311300770,6311300770,6311301558,6311301558,6311301558,6311304804,6311400731,6311400731,6311401507,6311401507,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373};
       IntCompress compress{values};

       std::stringstream out;
       out << compress;

       IntCompress decompress{};
       out >> decompress;

       TEST_ASSERT_EQUALS(compress,decompress);
    }

    public: 
      IntCompressTest( ) {
         TEST_ADD(IntCompressTest::ConstructIntCompress);
      } 

}; 
