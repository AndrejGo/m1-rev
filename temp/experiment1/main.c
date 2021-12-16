#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "../../loop_predictor/m1.h"

//#define LOOP_COUNT 10000
//#define MODULUS 100

int main() {

    // Give the OS a hint to schedule the thread executing main()
    // as a background process -> on an Icestorm core.
    //pthread_set_qos_class_self_np(QOS_CLASS_BACKGROUND, 0);
    pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);

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

    // Get the misprediction count before the branches
    uint64_t before = SREG_READ(PMC5);

    volatile int a = 1;

    for(unsigned long i = 0; i < LOOP_COUNT; i++) {
        if(i % MODULUS == 0) {
            a = 0;
        }
    }

    // Get the misprediction count after the branches
    uint64_t after = SREG_READ(PMC5);

    printf("%d %llu\n", core, after - before);

    return 0;
}