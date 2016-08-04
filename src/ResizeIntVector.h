#ifndef __RESIZE_INT_VECTOR_H_
#define __RESIZE_INT_VECTOR_H_

/**
 * resizes a vector... useful for intger conversion
 */
template <class T, class Y>
void resizeIntVector(const std::vector<T>& values, std::vector<Y>& rtn ) {
    for (T value : values ) {
        Y a = value;
        rtn.push_back(a);
    }
}

#endif // __RESIZE_INT_VECTOR_H_
