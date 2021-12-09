#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <pthread.h>
#include "../m1.h"
#include <unistd.h>

#define ICE 0
#define FIRE 1

uint64_t outer_count;
uint64_t inner_count;
unsigned long clocks;
int core;
uint64_t mispredictions;
uint64_t diff;

void* testing_thread(void* arg) {

    // Set the BACKGROUND quality of service class
    pthread_set_qos_class_self_np(QOS_CLASS_BACKGROUND, 0);
    //pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);

    // Go to sleep. We want the OS to "unschedule"
    // the thread and then bring it back, hopefully on the specified core.
    usleep(100);

    set_sysctl("kern.pmcr0", PMCR0_DEFAULT_XCODE);
    clocks = num_cycles_to_do_FP_loop(100);

    if(clocks < 400) {
        core = FIRE;
    } else {
        core = ICE;
    }

    // Enable PMC5
    uint64_t pmcr1 = SREG_READ(PMCR1);
    pmcr1 |= EL0A64_ENABLE(5);
    SREG_WRITE(PMCR1, pmcr1);

    // Set PMC5 to count branch mispredictions
    uint64_t pmesr0 = SREG_READ(PMESR0);
    pmesr0 = PMESR0_SET_EVENT(pmesr0, 5, BRANCH_MISPRED_NONSPEC);
    SREG_WRITE(PMESR0, pmesr0);

    // ====================================================================== //
    // No more branches beyond this point

    uint64_t pmc5_diff = 0;

    __asm__ volatile (
        BRANCH_SANITIZE_1024
        "       isb             \n"
        "       mrs x5, "PMC5"  \n" // Store the value of PMC5 in x6
        "       isb             \n"
        "       mov x1, %1      \n" // Load the outer loop count into x1
        "1:     mov x2, %2      \n" // Load the inner loop count into x2

            "2:     sub x2, x2, 1   \n" // Subtract the inner loop counter
            "       cmp x2, 0       \n"
            "       bne 2b          \n" // Jump back if the counter is not 0

        "       sub x1, x1, 1   \n" // Subtract the outer loop value
        "       cmp x1, 0       \n"
        "       bne 1b          \n" // Jump back if not 0
        "       isb             \n"
        "       mrs x6, "PMC5"  \n" // Store the value of PMC5 in x6
        "       isb             \n"
        "       sub %0, x6, x5  \n"
        : "=r"(diff) // Output variables
        : "r"(outer_count), "r"(inner_count) // Input variables
        : "x1", "x2", "x5", "x6" // Clobbered registers
    );

    return NULL;
}

int main(int argc, char* argv[]) {

    outer_count = atoi(argv[1]);
    inner_count = atoi(argv[2]);

    pthread_t background_thread;
    if(pthread_create(&background_thread, NULL, testing_thread, NULL) != 0) {
       return 1; 
    }
    pthread_join(background_thread, NULL);

    printf("%d %llu %lu\n", core, diff, clocks);

    return 0;
}