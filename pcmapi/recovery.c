# include "p_mmap.h"
# include "stdlib.h"
# include "time.h"

/**
 * This program is used to test recovery performance of Daisy
 * Setup:
 * Benchmark:
 * Exp-1:region (100-1000000), size (16,64,256,1024 bytes), 8 threads, no long-term transaction
 * Exp-2:long-term transaction ratio (0.05, 0.1, 0.2, 0.4), region (1000-1000000), size 256 bytes
 */

#define MIN_REGION_NUM 100
#define REGION_FAC 10
#define MAX_REGION_NUM 1000000
#define MIN_SIZE 16
#define SIZE_FAC 4
#define MAX_SIZE 1024
#define MIN_LT_RATIO 0.05
#define LT_RATIO_FAC 2
#define MAX_LT_RATIO 0.4

typedef struct obj_16 {
    unsigned long o1;
    unsigned long o2;
}obj_16;

typedef struct obj_64 {
    unsigned long o1;
    unsigned long o2;
    unsigned long o3;
    unsigned long o4;
    unsigned long o5;
    unsigned long o6;
    unsigned long o7;
    unsigned long o8;
}obj_64;

int main() {
    /* Exp-1 */
    /* Workflow:
     * Step 1: Build data structure with specified size (like 16 bytes)
     * Step 2: Create specified number (100) of persistent regions with calling p_malloc 
     * Step 3: Update 100 data regions with logging enabled
     * Step 4: Emulate random failures with transaction_end not called, notice that within 8 threads, failurs for normal transactions are at most 8, can be 0. We can take the average 4.
     * Step 5: Call recovery function (to be finished), first find inconsistent transactions (overhead fixed), the average is 4 but we can use 2.
     * Step 6: For each inconsistent transaction t, we undo all the operations (to be implemented).
     * Step 7: Compute the recovery time based on 
     */
    unsigned long i;
    unsigned long nid;
    int iRet; // p_recover retun 0 if successful
    double duration;
    clock_t start, end;
    int rand_num; // should be between 0 and 8
    srand((unsigned int)time(NULL));

    int region_num = 10; // 100 for test
    int size = 16; // 16 for simple test

    // allocate 100 persistent memory regions
    // init a 200 MB persistent memory (native heap)
    int heap_size = 200 * 1024 * 1024;
    iRet = p_init(heap_size);
    if (iRet < 0) {
        printf("Error p_init.\n");
        return -1;
    }
    p_clear(); // For initiation safety
    // allocate from the native heap
    printf("The size of obj_16 data structure is %d bytes.\n", (int)sizeof(obj_16));
    
    printf("----------Malloc procedure starts here!-------------\n");
    for (i = 0;i < region_num;i++) {
        nid = i + 1;
        // obj_16 *obj = (obj_16 *)p_malloc(nid, sizeof(obj_16)); // give id
        obj_64 *obj = (obj_64 *)p_malloc(nid, sizeof(obj_64)); // give id
    }
    printf("----------Malloc procedure ends here!-------------\n");

    // choose the number of inconsistent transactions
    rand_num = (int)rand() % 9;
    printf("this time the rand_num is %d\n", rand_num);
    
    // Update the first rand_num regions without calling transaction_end
    printf("----------Update and log procedure starts here!-------------\n");
    for (i = 0; i < region_num;i++) {
        nid = i + 1;
        // obj_16 *obj = (obj_16 *)p_get_malloc(nid);
        obj_64 *obj = (obj_64 *)p_get_malloc(nid);
        log_create(nid);
        transaction_start(nid);
        obj->o1 = i+0;
        transaction_record(nid, &(obj->o1));
        obj->o2 = i+1;
        transaction_record(nid, &(obj->o2));
        obj->o1 = i+0;
        transaction_record(nid, &(obj->o3));
        obj->o2 = i+1;
        transaction_record(nid, &(obj->o4));
        obj->o1 = i+0;
        transaction_record(nid, &(obj->o5));
        obj->o2 = i+1;
        transaction_record(nid, &(obj->o6));
        obj->o1 = i+0;
        transaction_record(nid, &(obj->o7));
        obj->o2 = i+1;
        transaction_record(nid, &(obj->o8));

        if (i >= rand_num) {
            transaction_end(nid);
        }
    }
    printf("----------Update and log procedure ends here!-------------\n");

    // recovery function
    start = clock();
    for (i = 0; i < rand_num;i++) {
        nid = i + 1;
        iRet = p_recover_single(nid);
    }
    end = clock();
    duration = (double)(end-start) / CLOCKS_PER_SEC;
    printf("The recovery delay for %d inconsistent transactions is %f.\n", rand_num, duration);
    
    p_clear(); // For safety purpose

    return 0;
}

