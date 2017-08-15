/*************************************************************************
	> File Name: gcc-test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月03日 星期四 22时22分49秒
 ************************************************************************/

#include<stdio.h>
#include "p_mmap.h"

int main() {

    int ret = p_init(64 * 1024);
    if (ret < 0) {
        printf("init error!\n");
        return -1;
    }

    int *p = (int *)p_malloc(sizeof(int));
    printf("The address allocated for object p is %p.\n", p);
    
}
