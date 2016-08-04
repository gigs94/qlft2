#include <cpptest.h>
#include <DeltaCompress.h>
#include <ReorderVector.h>
#include <vector>
#include <iostream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>


class DeltaCompressTest : public Test::Suite { 

     std::vector<int64_t> values{6311300770,6311300770,6311300770,6311301558,6311301558,6311301558,6311304804,6311400731,6311400731,6311401507,6311401507,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,-123123,-123123,5883581383};

    void ConstructDeltaCompress() {
       std::vector<int64_t> dvalues{};

       DeltaCompress _compress{values};
       DeltaCompress _decompress{};


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

    void sortIndexesTest() {
        for ( auto i : sort_indexes(values) ) {
            //std::cout << values[i] << std::endl;
        }
    }

    public: 
      DeltaCompressTest( ) {
         TEST_ADD(DeltaCompressTest::ConstructDeltaCompress);
         TEST_ADD(DeltaCompressTest::sortIndexesTest);
      } 

}; 
