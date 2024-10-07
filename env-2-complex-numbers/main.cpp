#include "Complex.h"
#include "SortComplex.h"
#include <iostream>
#include <array>


int main() {
  std::array<Complex, 6> z2 = {1, 7*I, -3, 5*I, 9, -6*I};

  sort_by_norm(z2.data(), z2.size());

  for (auto& z : z2) {
    std::cout << z << ' ';
  }
  std::cout << '\n';
}
