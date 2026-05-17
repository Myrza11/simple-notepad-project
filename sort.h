#ifndef SORT_H
#define SORT_H

#include <algorithm>
#include <vector>

template <typename T, typename Compare>
void insertion_sort(std::vector<T>& vec, Compare comp)
{
    for (std::size_t i = 1; i < vec.size(); ++i) {
        T key = vec[i];
        std::size_t j = i;

        while (j > 0 && comp(key, vec[j - 1])) {
            vec[j] = vec[j - 1];
            --j;
        }

        vec[j] = key;
    }
}

template <typename T>
void insertion_sort(std::vector<T>& vec)
{
    insertion_sort(vec, std::less<T>());
}

#endif // SORT_H
