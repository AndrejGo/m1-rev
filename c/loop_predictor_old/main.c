#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <pthread.h>
#include "../m1.h"

int main(int argc, char* argv[]) {

    pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);

    if(argc < 3) {
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

    uint64_t pmc5_before, pmc5_after;

    int result_count = (outer_count * (inner_count + 1) + 1);
    uint64_t* results = malloc(result_count * sizeof(uint64_t));

    __asm__ volatile (
        BRANCH_SANITIZE_1024
        "       mrs x6, "PMC5"  \n" // Store the value of PMC5 in x6
        "       isb             \n"
        "       mov x3, %0      \n" // Load the address of the results array into x3
        "       mov x1, %1      \n" // Load the outer loop count into x1
        "1:     mov x2, %2      \n" // Load the inner loop count into x2

            "2:     isb             \n"
            "       mrs x5, "PMC5"  \n" // Store the value of PMC5 in x5
            "       isb             \n"
            "       sub x7, x5, x6  \n" // Calculate the difference between the
                                        // current and old PMC5
            "       str x7, [x3]    \n" // Store the difference in the result array
            "       add x3, x3, 8   \n" // Increment the array offset
            "       mov x6, x5      \n" // Store the current PMC5 in x6 for the next
                                        // loop iteration.
            "       sub x2, x2, 1   \n" // Subtract the inner loop counter
            "       cmp x2, 0       \n"
            "       bne 2b          \n" // Jump back if the counter is not 0

        "       isb             \n"
        "       mrs x5, "PMC5"  \n" // Get the misprediction result of the last
        "       isb             \n" // repetition of the inner loop.
        "       sub x7, x5, x6  \n"
        "       str x7, [x3]    \n"
        "       add x3, x3, 8   \n"
        "       mov x6, x5      \n"

        "       sub x1, x1, 1   \n" // Subtract the outer loop value
        "       cmp x1, 0       \n"
        "       bne 1b          \n" // Jump back if not 0
        : // Output variables
        : "r"(results), "r"(outer_count), "r"(inner_count) // Input variables
        : "x1", "x2", "x3", "x5", "x6", "x7" // Clobbered registers
    );

    /*
    for(int i = 0; i < outer_count; i++) {
        for(int j = 0; j < inner_count + 1; j++) {
            int index = 1 + i * (inner_count + 1) + j;
            printf("%llu\n", results[index]);
        }
    }
    */

    for(int i = 1; i < result_count; i++) {
        if(i % (inner_count+1) == 0) {
            printf("\x1B[46m");
            if(results[i] >= 1) {
                printf("\x1B[31;1m");
            } else {
                printf("\x1B[30m");
            }
            printf("%llu", results[i]);
            printf("\x1B[0m\n");
        } else {
            if(results[i] >= 1) {
                printf("\x1B[32;1m");
            }
            printf("%llu ", results[i]);
            printf("\x1B[0m");
        }
    }
    printf("\n");

    return 0;
}