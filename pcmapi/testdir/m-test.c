#include<stdio.h>
#include "p_mmap.h"

int main() {
    int ret = p_init(64*1024); // Initiate a 64KB native heap
    if (ret < 0) {
        printf("Initiation failed!\n");
        return -1;
    }

    int *intAddr = (int *)p_malloc(sizeof(int)); // malloc a PM address for INTEGER data

    if (intAddr == NULL) {
        printf("Malloc failed!\n");
        return -1;
    }

    *intAddr = 1;
    printf("Malloc address is %p and its value is %d.\n", intAddr, *intAddr);

    p_bind(1, intAddr, sizeof(int)); // bind intAddr data to native heap with a native ID 1.

    return 0;
}
