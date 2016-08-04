#ifndef __DELTA_BLOCK_H__
#define __DELTA_BLOCK_H__

#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <cstdint>
#include <DeltaCompress.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>


class DeltaBlock {
    /**
     *  DeltaBlock is a class that takes a vector and creates a list of deltas based on the minimal value (the seed).
     *  This can be streamed to a file in binary format for compression sake.
     */
    public:

        DeltaBlock() {};
        DeltaBlock(std::vector<int64_t> values, int64_t blockSize) : _blockSize{blockSize} { compress(values); };
        DeltaBlock(const DeltaBlock& tc) {};
        virtual ~DeltaBlock() {};

        std::vector<int64_t> decompress() const {
            std::vector<int64_t> rtn;
            bool first{true};
            int64_t prev{0};

            for ( auto block : _blocks ) {
                auto a = block.decompress();
                rtn.insert(std::end(rtn), std::begin(a), std::end(a));
            }

            return std::move(rtn);
        }

        void compress(std::vector<int64_t>& values) {
           createBlocks(values);
        };

        friend std::ostream& operator<<( std::ostream& os, const DeltaBlock& rhs ) {
            for ( auto block : rhs._blocks ) {
                os << block;
            }
            return os;
        };

        friend bool operator==(const DeltaBlock& lhs, const DeltaBlock& rhs) {
            if (lhs._blockSize == rhs._blockSize) {
               if (lhs._blocks == rhs._blocks) {
                   return true;
               }
            }

            return false;
        };

    private:

        int64_t _blockSize;
        std::vector<DeltaCompress> _blocks;

        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & _blockSize;
            ar & _blocks;
        }


    protected:

        void createBlocks(std::vector<int64_t>& values) {
            int totalSize = values.size();
            int processedSize{0};

            if (totalSize > _blockSize) {
               while (processedSize < totalSize) {
                   int endPosition = processedSize + _blockSize;
                   if (endPosition >= totalSize) {
                       endPosition = totalSize;
                   }

                   std::vector<int64_t> newBlock(values.begin() + processedSize, values.begin() + endPosition);
                   DeltaCompress newDelta{newBlock};
                  
                   _blocks.push_back(newDelta);
                   processedSize = endPosition;
               }
               
            } else {
               _blocks.push_back(DeltaCompress{values});
            }
        }
};

#endif // __DELTA_BLOCK_H__
