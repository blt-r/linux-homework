#include <semaphore.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "shared_array.h"

int main() {
    SharedArray sharray("semaphore-and-char", sizeof(sem_t) + sizeof(char));

    *((char*)sharray.ptr + sizeof(sem_t)) = '!';

    sem_post((sem_t*)sharray.ptr);
}
