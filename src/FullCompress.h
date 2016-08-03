#ifndef __FULL_COMPRESS_H__
#define __FULL_COMPRESS_H__

#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <cstdint>
#include <StringCompress.h>
#include <DeltaCompress.h>
#include <FloatCompress.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

class FullCompress {
    /**
     *  FullCompress is a class that takes a vector and creates a list of deltas based on the minimal value (the seed).
     *  This can be streamed to a file in binary format for compression sake.
     */
    public:

        FullCompress(std::vector<std::string>& stocks,
                     std::vector<std::string>& exchange,
                     std::vector<std::string>& side,
                     std::vector<std::string>& condition,
                     std::vector<uint64_t>& time,
                     std::vector<uint64_t>& reptime,
                     std::vector<float>& price,
                     std::vector<uint64_t>& size) :
            _stocks{stocks},
            _exchange{exchange},
            _side{side},
            _condition{condition},
            _time{time},
            _reptime{reptime},
            _price{price},
            _size{size} {};
        FullCompress(const FullCompress& rhs) {};
        FullCompress() {};
        virtual ~FullCompress() {};

        friend std::ostream& operator<<( std::ostream& os, const FullCompress& rhs ) {
            os << rhs._stocks
               << rhs._exchange 
               << rhs._side 
               << rhs._condition 
               << rhs._time 
               << rhs._reptime 
               << rhs._price 
               << rhs._size;

            return os;
        };

        friend bool operator==(const FullCompress& lhs, const FullCompress& rhs) {
            if (lhs._stocks == rhs._stocks &&
                lhs._exchange == rhs._exchange &&
                lhs._side == rhs._side &&
                lhs._condition == rhs._condition &&
                lhs._time == rhs._time &&
                lhs._reptime == rhs._reptime &&
                lhs._price == rhs._price &&
                lhs._size == rhs._size) {

                return true;
            }

            return false;
        };

    private:
        StringCompress _stocks;
        StringCompress _exchange;
        StringCompress _side;
        StringCompress _condition;
        DeltaCompress _time;
        DeltaCompress _reptime;
        FloatCompress _price;
        DeltaCompress _size;

        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & _stocks;
            ar & _exchange;
            ar & _side;
            ar & _condition;
            ar & _time;
            ar & _reptime;
            ar & _price;
            ar & _size;
        }

};

#endif // __FULL_COMPRESS_H__
