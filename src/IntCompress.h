#ifndef __INT_COMPRESS_H__
#define __INT_COMPRESS_H__

#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <cstdint>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

class IntCompress {
    /**
     *  IntCompress is a class that takes a vector and creates a list of deltas based on the minimal value (the seed).
     *  This can be streamed to a file in binary format for compression sake.
     */
    public:

        IntCompress() : _seed{LONG_MAX}, _maxDelta{0} {};
        IntCompress(std::vector<int64_t> values) : _seed{LONG_MAX}, _maxDelta{0} { compress(values); };
        IntCompress(const IntCompress& tc) {};
        virtual ~IntCompress() {};

        std::vector<int64_t> decompress() {
            std::vector<int64_t> rtn;
            
            for (int64_t d : _deltas) {
                rtn.push_back(d+_seed);
            }

            return std::move(rtn);
        }

        void compress(std::vector<int64_t>& values) {
           // Spin through the values to determine the _seed value
           determineSeedValues(values);

           // Calculate the differences between _seed value and each value and the max delta value
           createDeltaValues(values);

           // Determine the smallest type we can use to compress this data
           determineMaxDeltaType();
        };

        friend std::ostream& operator<<( std::ostream& os, const IntCompress& tc ) {
            // Write header
            os << tc._seed << ":";
            os << tc._maxDelta << ":";
            os << tc._maxDeltaType << ":";
            os << tc._deltas.size() << ":";

            // TODO: could look at the number of same sequencial values and store X values of Y.  This
            // TODO: should probably self-optimize considering we don't know what the data looks like.

            // write delta vector
            for( int64_t d : tc._deltas ) {
                os << d << ";";
            }

            return os;
        };

        friend bool operator==(const IntCompress& lhs, const IntCompress& rhs) {
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

        int64_t _seed;
        std::vector<int64_t> _deltas;
        int64_t _maxDelta;
        enum DeltaType { eight, sixteen, thirtytwo, sixtyfour } _maxDeltaType;

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & _seed;
            ar & _maxDeltaType;
            ar & _deltas;
        }

    protected:

        void determineSeedValues(std::vector<int64_t> values) {
            for ( int64_t l : values ) {
               if ( l < _seed ) { _seed = l; }
            }
        }

        void createDeltaValues(std::vector<int64_t> values) {
            for ( int64_t l : values ) {
               int64_t delta = l-_seed;
               _deltas.push_back(delta);

               // Determine the max size of the delta values for storage compression
               max(delta);
            }
        }

        void max(int64_t value) {
           if (value > _maxDelta) { _maxDelta = value; }
        }

        void determineMaxDeltaType() {
           if (_maxDelta <= UINT64_MAX) _maxDeltaType = sixtyfour;
           if (_maxDelta <= UINT32_MAX) _maxDeltaType = thirtytwo;
           if (_maxDelta <= UINT16_MAX) _maxDeltaType = sixteen;
           if (_maxDelta <= UINT8_MAX) _maxDeltaType = eight;
        }
};

#endif // __INT_COMPRESS_H__
