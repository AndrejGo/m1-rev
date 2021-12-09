#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <pthread.h>
#include "../m1.h"

int main(int argc, char* argv[]) {

    pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);

    if(argc < 6) {
        printf("Usage: ./loop_predictor <outer-count> <inner-count>\n");
        return 0;
    }

    uint64_t outer_count = atoi(argv[1]);
    uint64_t inner_count = atoi(argv[2]);

    set_sysctl("kern.pmcr0", PMCR0_DEFAULT_XCODE);

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
        : "=r"(pmc5_diff) // Output variables
        : "r"(outer_count), "r"(inner_count) // Input variables
        : "x1", "x2", "x5", "x6" // Clobbered registers
    );

    printf("%llu\n", pmc5_diff);

    uint64_t count1 = atoi(argv[3]);
    uint64_t count2 = atoi(argv[4]);
    uint64_t count3 = atoi(argv[5]);

    __asm__ volatile (
        "       mrs x5, "PMC5"  \n" // Store the value of PMC5 in x6
        "       isb             \n"
        "       mov x1, %1      \n" // Load the outer loop count into x1
        "4:     mov x2, %2      \n" // Load the inner loop count into x2

            "5:     mov x3, %3      \n"
            
                "6:     sub x3, x3, 1   \n"
                "       cmp x3, 0       \n"
                "       bne 6b          \n"
            
            "       sub x2, x2, 1   \n" // Subtract the inner loop counter
            "       cmp x2, 0       \n"
            "       bne 5b          \n" // Jump back if the counter is not 0

        "       sub x1, x1, 1   \n" // Subtract the outer loop value
        "       cmp x1, 0       \n"
        "       bne 4b          \n" // Jump back if not 0
        "       isb             \n"
        "       mrs x6, "PMC5"  \n" // Store the value of PMC5 in x6
        "       isb             \n"
        "       sub %0, x6, x5  \n"
        : "=r"(pmc5_diff) // Output variables
        : "r"(count1), "r"(count2), "r"(count3) // Input variables
        : "x1", "x2", "x3", "x5", "x6" // Clobbered registers
    );

    printf("%llu\n", pmc5_diff);

    return 0;
}