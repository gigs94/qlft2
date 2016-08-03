#include <cpptest.h>
#include <StringCompress.h>
#include <vector>
#include <iostream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

class StringCompressTest : public Test::Suite { 

    void ConstructStringCompress() {
       StringCompress sc{};
    }

    void ContructWithValuesAndRecreate() {
       std::vector<std::string> values{"asdf", "asdf", "qwer", "zcxv"};
       StringCompress sc{values};

       std::vector<std::string> outvalues = sc.decompress();

       StringCompress sc1{outvalues};
       TEST_ASSERT_EQUALS(sc1,sc);
    }

    void testInEquality() {
       std::vector<std::string> values{"asdf", "asdf", "qwer", "zcxv"};
       std::vector<std::string> values1{"asdf", "qwer", "zcxv"};
       StringCompress sc{values};
       StringCompress sc1{values1};

std::cout << sc << std::endl;

       TEST_ASSERT(sc1 != sc);
    }

    void testSerialization() {
       std::vector<std::string> values{"asdf", "asdf", "qwer", "zcxv"};
       StringCompress sc{values};
       StringCompress sc1{};

       // save data to archive
       {
           std::ofstream ofs("filename");
           boost::archive::binary_oarchive oa(ofs);
           oa << sc;
       }

       {
           std::ifstream ifs("filename");
           boost::archive::binary_iarchive ia(ifs);
           ia >> sc1;
       }

       TEST_ASSERT(sc1 == sc);
    }



    public: 
      StringCompressTest( ) {
         TEST_ADD(StringCompressTest::ConstructStringCompress);
         TEST_ADD(StringCompressTest::ContructWithValuesAndRecreate);
         TEST_ADD(StringCompressTest::testInEquality);
         TEST_ADD(StringCompressTest::testSerialization);
      } 

}; 
