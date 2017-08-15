/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月06日 星期日 12时37分03秒
 ************************************************************************/

#include<stdio.h>
#include "p_mmap.h"

int main() {
    int ret = p_init(64*1024);
    if (ret < 0) {
        printf("Init failed, exit!\n");
        return -1;
    }

    char *pBaseAddr = p_get_base();
    printf("The base address for p_init is %p.\n", pBaseAddr);

    // Access initiated address memory
    pBaseAddr[0] = 'a';
    pBaseAddr[1] = 'b';
    char *pStrAddr = (char *)&pBaseAddr[2];
    pStrAddr = "cdefg"; 
    int *intAddr = (int *)&pBaseAddr[3];
    *intAddr = 10;
    
    printf("Addresses for elements are %p, %p, %p and %p respectively.\n", &pBaseAddr[0], &pBaseAddr[1], pStrAddr, intAddr);

    printf("Each element is %c, %c, %s and %d respectively.\n", pBaseAddr[0], pBaseAddr[1], pStrAddr, *intAddr);

    return 0;
}
