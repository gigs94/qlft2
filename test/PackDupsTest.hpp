#include <cpptest.h>
#include <PackDups.h>
#include <vector>

class PackDupsTest : public Test::Suite { 

    void TestPackDups() {
       std::vector<int64_t> values{6311300770,6311300770,6311300770,6311301558,6311301558,6311301558,6311304804,6311400731,6311400731,6311401507,6311401507,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373,6311408373};

       auto value = packDups(values);

       auto dvalues = unPackDups(value);

       TEST_ASSERT(values==dvalues);
    }

    public: 
      PackDupsTest( ) {
         TEST_ADD(PackDupsTest::TestPackDups);
      } 

}; 
