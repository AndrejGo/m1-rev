#ifndef M1_H
#define M1_H

// ========================================================================== //
// System register name definitions
#define PMCR0 "S3_1_c15_c0_0"
#define PMCR1 "S3_1_c15_c1_0"
#define PMESR0 "S3_1_c15_c5_0"
#define PMESR1 "S3_1_c15_c6_0"

#define PMC0 "S3_2_c15_c0_0"
#define PMC1 "S3_2_c15_c1_0"
#define PMC2 "S3_2_c15_c2_0"
#define PMC3 "S3_2_c15_c3_0"
#define PMC4 "S3_2_c15_c4_0"
#define PMC5 "S3_2_c15_c5_0"
#define PMC6 "S3_2_c15_c6_0"
#define PMC7 "S3_2_c15_c7_0"
#define PMC8 "S3_2_c15_c9_0"
#define PMC9 "S3_2_c15_c10_0"

// ========================================================================== //
// Event numbers
#define UNKNOWN_EVENT 0x00

// Atomic or exclusive instruction failed (due to contention).
#define ATOMIC_OR_EXCLUSIVE_FAIL UNKNOWN_EVENT // PMI Error

// Atomic or exclusive instruction successfully completed.
#define ATOMIC_OR_EXCLUSIVE_SUCC UNKNOWN_EVENT // PMI Error

// Retired indirect call instructions mispredicted.
#define BRANCH_CALL_INDIR_MISPRED_NONSPEC UNKNOWN_EVENT // PMI Error

// (PMC5) Retired conditional branch instructions that mispredicted.
#define BRANCH_COND_MISPRED_NONSPEC 0xc5

// (PMC5) Retired indirect branch instructions including calls and returns that
// mispredicted.
#define BRANCH_INDIR_MISPRED_NONSPEC 0xc6

// (PMC5) Retired branch instructions including calls and returns that
// mispredicted.
#define BRANCH_MISPRED_NONSPEC 0xcb

// (PMC5) Retired return instructions that mispredicted.
#define BRANCH_RET_INDIR_MISPRED_NONSPEC 0xc8

// (PMC2) Cycles while the core was active.
#define CORE_ACTIVE_CYCLE 0x2

// (PMC0) Number of clock cycles elapsed.
#define FIXED_CYCLES 0x00 // PMC0 is always cycles

// (PMC2) Fetch Unit internal restarts for any reason. Does not include branch
// mispredicts.
#define FETCH_RESTART 0xde

// (PMC2) Pipeline flush and restarts that were not due to branch mispredictions
// or memory order violations.
#define FLUSH_RESTART_OTHER_NONSPEC 0x84

// (PMC7) All retired instructions.
#define INST_ALL 0x8c

// (PMC5) Retired barrier instructions.
#define INST_BARRIER 0x9c

// (PMC5) Retired branch instructions including calls and returns.
#define INST_BRANCH 0x8d

// (PMC5) Retired subroutine call instructions.
#define INST_BRANCH_CALL 0x8e

// (PMC5) Retired conditional branch instructions.
#define INST_BRANCH_COND 0x94

// (PMC5) Retired indirect branch instructions.
#define INST_BRANCH_INDIR 0x93

// (PMC5) Retired subroutine return instructions.
#define INST_BRANCH_RET 0x8f

// (PMC5) Retired taken branch instructions.
#define INST_BRANCH_TAKEN 0x90

// (PMC7) Retired non-branch and non-load/store Integer Unit instructions.
#define INST_INT_ALU 0x97

// (PMC7) Retired load and store instructions.
#define INST_LDST 0x9b // PMCR1: 0x38300 PMESR1: 0x9b00

// (PMC7) Retired non-load/store Advanced SIMD and FP Unit instructions.
#define INST_SIMD_ALU 0x9a

// (PMC2) Cycles when an interrupt was pending because it was masked.
#define INTERRUPT_PENDING 0x6c

// (PMC1) Number of retired instructions.
#define FIXED_INSTRUCTIONS UNKNOWN_EVENT // PMC1 is always instructions.

// (PMC2) Loads that missed the L1 Data Cache.
#define L1D_CACHE_MISS_LD 0xa3

// (PMC5) Retired loads that missed in the L1 Data Cache.
#define L1D_CACHE_MISS_LD_NONSPEC 0xbf

// (PMC2) Stores that missed the L1 Data Cache.
#define L1D_CACHE_MISS_ST 0xa2

// (PMC5) Retired stores that missed in the L1 Data Cache.
#define L1D_CACHE_MISS_ST_NONSPEC 0xc0

// (PMC2) Dirty cache lines written back from the L1D Cache toward the Shared L2
// Cache.
#define L1D_CACHE_WRITEBACK 0xa8

// (PMC2) Load and store accesses to the L1 Data TLB.
#define L1D_TLB_ACCESS 0xa0

// (PMC2) Translations filled into the L1 Data TLB.
#define L1D_TLB_FILL 0x05

// (PMC2) Load and store accesses that missed the L1 Data TLB.
#define L1D_TLB_MISS 0xa1

// (PMC5) Retired loads and stores that missed in the L1 Data TLB.
#define L1D_TLB_MISS_NONSPEC 0xc1

// (PMC2) Demand instruction fetches that missed in the L1 Instructtion Cache.
#define L1I_CACHE_MISS_DEMAND 0xd3

// (PMC2) Translations filled into the L1 Instruction TLB.
#define L1I_TLB_FILL 0x04

// (PMC2) Demand instruction fetches that missed in the L1 Instruction TLB.
#define L1I_TLB_MISS_DEMAND 0xd4

// (PMC2) Loads and stores that missed in the L2 TLB.
#define L2_TLB_MISS_DATA 0x0b

// (PMC2) Instruction fetches that missed in the L2 TLB.
#define L2_TLB_MISS_INSTRUCTION 0x0a

// (PMC2) Load and store uops that crossed a 64B boundary.
#define LDST_X64_UOP 0xb1

// (PMC2) Load and store uops that crossed a 16KiB page boundary.
#define LDST_XPG_UOP 0xb2

// (PMC2) Load uops that executed with non-temporal hint.
#define LD_NT_UOP 0xe6

// (PMC2) Uops that flowed through the Load Unit.
#define LD_UNIT_UOP 0xa6

// (PMC2) Bubble detected in dispatch stage.
#define MAP_DISPATCH_BUBBLE 0xd6

// (PMC2) Mapped Unteger Unit uops.
#define MAP_INT_UOP 0x7c

// (PMC2) Mapped Load and Store Unit uops, including GPR to vector register
// converts.
#define MAP_LDST_UOP 0x7d

// (PMC2) Cycles while the Map Unit was blocked while rewinding due to flush and
// restart.
#define MAP_REWIND 0x75

// (PMC2) Mapped Advanced SIMD and FP Unit uops.
#define MAP_SIMD_UOP 0x7e

// (PMC2) Cycles while the Map Unit was stalled for any reason.
#define MAP_STALL 0x76

// (PMC2) Cycles while the Map Unit was stalled because of Dispatch back
// pressure.
#define MAP_STALL_DISPATCH 0x70

// (PMC2) Table walk memory requests on behalf of data accesses.
#define MMU_TABLE_WALK_DATA 0x08

// (PMC2) Table walk memory requests on behalf of instruction fetches.
#define MMU_TABLE_WALK_INSTRUCTION 0x07

// (PMC2) Memory accesses that reached retirement that triggered any of the MMU
// vrtual memory faults.
#define MMU_VIRTUAL_MEMORY_FAULT_NONSPEC 0x0d

// (PMC7) All retired uops.
#define RETIRE_UOP 0x01

// (PMC2) Uops issued by the scheduler to any execution unit.
#define SCHEDULE_UOP 0x52

// (PMC5) Retired stores that triggered memory order violations.
#define ST_MEMORY_ORDER_VIOLATION_NONSPEC 0xc4

// (PMC2) Store uops that executed with non-temporal hint.
#define ST_NT_UOP 0xe5

// (PMC2) Uops that flowed through the Store Unit.
#define ST_UNIT_UOP 0xa7

// ========================================================================== //
// Register reading and writing

#define SREG_WRITE(SR, V) __asm__ volatile("msr " SR ", %0 ; isb" : : "r"(V))
#define SREG_READ(SR) ({ \
    uint64_t VAL; \
    __asm__ volatile("mrs %0, " SR : "=r"(VAL)); \
    VAL; \
})

// Enable bit in PMCR1
#define EL0A64_ENABLE(CNT) (1 << ((CNT) + 8))

#define PMCR1_DISABLE_CNT(CURR_PMCR1, CNT) ({ \
    uint64_t v = CURR_PMCR1; \
    v &= ~EL0A64_ENABLE(CNT); \
    v; \
})

#define PMCR1_ENABLE_CNT(CURR_PMCR1, CNT) ({ \
    uint64_t v = CURR_PMCR1; \
    v |= EL0A64_ENABLE(CNT); \
    v; \
})

// Event number in PMESR
#define EVENT_NUM_MASK ({ uint64_t v = 0xff; v; })

#define PMESR0_SHIFT(CNT) (8 * ((CNT) - 2))
#define PMESR1_SHIFT(CNT) (8 * ((CNT) - 6))

#define PMESR0_MASK(CNT) (EVENT_NUM_MASK << PMESR0_SHIFT(CNT))
#define PMESR1_MASK(CNT) (EVENT_NUM_MASK << PMESR1_SHIFT(CNT))

#define PMESR0_ENCODE_EVENT(CNT, EVT) ((EVT & EVENT_NUM_MASK) << PMESR0_SHIFT(CNT))
#define PMESR1_ENCODE_EVENT(CNT, EVT) ((EVT & EVENT_NUM_MASK) << PMESR1_SHIFT(CNT))

#define PMESR0_SET_EVENT(CURR_PMESR0, CNT, EVT) ({ \
    uint64_t v = CURR_PMESR0; \
    v &= ~PMESR0_MASK(CNT); \
    v |= PMESR0_ENCODE_EVENT(CNT, EVT); \
    v; \
})

#define PMESR1_SET_EVENT(CURR_PMESR1, CNT, EVT) ({ \
    uint64_t v = CURR_PMESR1; \
    v &= ~PMESR1_MASK(CNT); \
    v |= PMESR1_ENCODE_EVENT(CNT, EVT); \
    v; \
})

#endif /* M1_H */