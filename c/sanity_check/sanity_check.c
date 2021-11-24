#include <stdlib.h>
#include <stdio.h>
#include <sys/sysctl.h>
#include <inttypes.h>
#include <pthread.h>

#define SREG_PMCR0  "S3_1_c15_c0_0"
#define SREG_PMCR1  "S3_1_c15_c1_0"
#define PMC0 "S3_2_c15_c0_0"

/*
 * Thanks Apple!
 * Macros for reading and writing system registers.
 *
 * SR must be one of the SREG_* defines above.
 */
#define SREG_WRITE(SR, V) __asm__ volatile("msr " SR ", %0 ; isb" : : "r"(V))
#define SREG_READ(SR)     ({ uint64_t VAL; \
                             __asm__ volatile("mrs %0, " SR : "=r"(VAL)); \
                             VAL; })

static void set_sysctl(const char *name, uint64_t v) {
    if (sysctlbyname(name, NULL, 0, &v, sizeof v)) {
        printf("set_sysctl: sysctlbyname failed\n");
        exit(1);
    }
}

static uint64_t get_sysctl(const char *name) {
    uint64_t v = 0;
    size_t l = sizeof(v);
    if (sysctlbyname(name, &v, &l, NULL, 0)) {
        printf("get_sysctl: sysctlbyname failed\n");
        exit(1);
    }
    return v;
}

// I don't think this works.
/*
static int get_current_core() {
    return SREG_READ("TPIDRRO_EL0") & 7;
}
*/

volatile unsigned long dont_optimize_me;

int main(int argc, char* argv[]) {

    if(argc < 2) {
        printf("Please run the program with a loop count\n");
        return 0;
    }

    // We will perform operations on this variable in loops to avoid Clang
    // optimizing them away.
    dont_optimize_me = 0;

    // Give a hint to run this on a Firestorm core
    pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);

    // Use the kernel extension to initialize PMCR0. The most important thing
    // for us is setting bit 30.
    set_sysctl("kern.pmcr0", 0x3003400ff4ff);

    int loop_count = atoi(argv[1]);

    unsigned long long start = 0;
    __asm__ volatile (
        "           mrs %0, "PMC0"       \n\t"
        "           isb                  \n\t"
        : "=r"(start) // Output variables
        : // Input variables
        : // Clobbered registers
    );

    for(int i = 0; i < loop_count; i++) {
        dont_optimize_me |= i;
    }

    unsigned long long stop = 0;
    __asm__ volatile (
        "           isb                  \n\t"
        "           mrs %0, "PMC0"       \n\t"
        "           isb                  \n\t"
        : "=r"(stop) // Output variables
        : // Input variables
        : // Clobbered registers
    );

    printf("%llu\n", stop - start);

    return 0;
}