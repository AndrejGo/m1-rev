#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <pthread.h>
#include <unistd.h>
#include "../../loop_predictor/m1.h"

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

    // Configure the counter to trach branch mispredictions
    // Enable PMC5
    uint64_t pmcr1 = SREG_READ(PMCR1);
    pmcr1 |= EL0A64_ENABLE(5);
    SREG_WRITE(PMCR1, pmcr1);

    // Set PMC5 to count branch mispredictions
    uint64_t pmesr0 = SREG_READ(PMESR0);
    pmesr0 = PMESR0_SET_EVENT(pmesr0, 5, BRANCH_INDIR_MISPRED_NONSPEC);
    SREG_WRITE(PMESR0, pmesr0);

    // Get the misprediction count before the branches
    uint64_t before = SREG_READ(PMC5);
    uint64_t clk_before = SREG_READ(PMC1);

    uint64_t temp = 0;
    for(unsigned long i = 0; i < LOOP_COUNT; i++) {
        __asm__ volatile (
            ASM_CODE
            : "=r"(temp) // Output variables
            : // Input variables
            : CLOBERED // Clobbered registers
        );
    }

    //printf("temp: %llx\n", temp);

    // Get the misprediction count after the branches
    uint64_t after = SREG_READ(PMC5);
    uint64_t clk_after = SREG_READ(PMC1);

    
    printf("%d %llu %llu\n", core, after - before, clk_after - clk_before);

}