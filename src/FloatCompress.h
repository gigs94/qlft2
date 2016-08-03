#ifndef __FLOAT_COMPRESS_H__
#define __FLOAT_COMPRESS_H__

#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <cstdint>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

class FloatCompress {
    /**
     *  FloatCompress is a class that takes a vector and creates a list of deltas based on the minimal value (the seed).
     *  This can be streamed to a file in binary format for compression sake.
     */
    public:

        FloatCompress() : _seed{LONG_MAX}, _maxFloat{0} {};
        FloatCompress(std::vector<float> values) : _seed{LONG_MAX}, _maxFloat{0} { compress(values); };
        FloatCompress(const FloatCompress& tc) {};
        virtual ~FloatCompress() {};

        std::vector<float> decompress() {
            std::vector<float> rtn;
            bool first{true};
            float prev{0};

            for (float d : _deltas) {
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

        void compress(std::vector<float>& values) {
           // Spin through the values to determine the _seed value
           determineSeedValues(values);

           // Calculate the differences between _seed value and each value and the max delta value
           createFloatValues(values);

           // Determine the smallest type we can use to compress this data
           determineMaxFloatType();
        };

        friend std::ostream& operator<<( std::ostream& os, const FloatCompress& tc ) {
            // Write header
            os << tc._seed << ":";
            os << tc._maxFloat << ":";
            os << tc._maxFloatType << ":";
            os << tc._deltas.size() << ":";

            // TODO: could look at the number of same sequencial values and store X values of Y.  This
            // TODO: should probably self-optimize considering we don't know what the data looks like.

            // write delta vector
            for( float d : tc._deltas ) {
                os << d << ";";
            }

            return os;
        };

        friend bool operator==(const FloatCompress& lhs, const FloatCompress& rhs) {
            if (lhs._seed == rhs._seed) {
               if (lhs._maxFloatType == rhs._maxFloatType) {
                   if (lhs._deltas == rhs._deltas) {
                       return true;
                   }
                }
            }

            return false;
        };

    private:

        float _seed;
        std::vector<float> _deltas;
        float _maxFloat;
        enum FloatType { eight, sixteen, thirtytwo, sixtyfour } _maxFloatType;

        friend class boost::serialization::access;
        // When the class Archive corresponds to an output archive, the
        // & operator is defined similar to <<.  Likewise, when the class Archive
        // is a type of input archive the & operator is defined similar to >>.
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & _seed;
            ar & _maxFloatType;
            ar & _deltas;
        }

    protected:

        // Seed value is always the first value in this case
        void determineSeedValues(std::vector<float> values) {
            for ( float l : values ) {
               _seed = l;
               break;
            }
        }

        void createFloatValues(std::vector<float> values) {
            bool first=true;
            float prev{0};
            float delta{0};

            for ( float l : values ) {
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

        void max(float value) {
           if (value > _maxFloat) { _maxFloat = value; }
        }

        void determineMaxFloatType() {
           if (_maxFloat <= UINT64_MAX) _maxFloatType = sixtyfour;
           if (_maxFloat <= UINT32_MAX) _maxFloatType = thirtytwo;
           if (_maxFloat <= UINT16_MAX) _maxFloatType = sixteen;
           if (_maxFloat <= UINT8_MAX) _maxFloatType = eight;
        }
};

#endif // __FLOAT_COMPRESS_H__
