#ifndef __STRING_COMPRESS_H__
#define __STRING_COMPRESS_H__

#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <cstdint>
#include <map>
#include <PackDups.h>
#include <MaxSize.h>
#include <Zip.h>
#include <ResizeIntVector.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

class StringCompress {
    /**
     *  StringCompress is a class that takes a vector and creates a list of values based on the minimal value (the seed).
     *  This can be streamed to a file in binary format for compression sake.
     */
    public:

        StringCompress() {};
        StringCompress(const std::vector<std::string>& values) { compress(values); };
        StringCompress(const StringCompress& tc) {};
        virtual ~StringCompress() {};

        std::vector<std::string> decompress() const {
            std::vector<std::string> rtn;

            for ( int p : _values ) {
                // find the value for p
                rtn.push_back(_dict.at(p));
            }

            return std::move(rtn);
        }

        void compress(const std::vector<std::string>& values) {
            std::vector<std::string> v{values};
            std::sort(v.begin(), v.end());
            auto last = std::unique(v.begin(), v.end());
            v.erase(last, v.end()); 

            std::map<std::string,int> reverse_dict{};
            for (int i=0; i<v.size(); i++) {
                _dict[i]=v[i];
                reverse_dict[v[i]]=i;
            }

            for (std::string s : values) {
                _values.push_back(reverse_dict[s]);
            }
        };

        friend std::ostream& operator<<( std::ostream& os, const StringCompress& tc ) {
            os << "dictionary:\n";
            for ( auto&& x : tc._dict ) {
                os << "   " << x.first << ":" << x.second << "\n";
            }

            os << "values:\n";
            for ( auto&& y : tc._values ) {
                os << "   " << y << "\n";
            }

            return os;
        };

        friend bool operator!=(const StringCompress& lhs, const StringCompress& rhs) {
            return !(lhs==rhs);
        }

        friend bool operator==(const StringCompress& lhs, const StringCompress& rhs) {
            if (lhs._dict == rhs._dict) {
                if (lhs._values == rhs._values) {
                    return true;
                }
            }
            return false;
        };


    private:

        std::map<int,std::string> _dict;
        std::vector<int64_t> _values;
        
        friend class boost::serialization::access;
        BOOST_SERIALIZATION_SPLIT_MEMBER();

        template<class Archive>
        void save(Archive & ar, const unsigned int version) const {
            ar << _dict;
            MaxSize max;
            max.max(_values);

            ar << max.maxType();

            switch(max.maxType()) {
                case eight:
                    {
                        std::vector<int8_t> new_values;
                        resizeIntVector(_values, new_values);
                        auto packed_values = packDups(new_values);
                        ar << packed_values;
                    }
                    break;
                case  sixteen:
                    {
                        std::vector<int16_t> new_values;
                        resizeIntVector(_values, new_values);
                        auto packed_values = packDups(new_values);
                        ar << packed_values;
                    }
                    break;
                case  thirtytwo:
                    {
                        std::vector<int32_t> new_values;
                        resizeIntVector(_values, new_values);
                        auto packed_values = packDups(new_values);
                        ar << packed_values;
                    }
                    break;
                case  sixtyfour:
                    {
                        std::vector<int64_t> new_values;
                        resizeIntVector(_values, new_values);
                        auto packed_values = packDups(new_values);
                        ar << packed_values;
                    }
                    break;
            }
            
        }

        template<class Archive>
        void load(Archive & ar, const unsigned int version) {
            ar >> _dict;
 
            MaxType maxType;
            ar >> maxType;

            // NOTE:  there has GOT to be a cleaner way to do this now
            switch(maxType) {
                case eight:
                    {
                        std::vector<std::pair<uint8_t,int8_t>> packed_values;
                        ar >> packed_values;
                        auto unpacked_values = unPackDups(packed_values);
                        resizeIntVector(unpacked_values, _values);
                    }
                    break;
                case  sixteen:
                    {
                        std::vector<std::pair<uint8_t,int16_t>> packed_values;
                        ar >> packed_values;
                        auto unpacked_values = unPackDups(packed_values);
                        resizeIntVector(unpacked_values, _values);
                    }
                    break;
                case  thirtytwo:
                    {
                        std::vector<std::pair<uint8_t,int32_t>> packed_values;
                        ar >> packed_values;
                        auto unpacked_values = unPackDups(packed_values);
                        resizeIntVector(unpacked_values, _values);
                    }
                    break;
                case  sixtyfour:
                    {
                        std::vector<std::pair<uint8_t,int64_t>> packed_values;
                        ar >> packed_values;
                        auto unpacked_values = unPackDups(packed_values);
                        resizeIntVector(unpacked_values, _values);
                    }
                    break;
            }
        }

};

#endif // __STRING_COMPRESS_H__
