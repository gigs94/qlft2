#ifndef __DELTA_COMPRESS_H__
#define __DELTA_COMPRESS_H__

#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <cstdint>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

class DeltaCompress {
    /**
     *  DeltaCompress is a class that takes a vector and creates a list of deltas based on the minimal value (the seed).
     *  This can be streamed to a file in binary format for compression sake.
     */
    public:

        DeltaCompress() : _seed{LONG_MAX}, _maxDelta{0} {};
        DeltaCompress(std::vector<uint64_t> values) : _seed{LONG_MAX}, _maxDelta{0} { compress(values); };
        DeltaCompress(const DeltaCompress& tc) {};
        virtual ~DeltaCompress() {};

        std::vector<uint64_t> decompress() {
            std::vector<uint64_t> rtn;
            bool first{true};
            uint64_t prev{0};

            for (uint64_t d : _deltas) {
                if (first) {
                    prev = d+_seed;
                    first=false;
                } else {
                    prev = d+prev;
                }
                rtn.push_back(prev);
            }

            return std::move(rtn);
        }

        void compress(std::vector<uint64_t>& values) {
           // Spin through the values to determine the _seed value
           determineSeedValues(values);

           // Calculate the differences between _seed value and each value and the max delta value
           createDeltaValues(values);

           // Determine the smallest type we can use to compress this data
           determineMaxDeltaType();
        };

        friend std::ostream& operator<<( std::ostream& os, const DeltaCompress& tc ) {
            // Write header
            os << tc._seed << ":";
            os << tc._maxDelta << ":";
            os << tc._maxDeltaType << ":";
            os << tc._deltas.size() << ":";

            // TODO: could look at the number of same sequencial values and store X values of Y.  This
            // TODO: should probably self-optimize considering we don't know what the data looks like.

            // write delta vector
            for( uint64_t d : tc._deltas ) {
                os << d << ";";
            }

            return os;
        };

        friend bool operator==(const DeltaCompress& lhs, const DeltaCompress& rhs) {
            if (lhs._seed == rhs._seed) {
               if (lhs._maxDeltaType == rhs._maxDeltaType) {
                   if (lhs._deltas == rhs._deltas) {
                       return true;
                   }
                }
            }

            return false;
        };

    private:

        uint64_t _seed;
        std::vector<uint64_t> _deltas;
        uint64_t _maxDelta;
        enum DeltaType { eight, sixteen, thirtytwo, sixtyfour } _maxDeltaType;

        friend class boost::serialization::access;
        // When the class Archive corresponds to an output archive, the
        // & operator is defined similar to <<.  Likewise, when the class Archive
        // is a type of input archive the & operator is defined similar to >>.
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & _seed;
            ar & _maxDeltaType;
            ar & _deltas;
        }

    protected:

        // Seed value is always the first value in this case
        void determineSeedValues(std::vector<uint64_t> values) {
            for ( uint64_t l : values ) {
               _seed = l;
               break;
            }
        }

        void createDeltaValues(std::vector<uint64_t> values) {
            bool first=true;
            uint64_t prev{0};
            uint64_t delta{0};

            for ( uint64_t l : values ) {
               // first delta is always 0
               if (first) {
                    first=false;
               } else {
                    delta = l-prev;
               }

               prev=l;
               _deltas.push_back(delta);

               // Determine the max size of the delta values for storage compression
               max(delta);
            }
        }

        void max(uint64_t value) {
           if (value > _maxDelta) { _maxDelta = value; }
        }

        void determineMaxDeltaType() {
           if (_maxDelta <= UINT64_MAX) _maxDeltaType = sixtyfour;
           if (_maxDelta <= UINT32_MAX) _maxDeltaType = thirtytwo;
           if (_maxDelta <= UINT16_MAX) _maxDeltaType = sixteen;
           if (_maxDelta <= UINT8_MAX) _maxDeltaType = eight;
        }
};

#endif // __DELTA_COMPRESS_H__
