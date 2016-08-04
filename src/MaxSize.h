
#ifndef __MAX_SIZE_H_
#define __MAX_SIZE_H_

enum MaxType { eight, sixteen, thirtytwo, sixtyfour };

class MaxSize {

    public:

        MaxSize() : _max{0}, _maxType{eight} {};

        uint64_t max() {
            return _max;
        }

        uint64_t max(int64_t value) {
           if (std::abs(value) > _max) { 
               _max = std::abs(value);
           }
           return max();
        }

        uint64_t max(std::vector<int64_t> values) {
           for ( auto value : values ) {
               max(value);
           }
           return max();
        }

        void determineMaxType() {
           if (_max <= INT64_MAX) _maxType = sixtyfour;
           if (_max <= INT32_MAX) _maxType = thirtytwo;
           if (_max <= INT16_MAX) _maxType = sixteen;
           if (_max <= INT8_MAX) _maxType = eight;
        }

        MaxType maxType() {
            determineMaxType();
            return _maxType;
        }

     private:
        uint64_t _max;
        MaxType _maxType;

};

#endif
