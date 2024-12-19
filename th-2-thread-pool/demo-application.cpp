#include <iostream>

#include <stdatomic.h>

#include "parallel-scehduler.h"

int main() {
  atomic_int local = 0;

  ThreadPool pool(16);

  for (int i = 0; i < 10000; ++i) {
    pool.add_task(
        [](void *void_args) {
          atomic_int *data = (atomic_int *)void_args;
          *data += 1;
        },
        &local);
  }

  pool.wait_all();

  std::cout << "Computed value: " << local << "\n";
}
