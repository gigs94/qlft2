#ifndef __PACK_DUPS_H_
#define __PACK_DUPS_H_

template<class T>
std::vector<std::pair<uint8_t,T>> packDups(std::vector<T> values) {
    T previous;
    uint8_t dupCount{1};
    bool first{true};
    std::vector<std::pair<uint8_t,T>> rtn;

    for (auto item : values) {
        if (first) {
            first=false;
        } else {
            if (item==previous) {
                 if (dupCount == UINT8_MAX-1) {
                     // Cut off this block and start another one
                     rtn.push_back(std::pair<int8_t,T>{dupCount,previous});
                     dupCount=1;
                 } else {
                     dupCount++;
                 }
            } else {
                 rtn.push_back(std::pair<int8_t,T>{dupCount,previous});
                 dupCount=1;
            }
        }
        previous=item;
    }

    rtn.push_back(std::pair<uint8_t,T>{dupCount,previous});

    return std::move(rtn);
}


template<class T>
std::vector<T> unPackDups(std::vector<std::pair<uint8_t,T>> values) {
    std::vector<T> rtn;

    for (auto item : values) {
        for (uint8_t i = 0; i < item.first; i++) {
             rtn.push_back(item.second);
        }
    }

    return std::move(rtn);
}

#endif // __PACK_DUPS_H_
