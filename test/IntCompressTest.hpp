#include <cpptest.h>
#include <IntCompress.h>
#include <vector>
#include <iostream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

class IntCompressTest : public Test::Suite { 

    void ConstructIntCompress() {
       std::vector<int64_t> values{6311300770,6311300770,6311300770,6311301558,6311301558,6311301558,6311304804,6311400731,6311400731,6311401507,6311401507,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373};
       std::vector<int64_t> dvalues{};

       IntCompress _compress{values};
       IntCompress _decompress{};


       // save data to archive
       {
           std::ofstream ofs("filename");
           boost::archive::binary_oarchive oa(ofs);
           oa << _compress;
       }
   
       {
           std::ifstream ifs("filename");
           boost::archive::binary_iarchive ia(ifs);
           ia >> _decompress;
           dvalues = _decompress.decompress();
       }


       TEST_ASSERT(values==dvalues);
       TEST_ASSERT_EQUALS(_compress,_decompress);
    }

    public: 
      IntCompressTest( ) {
         TEST_ADD(IntCompressTest::ConstructIntCompress);
      } 

}; 
