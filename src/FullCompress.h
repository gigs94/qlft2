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
#include <Zip.h>

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
                     std::vector<int64_t>& time,
                     std::vector<int64_t>& reptime,
                     std::vector<float>& price,
                     std::vector<int64_t>& size) :
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
            std::vector<std::string> stocks = rhs._stocks.decompress();
            std::vector<std::string> exchange = rhs._exchange.decompress();
            std::vector<std::string> side = rhs._side.decompress();
            std::vector<std::string> condition = rhs._condition.decompress();
            std::vector<int64_t> time = rhs._time.decompress();
            std::vector<int64_t> reptime = rhs._reptime.decompress();
            std::vector<float> price = rhs._price.decompress();
            std::vector<int64_t> size = rhs._size.decompress();

            zip (
                [&](std::string i,
                   std::string j,
                   std::string k,
                   std::string l,
                   int64_t m,
                   int64_t n,
                   float o,
                   int p) {
                    os << i << ","
                       << j << ","
                       << k << ","
                       << l << ","
                       << m << ","
                       << n << ","
                       << o << ","
                       << p
                       << std::endl;
                   },
                stocks.begin(),stocks.end(),
                exchange.begin(),
                side.begin(),
                condition.begin(),
                time.begin(),
                reptime.begin(),
                price.begin(),
                size.begin());

            return os;
        };

        friend bool operator==(const FullCompress& lhs, const FullCompress& rhs) {
            if (lhs._stocks == rhs._stocks) { 
                if (lhs._exchange == rhs._exchange) { 
                    if (lhs._side == rhs._side) {
                        if (lhs._condition == rhs._condition) {
                            if (lhs._time == rhs._time) {
                                if (lhs._reptime == rhs._reptime) {
                                    if (lhs._price == rhs._price) {
                                        if (lhs._size == rhs._size) {

                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
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
