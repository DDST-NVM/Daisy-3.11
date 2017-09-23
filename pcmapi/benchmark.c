#include "test.h"
#include "p_mmap.h"
#include "stdlib.h"
#include "time.h"
#include <stdio.h>

/**
 * This program is written for real-time workload running with NVM usage monitoring
 * The benchmark flow is as follows:
 * 1. Set three random allocation size with ID: (~10M,1), (~20M,2), (~50M,3)
 * 2. Set a random flag (0 or 1) to decide if a write-to-file flush need to be done after each allocation
 * 3. free the allocated NVM region in the sequence of (1, 3, 2)
 * 4. Set a random flag (0 or 1) to decide if a write-to-file flush need to  be done after each allocation
*/

void file_record(FILE *fp) {
    int total = 200; // in MB format
    int free = get_free_size() / (1024 * 1024);
    int used = total - free; 
    
    fputs("NVM ", fp);
    fputs(used, fp);
    fputs("\n");
}


int main() {
    int iRet = 0;
    int ID_1 = 1;
    int ID_2 = 2;
    int ID_3 = 3;
    int rand_size_1, rand_size_2, rand_size_3; // in MB format
    FILE *fp;
    fp = open("memdata.txt","w+");
    // Specify the size of native heap
    int size = 200 * 1024 * 1024;
    iRet = p_init(size);
    if (iRet) {
        printf("Error in p_init!\n");
        return -1;
    }

    /* Add a while loop here */

    /* Generate random allocation size */
    srand((unsigned int)time(NULL));
    rand_size_1 = rand() % 10 * 1024 * 1024;
    rand_size_2 = rand() % 30 * 1024 * 1024;
    rand_size_3 = rand() % 50;

    int rand_flag;

    // output check
    printf("rand_size_1 is %d.\n", rand_size_1);
    printf("rand_size_2 is %d.\n", rand_size_2);
    printf("rand_size_3 is %d.\n", rand_size_3);

    /* allocation process */
    int *obj_1 = (int *)p_malloc(ID_1, rand_size_1);
    rand_flag = rand() % 2;
    if (rand_flag == 1) {
        file_record(fp);
    } 

    int *obj_2 = (int *)p_malloc(ID_2, rand_size_2);
    rand_flag = rand() % 2;
    if (rand_flag == 1) {
        file_record(fp);
    } 

    int *obj_3 = (int *)p_malloc(ID_3, rand_size_3);
    rand_flag = rand() % 2;
    if (rand_flag == 1) {
        file_record(fp);
    } 

    /* recycling process */

    p_free(ID_3);
    rand_flag = rand() % 2;
    if (rand_flag == 1) {
        file_record(fp);
    } 

    p_free(ID_2);
    rand_flag = rand() % 2;
    if (rand_flag == 1) {
        file_record(fp);
    } 

    p_free(ID_1);
    rand_flag = rand() % 2;
    if (rand_flag == 1) {
        file_record(fp);
    } 

    return 0;
}
