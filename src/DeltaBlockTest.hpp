#include <cpptest.h>
#include <DeltaBlock.h>
#include <vector>
#include <iostream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <cstdio>


class DeltaBlockTest : public Test::Suite { 

     std::vector<int64_t> values{6311300770,6311300770,6311300770,6311301558,6311301558,6311301558,6311304804,6311400731,6311400731,6311401507,6311401507,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,-123123,-123123,5883581383};

    void ConstructDeltaBlock() {
       std::string filename{".tmp.file.asdfasdiwvar"};
       std::vector<int64_t> dvalues{};

       DeltaBlock _compress{values,5};
       DeltaBlock _decompress{};

       // save data to archive
       {
           std::ofstream ofs(filename);
           boost::archive::binary_oarchive oa(ofs);
           oa << _compress;
       }
   
       {
           std::ifstream ifs(filename);
           boost::archive::binary_iarchive ia(ifs);
           ia >> _decompress;
           dvalues = _decompress.decompress();
       }

       std::remove(filename.c_str());

       TEST_ASSERT(values==dvalues);
       TEST_ASSERT_EQUALS(_compress,_decompress);
    }

    public: 
      DeltaBlockTest( ) {
         TEST_ADD(DeltaBlockTest::ConstructDeltaBlock);
      } 

}; 
