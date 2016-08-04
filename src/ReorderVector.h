
/**
 * this was a thought where we could order a column so that we can compress it better.
 * you would need to store the order vector and the data vector in the output so
 * you will have to get a lot of compression out of this to make it worthwhile
 */

#include <vector>
#include <numeric>

template <typename T>
std::vector<int64_t> sort_indexes(const std::vector<T> &v) {

  // initialize original index locations
  std::vector<int64_t> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  // sort indexes based on comparing values in v
  sort(idx.begin(), idx.end(),
       [&v](int64_t i1, int64_t i2) {return v[i1] < v[i2];});

  return idx;
}

template <typename T>
void reorder(const std::vector<T> values) {
   for ( auto value : sort_indexes(values) ) {
       
   }
}
