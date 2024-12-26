#include <utility>

#include "SortComplex.h"

void sort_by_norm(Complex nums[], std::size_t size) {
    // best sorting algorithm known to man

    for (std::size_t i = 0; i < size; i++) {
        bool is_sorted = true;
        for (std::size_t j = 0; j < size - 1 - i; j++) {
            if (nums[j].norm_sq() > nums[j + 1].norm_sq()) {
                std::swap(nums[j], nums[j + 1]);
            }
            is_sorted = false;
        }
        if (is_sorted) {
            break;
        }
    }
}
