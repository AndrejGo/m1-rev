#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../m1.h"
#include <unistd.h>

#define ICESTORM
#define LOOP_COUNT 1000

uint64_t clocks;

void* thread_function(void* arg) {

    // Set the BACKGROUND quality of service class
    pthread_set_qos_class_self_np(QOS_CLASS_BACKGROUND, 0);

    // Set PMCR0 and go to sleep. We want the OS to "unschedule"
    // the thread and then bring it back, hopefully on an
    // Icestorm core.
    usleep(100);

    set_sysctl("kern.pmcr0", PMCR0_DEFAULT_XCODE);
    clocks = num_cycles_to_do_FP_loop(LOOP_COUNT);

    return NULL;
}

int main() {

    // Start the thread or profile directly
#ifdef ICESTORM

    pthread_t background_thread;
    if(pthread_create(&background_thread, NULL, thread_function, NULL) != 0) {
       return 1; 
    }
    pthread_join(background_thread, NULL);
#else
    pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);
    usleep(100);
    set_sysctl("kern.pmcr0", PMCR0_DEFAULT_XCODE);
    clocks = num_cycles_to_do_FP_loop(LOOP_COUNT);
#endif

    printf("%llu\n", clocks);

    /*
    uint64_t start_cycles = SREG_READ(PMC0);

    uint64_t loop_count = 5000;
    __asm__ volatile (
        "       mov x0, %0          \n"
        "       mov x1, #0x3ff0000000000000     \n" // IEEE 754 1.0
        "       fmov d0, x1         \n"
        "       fmov d1, x1         \n"
        "1:     fadd d2, d0, d1     \n"
        "       fadd d3, d0, d1     \n"
        "       fadd d4, d0, d1     \n"
        "       fadd d5, d0, d1     \n"
        "       sub x0, x0, 1       \n"
        "       cmp x0, 0           \n"
        "       bne 1b              \n"
        : // Output
        : "r"(loop_count) // Input
        : "x0", "d0", "d1", "d2", "d3", "d4", "d5" // Clobbers
    );

    uint64_t end_cycles = SREG_READ(PMC0);

    uint cpuid = SREG_READ("TPIDRRO_EL0") & 7;
    printf("%llu %x\n", end_cycles - start_cycles, cpuid);
    */

    return 0;
}