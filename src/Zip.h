#ifndef __ZIP_H_
#define __ZIP_H_

template <typename Iterator>
void advance_all (Iterator & iterator) {
    ++iterator;
}

template <typename Iterator, typename ... Iterators>
void advance_all (Iterator & iterator, Iterators& ... iterators) {
    ++iterator;
    advance_all(iterators...);
} 

template <typename Function, typename Iterator, typename ... Iterators>
Function zip (Function func,
              Iterator begin, 
              Iterator end, 
              Iterators ... iterators)
{
    for(;begin != end; ++begin, advance_all(iterators...))
        func(*begin, *(iterators)... );

    return func;
}

#endif // __ZIP_H_
