#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <sys/sysctl.h>
#include "../m1.h"

static void set_sysctl(const char *name, uint64_t v) {
    if (sysctlbyname(name, NULL, 0, &v, sizeof v)) {
        printf("set_sysctl: sysctlbyname failed\n");
        exit(1);
    }
}

int main(int argc, char* argv[]) {

    uint64_t random_bits;
    uint64_t random_bit = 0;
    uint64_t mask = 0x1;
    random_bit ^= random_bits & mask;         mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 1;  mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 2;  mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 3;  mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 4;  mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 5;  mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 6;  mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 7;  mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 8;  mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 9;  mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 10; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 11; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 12; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 13; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 14; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 15; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 16; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 17; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 18; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 19; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 20; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 21; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 22; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 23; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 24; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 25; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 26; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 27; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 28; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 29; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 30; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 31; mask = mask << 1;

    random_bit ^= (random_bits & mask) >> 32; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 33; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 34; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 35; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 36; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 37; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 38; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 39; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 40; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 41; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 42; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 43; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 44; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 45; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 46; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 47; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 48; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 49; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 50; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 51; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 52; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 53; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 54; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 55; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 56; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 57; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 58; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 59; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 60; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 61; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 62; mask = mask << 1;
    random_bit ^= (random_bits & mask) >> 63;

    // Enable PMCs
    set_sysctl("kern.pmcr0", 0x3003470ff4ff);

    // Enable PMC2 in 64 bit EL0 mode
    uint64_t pmcr1 = SREG_READ(PMCR1);
    pmcr1 = PMCR1_ENABLE_CNT(pmcr1, 5);
    SREG_WRITE(PMCR1, pmcr1);

    // Configure PMC2 to count branch mispredictions
    uint64_t pmesr0 = SREG_READ(PMESR0);
    pmesr0 = PMESR0_SET_EVENT(pmesr0, 5, BRANCH_MISPRED_NONSPEC);
    SREG_WRITE(PMESR0, pmesr0);

    uint64_t pmc2_before, pmc2_after;

    __asm__ volatile (
        "           isb                   \n"
        "           mrs %0, "PMC5"        \n"
        "           isb                   \n"
        "           cmp %2, 0             \n"
        "           beq 0f                \n"
        "           mov x1, 0             \n"
        "           add x1, x1, 1         \n"
        "           add x1, x1, 1         \n"
        "           add x1, x1, 1         \n"
        "           add x1, x1, 1         \n"
        "           add x1, x1, 1         \n"
        "           add x1, x1, 1         \n"
        "           add x1, x1, 1         \n"
        "           add x1, x1, 1         \n"
        "           add x1, x1, 1         \n"
        "           add x1, x1, 1         \n"
        "0:                               \n"
        "           isb                   \n"
        "           mrs %1, "PMC5"        \n"
        "           isb                   \n"
        : "=r"(pmc2_before), "=r"(pmc2_after) // Output variables
        : "r"(random_bit) // Input variables
        : "x1" // Clobbered registers
    );

    printf("%lld\n", pmc2_after - pmc2_before);

    return 0;
}