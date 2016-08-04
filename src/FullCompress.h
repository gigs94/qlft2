#ifndef __FULL_COMPRESS_H__
#define __FULL_COMPRESS_H__

#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <cstdint>
#include <StringCompress.h>
#include <DeltaBlock.h>
//#include <FloatCompress.h>
#include <Zip.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

class FullCompress {
    /**
     *  FullCompress is a class that takes a vector and creates a list of deltas based on the minimal value (the seed).
     *  This can be streamed to a file in binary format for compression sake.
     *
     *  FullCompress contains the columns for the ebat file format.   Here you can change the type of the column and
     *  for different modules that are defined for compression.  Currently, only DeltaCompress and StringCompress are
     *  fully developed and used.  To make a change, you need to change the FullCompress ctor vector type for the new
     *  compression type and the type of the member.   Fairly simple to play with the compression types for different
     *  columns for customized performance purposes. 
     *
     */
    public:

        FullCompress(const std::vector<std::string>& stocks,
                     const std::vector<std::string>& exchange,
                     const std::vector<std::string>& side,
                     const std::vector<std::string>& condition,
                     const std::vector<int64_t>& time,
                     const std::vector<int64_t>& reptime,
                     const std::vector<std::string>& price,
                     const std::vector<int64_t>& size,
                     int blockSize) :
            _stocks{stocks},
            _exchange{exchange},
            _side{side},
            _condition{condition},
            _time{time, blockSize},
            _reptime{reptime, blockSize},
            _price{price},
            // blocksize for size is hardcoded right now because it's values aren't as disperse as the time values
            _size{size, 50} {};
        FullCompress(const FullCompress& rhs) {};
        FullCompress() {};
        virtual ~FullCompress() {};

        friend std::ostream& operator<<( std::ostream& os, const FullCompress& rhs ) {
            auto stocks = rhs._stocks.decompress();
            auto exchange = rhs._exchange.decompress();
            auto side = rhs._side.decompress();
            auto condition = rhs._condition.decompress();
            auto time = rhs._time.decompress();
            auto reptime = rhs._reptime.decompress();
            auto price = rhs._price.decompress();
            auto size = rhs._size.decompress();

            zip (
                [&](std::string i,
                   std::string j,
                   std::string k,
                   std::string l,
                   int64_t m,
                   int64_t n,
                   std::string o,
                   int p) {
                    os << i << ","
                       << j << ","
                       << k << ","
                       << l << ","
                       << m << ","
                       << n << ","
                       << o << ","
                       << p << "\r"
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
        DeltaBlock _time;
        DeltaBlock _reptime;

        // Using StringCompress here because convertion to float and back causes precision loss.
        // There is a FloatCompress module, that gets 'close' but that's not good enough.
        StringCompress _price;
        DeltaBlock _size;

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
