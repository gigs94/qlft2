#ifndef __DELTA_COMPRESS_H__
#define __DELTA_COMPRESS_H__

#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <cstdint>
#include <PackDups.h>
#include <MaxSize.h>
#include <ResizeIntVector.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>



template <typename T>
vector<size_t> sort_indexes(const vector<T> &v) {

  // initialize original index locations
  vector<size_t> idx(v.size());
  iota(idx.begin(), idx.end(), 0);

  // sort indexes based on comparing values in v
  sort(idx.begin(), idx.end(),
       [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});

  return idx;
}

reorder(values) {
   for ( auto value : values ) {
       
   }
}

class DeltaCompress {
    /**
     *  DeltaCompress is a class that takes a vector and creates a list of deltas based on the minimal value (the seed).
     *  This can be streamed to a file in binary format for compression sake.
     */
    public:

        DeltaCompress() : _seed{LONG_MAX} {};
        DeltaCompress(std::vector<int64_t> values) : _seed{LONG_MAX} { compress(values); };
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
        };

        friend std::ostream& operator<<( std::ostream& os, const DeltaCompress& tc ) {
            // Write header
            os << tc._seed << ":";
            os << tc._deltas.size() << ":";

            // write delta vector
            for( int64_t d : tc._deltas ) {
                os << d << ";";
            }

            return os;
        };

        friend bool operator==(const DeltaCompress& lhs, const DeltaCompress& rhs) {
            if (lhs._seed == rhs._seed) {
               if (lhs._deltas == rhs._deltas) {
                   return true;
               }
            }

            return false;
        };

    private:

        int64_t _seed;
        std::vector<int64_t> _deltas;

        friend class boost::serialization::access;
        BOOST_SERIALIZATION_SPLIT_MEMBER();

        template<class Archive>
        void save(Archive & ar, const unsigned int version) const {
            ar << _seed;

            MaxSize max;
            max.max(_deltas);

            ar << max.maxType();

            switch (max.maxType()) {
                case eight:
                    {
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
                        std::vector<int32_t> newdeltas;
                        for ( int32_t i : _deltas ) {
                            newdeltas.push_back(i);
                        }
                        auto packed_newdeltas = packDups(newdeltas);
                        ar << packed_newdeltas;
                    }
                    break;
                case  sixtyfour:
                    auto packed_newdeltas = packDups(_deltas);
                    ar << packed_newdeltas;
                    break;
            }

        }

        template<class Archive>
        void load(Archive & ar, const unsigned int version) {
            ar >> _seed;
            MaxType maxType;

            ar >> maxType;

            switch (maxType) {
                case eight:
                    {
                        std::vector<std::pair<uint8_t,int8_t>> packed_newdeltas;
                        ar >> packed_newdeltas;
                        
                        auto newdeltas = unPackDups(packed_newdeltas);
                        resizeIntVector(newdeltas, _deltas);
                    }
                    break;
                case  sixteen:
                    {
                        std::vector<std::pair<uint8_t,int16_t>> packed_newdeltas;
                        ar >> packed_newdeltas;
                        
                        auto newdeltas = unPackDups(packed_newdeltas);
                        resizeIntVector(newdeltas, _deltas);
                    }
                    break;
                case  thirtytwo:
                    {
                        std::vector<std::pair<uint8_t,int32_t>> packed_newdeltas;
                        ar >> packed_newdeltas;
                        
                        auto newdeltas = unPackDups(packed_newdeltas);
                        resizeIntVector(newdeltas, _deltas);
                    }
                    break;
                case  sixtyfour:
                        std::vector<std::pair<uint8_t,int64_t>> packed_newdeltas;
                        ar >> packed_newdeltas;
                        
                        auto newdeltas = unPackDups(packed_newdeltas);
                        resizeIntVector(newdeltas, _deltas);
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
            }
        }
};

#endif // __DELTA_COMPRESS_H__
