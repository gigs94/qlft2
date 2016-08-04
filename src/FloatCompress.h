#ifndef __FLOAT_COMPRESS_H__
#define __FLOAT_COMPRESS_H__

#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <cstdint>
#include <regex>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/algorithm/string.hpp>


std::vector<std::string> split(const std::string& input, const std::string& regex) {
    std::regex re(regex);
    std::sregex_token_iterator
        first{input.begin(), input.end(), re, -1},
        last;
    return {first, last};
}

int findSigDigits(const std::vector<std::string>& values) {
    int sigDigs{0};
    for ( auto value : values ) { 
        auto a = split(value, "\\.");
        assert(a.size() <= 2);
        bool first{true};
        for ( auto x : a ) {
            if (first) {
                first=false;
            } else {
                if (x.size() > sigDigs) { sigDigs = x.size(); }
            }
            
        }
    }
    return sigDigs;
}

std::vector<int64_t> stripOutDecimals(std::vector<std::string>& values, int sigDigits) {
    std::vector<int64_t> rtn;

    std::cout << " >>>> " << std::pow(10,sigDigits) << std::endl;

    for ( auto str : values ) {
        float f = std::stof(str);
        int64_t i = static_cast<int64_t>(f*std::pow(10,sigDigits));
std::cout << str << " : " << f << " : " << f*std::pow(10,sigDigits) << " : " << i << std::endl;
        rtn.push_back(i);
    }

    return std::move(rtn);
}

std::vector<std::string> reconstitute(std::vector<std::int64_t>& values, int sigDigits) {
    std::vector<std::string> rtn;

    for ( auto i : values ) {
        float f = static_cast<float>(i)/std::pow(sigDigits,10);
        rtn.push_back(std::to_string(f));
    }

    return std::move(rtn);
}


class FloatCompress {
    /**
     *  FloatCompress is a class that takes a vector and creates a list of deltas based on the minimal value (the seed).
     *  This can be streamed to a file in binary format for compression sake.
     */
    public:

        FloatCompress() : _sigDigits{0} {};
        FloatCompress(std::vector<std::string> values) : _sigDigits{0} { compress(values); };
        FloatCompress(const FloatCompress& tc) {};
        virtual ~FloatCompress() {};

        std::vector<std::string> decompress() const {
            std::vector<std::string> rtn;

            std::vector<int64_t> dcValues = _dc.decompress();

            return std::move(reconstitute(dcValues, _sigDigits));
        }

        void compress(std::vector<std::string>& values) {

           _sigDigits = findSigDigits(values);
           _dc = std::move(DeltaCompress{stripOutDecimals(values, _sigDigits)});

        };

        friend std::ostream& operator<<( std::ostream& os, const FloatCompress& tc ) {
            return os;
        };

        friend bool operator==(const FloatCompress& lhs, const FloatCompress& rhs) {
            if (lhs._sigDigits == rhs._sigDigits) {
               if (lhs._dc == rhs._dc) {
                  return true;
               }
            }

            return false;
        };

    private:

        int64_t _sigDigits;
        DeltaCompress _dc;

        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & _sigDigits;
            ar & _dc;
        }

};

#endif // __FLOAT_COMPRESS_H__
