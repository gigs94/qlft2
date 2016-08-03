#ifndef __PACK_DUPS_H_
#define __PACK_DUPS_H_

template<class T>
std::vector<std::pair<int,T>> packDups(std::vector<T> values) {
    T previous;
    int dupCount{1};
    bool first{true};
    std::vector<std::pair<int,T>> rtn;

    for (auto item : values) {
        if (first) {
            first=false;
        } else {
            if (item==previous) {
                 dupCount++;
            } else {
                 rtn.push_back(std::pair<int,T>{dupCount,previous});
                 dupCount=1;
            }
        }
        previous=item;
    }

    rtn.push_back(std::pair<int,T>{dupCount,previous});

    return std::move(rtn);
}


template<class T>
std::vector<T> unPackDups(std::vector<std::pair<int,T>> values) {
    std::vector<T> rtn;

    for (auto item : values) {
        for (int i = 0; i < item.first; i++) {
             rtn.push_back(item.second);
        }
    }

    return std::move(rtn);
}

#endif // __PACK_DUPS_H_
