#include <semaphore.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "shared_array.h"

int main() {
    SharedArray sharray("semaphore-and-char", sizeof(sem_t) + sizeof(char));

    sem_init((sem_t*)sharray.ptr, 1, 0);

    printf("waitintg on semaphre...\n");

    sem_wait((sem_t*)sharray.ptr);

    printf("recieved char %c\n", *((char*)sharray.ptr + sizeof(sem_t)));
}
