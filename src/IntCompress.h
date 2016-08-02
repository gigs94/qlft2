#ifndef __TIME_COMPRESS_H__
#define __TIME_COMPRESS_H__

#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <cstdint>

class IntCompress {
    /**
     *  IntCompress is a class that takes a vector and creates a list of deltas based on the minimal value (the seed).
     *  This can be streamed to a file in binary format for compression sake.
     */
    public:

        IntCompress() : _seed{LONG_MAX} {};
        IntCompress(std::vector<uint64_t> values) : _seed{LONG_MAX} { compress(values); };
        IntCompress(const IntCompress& tc) {};
        virtual ~IntCompress() {};

        void compress(std::vector<uint64_t>& values) {
           // Spin through the values to determine the _seed value
           determineSeedValues(values);

           // Calculate the differences between _seed value and each value and the max delta value
           createDeltaValues(values);

           // Determine the smallest type we can use to compress this data
           determineMaxDeltaType();
        };

        friend std::ostream& operator<<( std::ostream& os, const IntCompress& tc ) {
            // Write header
            os << tc._seed;
            os << tc._maxDeltaType;
            os << tc._deltas.size();

std::cout << "seed = " << tc._seed << std::endl;
std::cout << "type = " << tc._maxDeltaType << std::endl;
std::cout << "size = " << tc._deltas.size() << std::endl;

            // TODO: could look at the number of same sequencial values and store X values of Y.  This
            // TODO: should probably self-optimize considering we don't know what the data looks like.

            // write delta vector
            for( uint64_t d : tc._deltas ) {
                switch (tc._maxDeltaType) {
                    case eight:
                        os << (uint8_t)d;
                    case sixteen:
                        os << (uint16_t)d;
                    case thirtytwo:
                        os << (uint32_t)d;
                    case sixtyfour:
                        os << (uint64_t)d;
                }
            }
   
            return os;
        }

        friend std::istream& operator>>( std::istream& os, IntCompress& tc ) {
            // Read header
            std::uint64_t seed;
            std::uint32_t size;
            std::uint32_t type;

            os.read(reinterpret_cast<char*>(&seed), sizeof seed);
            os.read(reinterpret_cast<char*>(&type), sizeof type);
            os.read(reinterpret_cast<char*>(&size), sizeof size);

            tc._seed = seed;
            tc._maxDeltaType = (DeltaType)type;

std::cout << "seed = " << tc._seed << std::endl;
std::cout << "type = " << tc._maxDeltaType << std::endl;
std::cout << "size = " << size << std::endl;

            // TODO: could look at the number of same sequencial values and store X values of Y.  This
            // TODO: should probably self-optimize considering we don't know what the data looks like.

            // write delta vector
            for( int i = 0; i < size ; i++ ) {
                switch (tc._maxDeltaType) {
                    case eight:
                        std::uint8_t d1;
                        os.read(reinterpret_cast<char*>(&d1), sizeof d1);
                        tc._deltas.push_back(d1);
                    case sixteen:
                        uint16_t d2;
                        os.read(reinterpret_cast<char*>(&d2), sizeof d2);
                        tc._deltas.push_back(d2);
                    case thirtytwo:
                        uint32_t d3;
                        os.read(reinterpret_cast<char*>(&d3), sizeof d3);
                        tc._deltas.push_back(d3);
                    case sixtyfour:
                        uint64_t d4;
                        os.read(reinterpret_cast<char*>(&d4), sizeof d4);
                        tc._deltas.push_back(d4);
                }
            }
std::cout << "created = " << tc._deltas.size() << std::endl;
   
            return os;
        }

        friend bool operator==(const IntCompress& lhs, const IntCompress& rhs) {
            if (lhs._seed == rhs._seed) {
               if (lhs._maxDeltaType == rhs._maxDeltaType) {
                   if (lhs._deltas == rhs._deltas) {
                       return true;
                   }
                }
            }
            return false;
        }

    private:

        uint64_t _seed;
        std::vector<uint64_t> _deltas;
        uint64_t _maxDelta;
        enum DeltaType { eight, sixteen, thirtytwo, sixtyfour } _maxDeltaType;


    protected:

        void determineSeedValues(std::vector<uint64_t> values) {
            for ( uint64_t l : values ) {
std::cout << l << std::endl;
               if ( l < _seed ) { _seed = l; }
            }
std::cout << _seed << std::endl;
        }

        void createDeltaValues(std::vector<uint64_t> values) {
            for ( uint64_t l : values ) {
               _deltas.push_back(l-_seed);

               // Determine the max size of the delta values for storage compression
               max(l);
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

#endif // __TIME_COMPRESS_H__
