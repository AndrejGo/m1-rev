#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "../../loop_predictor/m1.h"

int main() {

    // Give the OS a hint to schedule the thread executing main()
    // as a background process -> on an Icestorm core.
#ifdef ICESTORM
    pthread_set_qos_class_self_np(QOS_CLASS_BACKGROUND, 0);
#else
    pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);
#endif

    // Sleeping causes the OS to unschedule this thread and then
    // bring it back, hopefully considering the hint.
    usleep(200);

    // Grant user space applications access to PMCs
    set_sysctl("kern.pmcr0", PMCR0_DEFAULT_XCODE);

    // Profile the core this thread is executing on.
    unsigned long clocks = num_cycles_to_do_FP_loop(1000);
    // 0 = Firestorm
    // 1 = Icestorm
    int core = clocks < CYCLE_LIMIT_FOR_EFFICIENT_CORE ? 0 : 1;

    // Configure the counter to trach branch mispredictions
    // Enable PMC5
    uint64_t pmcr1 = SREG_READ(PMCR1);
    pmcr1 |= EL0A64_ENABLE(5);
    SREG_WRITE(PMCR1, pmcr1);

    // Set PMC5 to count branch mispredictions
    uint64_t pmesr0 = SREG_READ(PMESR0);
    pmesr0 = PMESR0_SET_EVENT(pmesr0, 5, BRANCH_MISPRED_NONSPEC);
    SREG_WRITE(PMESR0, pmesr0);

    uint64_t diff = 0;
    uint64_t loop_count = LOOP_COUNT;
    uint64_t modulus = MODULUS;

    __asm__ volatile (
        BRANCH_SANITIZE_1024
        "       isb             \n"
        "       mrs x5, "PMC5"  \n" // Store the value of PMC5 in x5
        "       isb             \n"
        "       mov x1, %1      \n" // Load the loop count into x1
        "       mov x2, %2      \n" // Load the modulus into x2

        "1:     udiv x3, x1, x2     \n"         // x3 = x1 / x2
        "       mul x3, x3, x2      \n"         // x3 = x3 * x2
        "       subs x3, x1, x3     \n"         // x3 = x1 - x3 ... x3 now contains remainder
        "       cbnz x3, 2f             \n"     // If remainder is 0, jump to 2
        "       add x3, x3, 1       \n"
        "2:     add x3, x3, 2       \n"

        "       add x3, x3, 1       \n"
        "       add x3, x3, 1       \n"
        "       add x3, x3, 1       \n"

        "       sub x1, x1, 1   \n" // Subtract the outer loop value
        "       cmp x1, 0       \n"
        "       bne 1b          \n" // Jump back if not 0
        "       isb             \n"
        "       mrs x6, "PMC5"  \n" // Store the value of PMC5 in x6
        "       isb             \n"
        "       sub %0, x6, x5  \n"
        : "=r"(diff) // Output variables
        : "r"(loop_count), "r"(modulus) // Input variables
        : "x1", "x2", "x3", "x4", "x5", "x6" // Clobbered registers
    );

    printf("%d %llu\n", core, diff);

    return 0;
}