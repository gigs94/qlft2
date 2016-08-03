#ifndef __DELTA_COMPRESS_H__
#define __DELTA_COMPRESS_H__

#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <cstdint>
#include <PackDups.h>

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
        DeltaCompress(std::vector<int64_t> values) : _seed{LONG_MAX}, _maxDelta{0} { compress(values); };
        DeltaCompress(const DeltaCompress& tc) {};
        virtual ~DeltaCompress() {};

        std::vector<int64_t> decompress() const {
            std::vector<int64_t> rtn;
            bool first{true};
            int64_t prev{0};

            for (int64_t d : _deltas) {
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

        void compress(std::vector<int64_t>& values) {
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
            for( int64_t d : tc._deltas ) {
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

        int64_t _seed;
        std::vector<int64_t> _deltas;
        int64_t _maxDelta;
        enum DeltaType { eight, sixteen, thirtytwo, sixtyfour } _maxDeltaType;

        friend class boost::serialization::access;
        BOOST_SERIALIZATION_SPLIT_MEMBER();

        template<class Archive>
        void save(Archive & ar, const unsigned int version) const {
            ar << _seed;
            ar << _maxDeltaType;

            switch (_maxDeltaType) {
                case eight:
                    {
std::cout << "here 8" << std::endl;
                        std::vector<int8_t> newdeltas;
                        for ( int8_t i : _deltas ) {
                            newdeltas.push_back(i);
                        }
                        auto packed_newdeltas = packDups(newdeltas);
                        ar << packed_newdeltas;
                    }
                    break;
                case  sixteen:
                    {
std::cout << "here 16" << std::endl;
                        std::vector<int16_t> newdeltas;
                        for ( int16_t i : _deltas ) {
                            newdeltas.push_back(i);
                        }
                        auto packed_newdeltas = packDups(newdeltas);
                        ar << packed_newdeltas;
                    }
                    break;
                case  thirtytwo:
                    {
std::cout << "here 32" << std::endl;
                        std::vector<int32_t> newdeltas;
                        for ( int32_t i : _deltas ) {
                            newdeltas.push_back(i);
                        }
                        auto packed_newdeltas = packDups(newdeltas);
                        ar << packed_newdeltas;
                    }
                    break;
                case  sixtyfour:
std::cout << "here 64" << std::endl;
                    auto packed_newdeltas = packDups(_deltas);
                    ar << packed_newdeltas;
                    break;
            }

        }

        template<class Archive>
        void load(Archive & ar, const unsigned int version) {
            ar >> _seed;
            ar >> _maxDeltaType;

            switch (_maxDeltaType) {
                case eight:
                    {
std::cout << "here >> 8" << std::endl;
                        std::vector<std::pair<int,int8_t>> packed_newdeltas;
                        ar >> packed_newdeltas;
                        
                        auto newdeltas = unPackDups(packed_newdeltas);
                        for ( int8_t i : newdeltas ) {
                            _deltas.push_back(i);
                        }
                    }
                    break;
                case  sixteen:
                    {
std::cout << "here >> 16" << std::endl;
                        std::vector<std::pair<int,int16_t>> packed_newdeltas;
                        ar >> packed_newdeltas;
                        
                        auto newdeltas = unPackDups(packed_newdeltas);
                        for ( int16_t i : newdeltas ) {
                            _deltas.push_back(i);
                        }
                    }
                    break;
                case  thirtytwo:
                    {
std::cout << "here >> 32" << std::endl;
                        std::vector<std::pair<int,int32_t>> packed_newdeltas;
                        ar >> packed_newdeltas;
                        
                        auto newdeltas = unPackDups(packed_newdeltas);
                        for ( int32_t i : newdeltas ) {
                            _deltas.push_back(i);
                        }
                    }
                    break;
                case  sixtyfour:
std::cout << "here >> 64" << std::endl;
                        std::vector<std::pair<int,int64_t>> packed_newdeltas;
                        ar >> packed_newdeltas;
                        
                        auto newdeltas = unPackDups(packed_newdeltas);
                        for ( int64_t i : newdeltas ) {
                            _deltas.push_back(i);
                        }
                    break;
            }

        }


    protected:

        // Seed value is always the first value in this case
        void determineSeedValues(std::vector<int64_t> values) {
            for ( int64_t l : values ) {
               _seed = l;
               break;
            }
        }

        void createDeltaValues(std::vector<int64_t> values) {
            bool first=true;
            int64_t prev{0};
            int64_t delta{0};

            for ( int64_t l : values ) {
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

#endif // __DELTA_COMPRESS_H__
