#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <pthread.h>
#include "../m1.h"
#include <unistd.h>

#define ICE 0
#define FIRE 1

volatile int noopti = 1;

int main(int argc, char* argv[]) {

#ifdef ICESTORM
    pthread_set_qos_class_self_np(QOS_CLASS_BACKGROUND, 0);
#else
    pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);
#endif

    // Go to sleep. We want the OS to "unschedule"
    // the thread and then bring it back, hopefully on the specified core.
    usleep(100);

    set_sysctl("kern.pmcr0", PMCR0_DEFAULT_XCODE);
    unsigned long clocks = num_cycles_to_do_FP_loop(1000);

    int core = -1;
    if(clocks < CYCLE_LIMIT_FOR_EFFICIENT_CORE) {
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

    uint64_t pmc5_before = SREG_READ(PMC5);

    __asm__ volatile ("isb");

    int i = 0;
    do {
        int j = 0;
        do {
            j++;
        } while(j < INNER_COUNT);
        i++;
    } while(i < 1000);

    __asm__ volatile ("isb");

    uint64_t pmc5_after = SREG_READ(PMC5);

    __asm__ volatile ("isb");


    /*
    __asm__ volatile (
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
    */

    printf("%d %llu\n", core, pmc5_after - pmc5_before);

    return 0;
}