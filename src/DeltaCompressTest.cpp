#include <cpptest.h>
#include <DeltaCompress.h>
#include <vector>
#include <iostream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

class DeltaCompressTest : public Test::Suite { 

    void ConstructDeltaCompress() {
       std::vector<uint64_t> values{6311300770,6311300770,6311300770,6311301558,6311301558,6311301558,6311304804,6311400731,6311400731,6311401507,6311401507,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373};
       std::vector<uint64_t> dvalues{};

       DeltaCompress _compress{values};
       DeltaCompress _decompress{};


       // save data to archive
       {
           std::ofstream ofs("filename");
           boost::archive::binary_oarchive oa(ofs);
           // write class instance to archive
           oa << _compress;
           // archive and stream closed when destructors are called
       }
   
       {
           // create and open an archive for input
           std::ifstream ifs("filename");
           boost::archive::binary_iarchive ia(ifs);
           // read class state from archive
           ia >> _decompress;
           // archive and stream closed when destructors are called
           dvalues = _decompress.decompress();
       }


       for (uint64_t d : dvalues) {
           std::cout << d << ";";
       }
       std::cout << std::endl;
   
       assert(values==dvalues);
       TEST_ASSERT_EQUALS(_compress,_decompress);
    }

    public: 
      DeltaCompressTest( ) {
         TEST_ADD(DeltaCompressTest::ConstructDeltaCompress);
      } 

}; 
