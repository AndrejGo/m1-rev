#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <pthread.h>
#include <unistd.h>
#include "../../loop_predictor/m1.h"
#include <errno.h>
#include <vm/pmap.h>

#define PAGE_SIZE (1 << 14)
#define CACHE_LINE_SIZE 128
#define CACHE_LINES_PER_PAGE (PAGE_SIZE / CACHE_LINE_SIZE)

int main() {

    // Give the OS a hint to schedule the thread executing main()
    // as a background process -> on an Icestorm core.
#ifdef FIRESTORM
    pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);
#else
    pthread_set_qos_class_self_np(QOS_CLASS_BACKGROUND, 0);
#endif

    // Sleeping causes the OS to unschedule this thread and then
    // bring it back, hopefully considering the hint.
    usleep(200);

    // Grant user space applications access to PMCs
    set_sysctl("kern.pmcr0", PMCR0_DEFAULT_XCODE);

    // Profile the core this thread is executing on.
    unsigned long clocks = num_cycles_to_do_FP_loop(1000);
    //printf("Clocks: %lu\n", clocks);
    int core = clocks < CYCLE_LIMIT_FOR_EFFICIENT_CORE ? 0 : 1;
    // 0 -> Firestorm
    // 1 -> Icestorm
    if(core != 1) {
        printf("Not on Icestorm\n");
        return 0;
    }

    // Configure the counter to trach branch mispredictions
    // Enable PMC5
    uint64_t pmcr1 = SREG_READ(PMCR1);
    pmcr1 |= EL0A64_ENABLE(5);
    SREG_WRITE(PMCR1, pmcr1);

    // Set PMC5 to count branch mispredictions
    uint64_t pmesr0 = SREG_READ(PMESR0);
    pmesr0 = PMESR0_SET_EVENT(pmesr0, 5, BRANCH_INDIR_MISPRED_NONSPEC);
    SREG_WRITE(PMESR0, pmesr0);

    // Get a large amount of working space
    unsigned long size = ((unsigned long)(1) << 32);
    volatile char* working_space = malloc(size);

    if(working_space == NULL) {
        printf("malloc failed\n");
        return errno;
    }

    *working_space = 'a';

    uint64_t clocks_before = SREG_READ(PMC0);
    volatile char t = *working_space;
    uint64_t clocks_after = SREG_READ(PMC0);

    printf("Access took %llu clocks\n", clocks_after - clocks_before);

    clocks_before = SREG_READ(PMC0);
    t = *working_space;
    clocks_after = SREG_READ(PMC0);

    printf("Access took %llu clocks\n", clocks_after - clocks_before);

    //unsigned long num_pages = size / PAGE_SIZE;
    //unsigned long num_accesses = (num_pages-1) * CACHE_LINES_PER_PAGE;
    for(unsigned long i = 1; i < 1024; i++) {
        t ^= *(working_space + i * PAGE_SIZE);
    }

    //set_sysctl("kern.pmcr0", PMCR0_DEFAULT_XCODE);
    clocks_before = SREG_READ(PMC0);
    t = *working_space;
    clocks_after = SREG_READ(PMC0);

    printf("Access took %llu clocks\n", clocks_after - clocks_before);

    //set_sysctl("kern.pmcr0", PMCR0_DEFAULT_XCODE);
    clocks_before = SREG_READ(PMC0);
    t = *working_space;
    clocks_after = SREG_READ(PMC0);

    printf("Access took %llu clocks\n", clocks_after - clocks_before);

}