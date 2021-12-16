#ifndef M1_H
#define M1_H

#include <inttypes.h>
#include <sys/sysctl.h>

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
#define INST_LDST 0x9b

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

#define PMCR0_DEFAULT_XCODE 0x3003400ff4ff

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

# define CYCLE_LIMIT_FOR_EFFICIENT_CORE 3500
// CPU profiling to identify if we are running on Icestorm or Firestorm.
unsigned long num_cycles_to_do_FP_loop(uint64_t loop_count) {

    uint64_t start_cycles = SREG_READ(PMC0);

    __asm__ volatile (
        "       mov x0, %0          \n"
        "       mov x1, #0x3ff0000000000000     \n" // IEEE 754 for 1.0
        "       fmov d0, x1         \n"
        "       mov x2, #0x401c000000000000     \n"
        "       fmov d1, x2         \n"
        //"       movi v0.16b, 9      \n"
        //"       movi v1.16b, 10     \n"
        "1:     fadd d2, d0, d1     \n"
        "       fadd d3, d0, d1     \n"
        "       fadd d4, d0, d1     \n"
        "       fadd d5, d0, d1     \n"
        "       fadd d6, d0, d1     \n"
        "       fadd d7, d0, d1     \n"
        "       fadd d8, d0, d1     \n"
        "       fadd d9, d0, d1     \n"

        "       sub x0, x0, 1       \n"
        "       cmp x0, 0           \n"
        "       bne 1b              \n"
        "       isb                 \n"
        : // Output
        : "r"(loop_count) // Input
        : "x0", "x1", "x2", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8", "d9" // Clobbers
    );

    uint64_t end_cycles = SREG_READ(PMC0);

    return (unsigned long)(end_cycles - start_cycles);
}

// ========================================================================== //
// Branch sanitization with a repeated pattern
// - BRANCH_SANITIZE_256
// - BRANCH_SANITIZE_512
// - BRANCH_SANITIZE_1024

#define BRANCH_SANITIZE_256 "l0:      b l2       \n" \
"l1:      b l4       \n" \
"l2:      b l1       \n" \
"l3:      b l6       \n" \
"l4:      b l3       \n" \
"l5:      b l8       \n" \
"l6:      b l5       \n" \
"l7:      b l10      \n" \
"l8:      b l7       \n" \
"l9:      b l12      \n" \
"l10:     b l9       \n" \
"l11:     b l14      \n" \
"l12:     b l11      \n" \
"l13:     b l16      \n" \
"l14:     b l13      \n" \
"l15:     b l18      \n" \
"l16:     b l15      \n" \
"l17:     b l20      \n" \
"l18:     b l17      \n" \
"l19:     b l22      \n" \
"l20:     b l19      \n" \
"l21:     b l24      \n" \
"l22:     b l21      \n" \
"l23:     b l26      \n" \
"l24:     b l23      \n" \
"l25:     b l28      \n" \
"l26:     b l25      \n" \
"l27:     b l30      \n" \
"l28:     b l27      \n" \
"l29:     b l32      \n" \
"l30:     b l29      \n" \
"l31:     b l34      \n" \
"l32:     b l31      \n" \
"l33:     b l36      \n" \
"l34:     b l33      \n" \
"l35:     b l38      \n" \
"l36:     b l35      \n" \
"l37:     b l40      \n" \
"l38:     b l37      \n" \
"l39:     b l42      \n" \
"l40:     b l39      \n" \
"l41:     b l44      \n" \
"l42:     b l41      \n" \
"l43:     b l46      \n" \
"l44:     b l43      \n" \
"l45:     b l48      \n" \
"l46:     b l45      \n" \
"l47:     b l50      \n" \
"l48:     b l47      \n" \
"l49:     b l52      \n" \
"l50:     b l49      \n" \
"l51:     b l54      \n" \
"l52:     b l51      \n" \
"l53:     b l56      \n" \
"l54:     b l53      \n" \
"l55:     b l58      \n" \
"l56:     b l55      \n" \
"l57:     b l60      \n" \
"l58:     b l57      \n" \
"l59:     b l62      \n" \
"l60:     b l59      \n" \
"l61:     b l64      \n" \
"l62:     b l61      \n" \
"l63:     b l66      \n" \
"l64:     b l63      \n" \
"l65:     b l68      \n" \
"l66:     b l65      \n" \
"l67:     b l70      \n" \
"l68:     b l67      \n" \
"l69:     b l72      \n" \
"l70:     b l69      \n" \
"l71:     b l74      \n" \
"l72:     b l71      \n" \
"l73:     b l76      \n" \
"l74:     b l73      \n" \
"l75:     b l78      \n" \
"l76:     b l75      \n" \
"l77:     b l80      \n" \
"l78:     b l77      \n" \
"l79:     b l82      \n" \
"l80:     b l79      \n" \
"l81:     b l84      \n" \
"l82:     b l81      \n" \
"l83:     b l86      \n" \
"l84:     b l83      \n" \
"l85:     b l88      \n" \
"l86:     b l85      \n" \
"l87:     b l90      \n" \
"l88:     b l87      \n" \
"l89:     b l92      \n" \
"l90:     b l89      \n" \
"l91:     b l94      \n" \
"l92:     b l91      \n" \
"l93:     b l96      \n" \
"l94:     b l93      \n" \
"l95:     b l98      \n" \
"l96:     b l95      \n" \
"l97:     b l100     \n" \
"l98:     b l97      \n" \
"l99:     b l102     \n" \
"l100:    b l99      \n" \
"l101:    b l104     \n" \
"l102:    b l101     \n" \
"l103:    b l106     \n" \
"l104:    b l103     \n" \
"l105:    b l108     \n" \
"l106:    b l105     \n" \
"l107:    b l110     \n" \
"l108:    b l107     \n" \
"l109:    b l112     \n" \
"l110:    b l109     \n" \
"l111:    b l114     \n" \
"l112:    b l111     \n" \
"l113:    b l116     \n" \
"l114:    b l113     \n" \
"l115:    b l118     \n" \
"l116:    b l115     \n" \
"l117:    b l120     \n" \
"l118:    b l117     \n" \
"l119:    b l122     \n" \
"l120:    b l119     \n" \
"l121:    b l124     \n" \
"l122:    b l121     \n" \
"l123:    b l126     \n" \
"l124:    b l123     \n" \
"l125:    b l128     \n" \
"l126:    b l125     \n" \
"l127:    b l130     \n" \
"l128:    b l127     \n" \
"l129:    b l132     \n" \
"l130:    b l129     \n" \
"l131:    b l134     \n" \
"l132:    b l131     \n" \
"l133:    b l136     \n" \
"l134:    b l133     \n" \
"l135:    b l138     \n" \
"l136:    b l135     \n" \
"l137:    b l140     \n" \
"l138:    b l137     \n" \
"l139:    b l142     \n" \
"l140:    b l139     \n" \
"l141:    b l144     \n" \
"l142:    b l141     \n" \
"l143:    b l146     \n" \
"l144:    b l143     \n" \
"l145:    b l148     \n" \
"l146:    b l145     \n" \
"l147:    b l150     \n" \
"l148:    b l147     \n" \
"l149:    b l152     \n" \
"l150:    b l149     \n" \
"l151:    b l154     \n" \
"l152:    b l151     \n" \
"l153:    b l156     \n" \
"l154:    b l153     \n" \
"l155:    b l158     \n" \
"l156:    b l155     \n" \
"l157:    b l160     \n" \
"l158:    b l157     \n" \
"l159:    b l162     \n" \
"l160:    b l159     \n" \
"l161:    b l164     \n" \
"l162:    b l161     \n" \
"l163:    b l166     \n" \
"l164:    b l163     \n" \
"l165:    b l168     \n" \
"l166:    b l165     \n" \
"l167:    b l170     \n" \
"l168:    b l167     \n" \
"l169:    b l172     \n" \
"l170:    b l169     \n" \
"l171:    b l174     \n" \
"l172:    b l171     \n" \
"l173:    b l176     \n" \
"l174:    b l173     \n" \
"l175:    b l178     \n" \
"l176:    b l175     \n" \
"l177:    b l180     \n" \
"l178:    b l177     \n" \
"l179:    b l182     \n" \
"l180:    b l179     \n" \
"l181:    b l184     \n" \
"l182:    b l181     \n" \
"l183:    b l186     \n" \
"l184:    b l183     \n" \
"l185:    b l188     \n" \
"l186:    b l185     \n" \
"l187:    b l190     \n" \
"l188:    b l187     \n" \
"l189:    b l192     \n" \
"l190:    b l189     \n" \
"l191:    b l194     \n" \
"l192:    b l191     \n" \
"l193:    b l196     \n" \
"l194:    b l193     \n" \
"l195:    b l198     \n" \
"l196:    b l195     \n" \
"l197:    b l200     \n" \
"l198:    b l197     \n" \
"l199:    b l202     \n" \
"l200:    b l199     \n" \
"l201:    b l204     \n" \
"l202:    b l201     \n" \
"l203:    b l206     \n" \
"l204:    b l203     \n" \
"l205:    b l208     \n" \
"l206:    b l205     \n" \
"l207:    b l210     \n" \
"l208:    b l207     \n" \
"l209:    b l212     \n" \
"l210:    b l209     \n" \
"l211:    b l214     \n" \
"l212:    b l211     \n" \
"l213:    b l216     \n" \
"l214:    b l213     \n" \
"l215:    b l218     \n" \
"l216:    b l215     \n" \
"l217:    b l220     \n" \
"l218:    b l217     \n" \
"l219:    b l222     \n" \
"l220:    b l219     \n" \
"l221:    b l224     \n" \
"l222:    b l221     \n" \
"l223:    b l226     \n" \
"l224:    b l223     \n" \
"l225:    b l228     \n" \
"l226:    b l225     \n" \
"l227:    b l230     \n" \
"l228:    b l227     \n" \
"l229:    b l232     \n" \
"l230:    b l229     \n" \
"l231:    b l234     \n" \
"l232:    b l231     \n" \
"l233:    b l236     \n" \
"l234:    b l233     \n" \
"l235:    b l238     \n" \
"l236:    b l235     \n" \
"l237:    b l240     \n" \
"l238:    b l237     \n" \
"l239:    b l242     \n" \
"l240:    b l239     \n" \
"l241:    b l244     \n" \
"l242:    b l241     \n" \
"l243:    b l246     \n" \
"l244:    b l243     \n" \
"l245:    b l248     \n" \
"l246:    b l245     \n" \
"l247:    b l250     \n" \
"l248:    b l247     \n" \
"l249:    b l252     \n" \
"l250:    b l249     \n" \
"l251:    b l254     \n" \
"l252:    b l251     \n" \
"l253:    b l255     \n" \
"l254:    b l253     \n" \
"l255:   \n"

#define BRANCH_SANITIZE_512 "l0:      b l2       \n" \
"l1:      b l4       \n" \
"l2:      b l1       \n" \
"l3:      b l6       \n" \
"l4:      b l3       \n" \
"l5:      b l8       \n" \
"l6:      b l5       \n" \
"l7:      b l10      \n" \
"l8:      b l7       \n" \
"l9:      b l12      \n" \
"l10:     b l9       \n" \
"l11:     b l14      \n" \
"l12:     b l11      \n" \
"l13:     b l16      \n" \
"l14:     b l13      \n" \
"l15:     b l18      \n" \
"l16:     b l15      \n" \
"l17:     b l20      \n" \
"l18:     b l17      \n" \
"l19:     b l22      \n" \
"l20:     b l19      \n" \
"l21:     b l24      \n" \
"l22:     b l21      \n" \
"l23:     b l26      \n" \
"l24:     b l23      \n" \
"l25:     b l28      \n" \
"l26:     b l25      \n" \
"l27:     b l30      \n" \
"l28:     b l27      \n" \
"l29:     b l32      \n" \
"l30:     b l29      \n" \
"l31:     b l34      \n" \
"l32:     b l31      \n" \
"l33:     b l36      \n" \
"l34:     b l33      \n" \
"l35:     b l38      \n" \
"l36:     b l35      \n" \
"l37:     b l40      \n" \
"l38:     b l37      \n" \
"l39:     b l42      \n" \
"l40:     b l39      \n" \
"l41:     b l44      \n" \
"l42:     b l41      \n" \
"l43:     b l46      \n" \
"l44:     b l43      \n" \
"l45:     b l48      \n" \
"l46:     b l45      \n" \
"l47:     b l50      \n" \
"l48:     b l47      \n" \
"l49:     b l52      \n" \
"l50:     b l49      \n" \
"l51:     b l54      \n" \
"l52:     b l51      \n" \
"l53:     b l56      \n" \
"l54:     b l53      \n" \
"l55:     b l58      \n" \
"l56:     b l55      \n" \
"l57:     b l60      \n" \
"l58:     b l57      \n" \
"l59:     b l62      \n" \
"l60:     b l59      \n" \
"l61:     b l64      \n" \
"l62:     b l61      \n" \
"l63:     b l66      \n" \
"l64:     b l63      \n" \
"l65:     b l68      \n" \
"l66:     b l65      \n" \
"l67:     b l70      \n" \
"l68:     b l67      \n" \
"l69:     b l72      \n" \
"l70:     b l69      \n" \
"l71:     b l74      \n" \
"l72:     b l71      \n" \
"l73:     b l76      \n" \
"l74:     b l73      \n" \
"l75:     b l78      \n" \
"l76:     b l75      \n" \
"l77:     b l80      \n" \
"l78:     b l77      \n" \
"l79:     b l82      \n" \
"l80:     b l79      \n" \
"l81:     b l84      \n" \
"l82:     b l81      \n" \
"l83:     b l86      \n" \
"l84:     b l83      \n" \
"l85:     b l88      \n" \
"l86:     b l85      \n" \
"l87:     b l90      \n" \
"l88:     b l87      \n" \
"l89:     b l92      \n" \
"l90:     b l89      \n" \
"l91:     b l94      \n" \
"l92:     b l91      \n" \
"l93:     b l96      \n" \
"l94:     b l93      \n" \
"l95:     b l98      \n" \
"l96:     b l95      \n" \
"l97:     b l100     \n" \
"l98:     b l97      \n" \
"l99:     b l102     \n" \
"l100:    b l99      \n" \
"l101:    b l104     \n" \
"l102:    b l101     \n" \
"l103:    b l106     \n" \
"l104:    b l103     \n" \
"l105:    b l108     \n" \
"l106:    b l105     \n" \
"l107:    b l110     \n" \
"l108:    b l107     \n" \
"l109:    b l112     \n" \
"l110:    b l109     \n" \
"l111:    b l114     \n" \
"l112:    b l111     \n" \
"l113:    b l116     \n" \
"l114:    b l113     \n" \
"l115:    b l118     \n" \
"l116:    b l115     \n" \
"l117:    b l120     \n" \
"l118:    b l117     \n" \
"l119:    b l122     \n" \
"l120:    b l119     \n" \
"l121:    b l124     \n" \
"l122:    b l121     \n" \
"l123:    b l126     \n" \
"l124:    b l123     \n" \
"l125:    b l128     \n" \
"l126:    b l125     \n" \
"l127:    b l130     \n" \
"l128:    b l127     \n" \
"l129:    b l132     \n" \
"l130:    b l129     \n" \
"l131:    b l134     \n" \
"l132:    b l131     \n" \
"l133:    b l136     \n" \
"l134:    b l133     \n" \
"l135:    b l138     \n" \
"l136:    b l135     \n" \
"l137:    b l140     \n" \
"l138:    b l137     \n" \
"l139:    b l142     \n" \
"l140:    b l139     \n" \
"l141:    b l144     \n" \
"l142:    b l141     \n" \
"l143:    b l146     \n" \
"l144:    b l143     \n" \
"l145:    b l148     \n" \
"l146:    b l145     \n" \
"l147:    b l150     \n" \
"l148:    b l147     \n" \
"l149:    b l152     \n" \
"l150:    b l149     \n" \
"l151:    b l154     \n" \
"l152:    b l151     \n" \
"l153:    b l156     \n" \
"l154:    b l153     \n" \
"l155:    b l158     \n" \
"l156:    b l155     \n" \
"l157:    b l160     \n" \
"l158:    b l157     \n" \
"l159:    b l162     \n" \
"l160:    b l159     \n" \
"l161:    b l164     \n" \
"l162:    b l161     \n" \
"l163:    b l166     \n" \
"l164:    b l163     \n" \
"l165:    b l168     \n" \
"l166:    b l165     \n" \
"l167:    b l170     \n" \
"l168:    b l167     \n" \
"l169:    b l172     \n" \
"l170:    b l169     \n" \
"l171:    b l174     \n" \
"l172:    b l171     \n" \
"l173:    b l176     \n" \
"l174:    b l173     \n" \
"l175:    b l178     \n" \
"l176:    b l175     \n" \
"l177:    b l180     \n" \
"l178:    b l177     \n" \
"l179:    b l182     \n" \
"l180:    b l179     \n" \
"l181:    b l184     \n" \
"l182:    b l181     \n" \
"l183:    b l186     \n" \
"l184:    b l183     \n" \
"l185:    b l188     \n" \
"l186:    b l185     \n" \
"l187:    b l190     \n" \
"l188:    b l187     \n" \
"l189:    b l192     \n" \
"l190:    b l189     \n" \
"l191:    b l194     \n" \
"l192:    b l191     \n" \
"l193:    b l196     \n" \
"l194:    b l193     \n" \
"l195:    b l198     \n" \
"l196:    b l195     \n" \
"l197:    b l200     \n" \
"l198:    b l197     \n" \
"l199:    b l202     \n" \
"l200:    b l199     \n" \
"l201:    b l204     \n" \
"l202:    b l201     \n" \
"l203:    b l206     \n" \
"l204:    b l203     \n" \
"l205:    b l208     \n" \
"l206:    b l205     \n" \
"l207:    b l210     \n" \
"l208:    b l207     \n" \
"l209:    b l212     \n" \
"l210:    b l209     \n" \
"l211:    b l214     \n" \
"l212:    b l211     \n" \
"l213:    b l216     \n" \
"l214:    b l213     \n" \
"l215:    b l218     \n" \
"l216:    b l215     \n" \
"l217:    b l220     \n" \
"l218:    b l217     \n" \
"l219:    b l222     \n" \
"l220:    b l219     \n" \
"l221:    b l224     \n" \
"l222:    b l221     \n" \
"l223:    b l226     \n" \
"l224:    b l223     \n" \
"l225:    b l228     \n" \
"l226:    b l225     \n" \
"l227:    b l230     \n" \
"l228:    b l227     \n" \
"l229:    b l232     \n" \
"l230:    b l229     \n" \
"l231:    b l234     \n" \
"l232:    b l231     \n" \
"l233:    b l236     \n" \
"l234:    b l233     \n" \
"l235:    b l238     \n" \
"l236:    b l235     \n" \
"l237:    b l240     \n" \
"l238:    b l237     \n" \
"l239:    b l242     \n" \
"l240:    b l239     \n" \
"l241:    b l244     \n" \
"l242:    b l241     \n" \
"l243:    b l246     \n" \
"l244:    b l243     \n" \
"l245:    b l248     \n" \
"l246:    b l245     \n" \
"l247:    b l250     \n" \
"l248:    b l247     \n" \
"l249:    b l252     \n" \
"l250:    b l249     \n" \
"l251:    b l254     \n" \
"l252:    b l251     \n" \
"l253:    b l256     \n" \
"l254:    b l253     \n" \
"l255:    b l258     \n" \
"l256:    b l255     \n" \
"l257:    b l260     \n" \
"l258:    b l257     \n" \
"l259:    b l262     \n" \
"l260:    b l259     \n" \
"l261:    b l264     \n" \
"l262:    b l261     \n" \
"l263:    b l266     \n" \
"l264:    b l263     \n" \
"l265:    b l268     \n" \
"l266:    b l265     \n" \
"l267:    b l270     \n" \
"l268:    b l267     \n" \
"l269:    b l272     \n" \
"l270:    b l269     \n" \
"l271:    b l274     \n" \
"l272:    b l271     \n" \
"l273:    b l276     \n" \
"l274:    b l273     \n" \
"l275:    b l278     \n" \
"l276:    b l275     \n" \
"l277:    b l280     \n" \
"l278:    b l277     \n" \
"l279:    b l282     \n" \
"l280:    b l279     \n" \
"l281:    b l284     \n" \
"l282:    b l281     \n" \
"l283:    b l286     \n" \
"l284:    b l283     \n" \
"l285:    b l288     \n" \
"l286:    b l285     \n" \
"l287:    b l290     \n" \
"l288:    b l287     \n" \
"l289:    b l292     \n" \
"l290:    b l289     \n" \
"l291:    b l294     \n" \
"l292:    b l291     \n" \
"l293:    b l296     \n" \
"l294:    b l293     \n" \
"l295:    b l298     \n" \
"l296:    b l295     \n" \
"l297:    b l300     \n" \
"l298:    b l297     \n" \
"l299:    b l302     \n" \
"l300:    b l299     \n" \
"l301:    b l304     \n" \
"l302:    b l301     \n" \
"l303:    b l306     \n" \
"l304:    b l303     \n" \
"l305:    b l308     \n" \
"l306:    b l305     \n" \
"l307:    b l310     \n" \
"l308:    b l307     \n" \
"l309:    b l312     \n" \
"l310:    b l309     \n" \
"l311:    b l314     \n" \
"l312:    b l311     \n" \
"l313:    b l316     \n" \
"l314:    b l313     \n" \
"l315:    b l318     \n" \
"l316:    b l315     \n" \
"l317:    b l320     \n" \
"l318:    b l317     \n" \
"l319:    b l322     \n" \
"l320:    b l319     \n" \
"l321:    b l324     \n" \
"l322:    b l321     \n" \
"l323:    b l326     \n" \
"l324:    b l323     \n" \
"l325:    b l328     \n" \
"l326:    b l325     \n" \
"l327:    b l330     \n" \
"l328:    b l327     \n" \
"l329:    b l332     \n" \
"l330:    b l329     \n" \
"l331:    b l334     \n" \
"l332:    b l331     \n" \
"l333:    b l336     \n" \
"l334:    b l333     \n" \
"l335:    b l338     \n" \
"l336:    b l335     \n" \
"l337:    b l340     \n" \
"l338:    b l337     \n" \
"l339:    b l342     \n" \
"l340:    b l339     \n" \
"l341:    b l344     \n" \
"l342:    b l341     \n" \
"l343:    b l346     \n" \
"l344:    b l343     \n" \
"l345:    b l348     \n" \
"l346:    b l345     \n" \
"l347:    b l350     \n" \
"l348:    b l347     \n" \
"l349:    b l352     \n" \
"l350:    b l349     \n" \
"l351:    b l354     \n" \
"l352:    b l351     \n" \
"l353:    b l356     \n" \
"l354:    b l353     \n" \
"l355:    b l358     \n" \
"l356:    b l355     \n" \
"l357:    b l360     \n" \
"l358:    b l357     \n" \
"l359:    b l362     \n" \
"l360:    b l359     \n" \
"l361:    b l364     \n" \
"l362:    b l361     \n" \
"l363:    b l366     \n" \
"l364:    b l363     \n" \
"l365:    b l368     \n" \
"l366:    b l365     \n" \
"l367:    b l370     \n" \
"l368:    b l367     \n" \
"l369:    b l372     \n" \
"l370:    b l369     \n" \
"l371:    b l374     \n" \
"l372:    b l371     \n" \
"l373:    b l376     \n" \
"l374:    b l373     \n" \
"l375:    b l378     \n" \
"l376:    b l375     \n" \
"l377:    b l380     \n" \
"l378:    b l377     \n" \
"l379:    b l382     \n" \
"l380:    b l379     \n" \
"l381:    b l384     \n" \
"l382:    b l381     \n" \
"l383:    b l386     \n" \
"l384:    b l383     \n" \
"l385:    b l388     \n" \
"l386:    b l385     \n" \
"l387:    b l390     \n" \
"l388:    b l387     \n" \
"l389:    b l392     \n" \
"l390:    b l389     \n" \
"l391:    b l394     \n" \
"l392:    b l391     \n" \
"l393:    b l396     \n" \
"l394:    b l393     \n" \
"l395:    b l398     \n" \
"l396:    b l395     \n" \
"l397:    b l400     \n" \
"l398:    b l397     \n" \
"l399:    b l402     \n" \
"l400:    b l399     \n" \
"l401:    b l404     \n" \
"l402:    b l401     \n" \
"l403:    b l406     \n" \
"l404:    b l403     \n" \
"l405:    b l408     \n" \
"l406:    b l405     \n" \
"l407:    b l410     \n" \
"l408:    b l407     \n" \
"l409:    b l412     \n" \
"l410:    b l409     \n" \
"l411:    b l414     \n" \
"l412:    b l411     \n" \
"l413:    b l416     \n" \
"l414:    b l413     \n" \
"l415:    b l418     \n" \
"l416:    b l415     \n" \
"l417:    b l420     \n" \
"l418:    b l417     \n" \
"l419:    b l422     \n" \
"l420:    b l419     \n" \
"l421:    b l424     \n" \
"l422:    b l421     \n" \
"l423:    b l426     \n" \
"l424:    b l423     \n" \
"l425:    b l428     \n" \
"l426:    b l425     \n" \
"l427:    b l430     \n" \
"l428:    b l427     \n" \
"l429:    b l432     \n" \
"l430:    b l429     \n" \
"l431:    b l434     \n" \
"l432:    b l431     \n" \
"l433:    b l436     \n" \
"l434:    b l433     \n" \
"l435:    b l438     \n" \
"l436:    b l435     \n" \
"l437:    b l440     \n" \
"l438:    b l437     \n" \
"l439:    b l442     \n" \
"l440:    b l439     \n" \
"l441:    b l444     \n" \
"l442:    b l441     \n" \
"l443:    b l446     \n" \
"l444:    b l443     \n" \
"l445:    b l448     \n" \
"l446:    b l445     \n" \
"l447:    b l450     \n" \
"l448:    b l447     \n" \
"l449:    b l452     \n" \
"l450:    b l449     \n" \
"l451:    b l454     \n" \
"l452:    b l451     \n" \
"l453:    b l456     \n" \
"l454:    b l453     \n" \
"l455:    b l458     \n" \
"l456:    b l455     \n" \
"l457:    b l460     \n" \
"l458:    b l457     \n" \
"l459:    b l462     \n" \
"l460:    b l459     \n" \
"l461:    b l464     \n" \
"l462:    b l461     \n" \
"l463:    b l466     \n" \
"l464:    b l463     \n" \
"l465:    b l468     \n" \
"l466:    b l465     \n" \
"l467:    b l470     \n" \
"l468:    b l467     \n" \
"l469:    b l472     \n" \
"l470:    b l469     \n" \
"l471:    b l474     \n" \
"l472:    b l471     \n" \
"l473:    b l476     \n" \
"l474:    b l473     \n" \
"l475:    b l478     \n" \
"l476:    b l475     \n" \
"l477:    b l480     \n" \
"l478:    b l477     \n" \
"l479:    b l482     \n" \
"l480:    b l479     \n" \
"l481:    b l484     \n" \
"l482:    b l481     \n" \
"l483:    b l486     \n" \
"l484:    b l483     \n" \
"l485:    b l488     \n" \
"l486:    b l485     \n" \
"l487:    b l490     \n" \
"l488:    b l487     \n" \
"l489:    b l492     \n" \
"l490:    b l489     \n" \
"l491:    b l494     \n" \
"l492:    b l491     \n" \
"l493:    b l496     \n" \
"l494:    b l493     \n" \
"l495:    b l498     \n" \
"l496:    b l495     \n" \
"l497:    b l500     \n" \
"l498:    b l497     \n" \
"l499:    b l502     \n" \
"l500:    b l499     \n" \
"l501:    b l504     \n" \
"l502:    b l501     \n" \
"l503:    b l506     \n" \
"l504:    b l503     \n" \
"l505:    b l508     \n" \
"l506:    b l505     \n" \
"l507:    b l510     \n" \
"l508:    b l507     \n" \
"l509:    b l511     \n" \
"l510:    b l509     \n" \
"l511:   \n"

#define BRANCH_SANITIZE_1024 "l0:      b l2       \n" \
"l1:      b l4       \n" \
"l2:      b l1       \n" \
"l3:      b l6       \n" \
"l4:      b l3       \n" \
"l5:      b l8       \n" \
"l6:      b l5       \n" \
"l7:      b l10      \n" \
"l8:      b l7       \n" \
"l9:      b l12      \n" \
"l10:     b l9       \n" \
"l11:     b l14      \n" \
"l12:     b l11      \n" \
"l13:     b l16      \n" \
"l14:     b l13      \n" \
"l15:     b l18      \n" \
"l16:     b l15      \n" \
"l17:     b l20      \n" \
"l18:     b l17      \n" \
"l19:     b l22      \n" \
"l20:     b l19      \n" \
"l21:     b l24      \n" \
"l22:     b l21      \n" \
"l23:     b l26      \n" \
"l24:     b l23      \n" \
"l25:     b l28      \n" \
"l26:     b l25      \n" \
"l27:     b l30      \n" \
"l28:     b l27      \n" \
"l29:     b l32      \n" \
"l30:     b l29      \n" \
"l31:     b l34      \n" \
"l32:     b l31      \n" \
"l33:     b l36      \n" \
"l34:     b l33      \n" \
"l35:     b l38      \n" \
"l36:     b l35      \n" \
"l37:     b l40      \n" \
"l38:     b l37      \n" \
"l39:     b l42      \n" \
"l40:     b l39      \n" \
"l41:     b l44      \n" \
"l42:     b l41      \n" \
"l43:     b l46      \n" \
"l44:     b l43      \n" \
"l45:     b l48      \n" \
"l46:     b l45      \n" \
"l47:     b l50      \n" \
"l48:     b l47      \n" \
"l49:     b l52      \n" \
"l50:     b l49      \n" \
"l51:     b l54      \n" \
"l52:     b l51      \n" \
"l53:     b l56      \n" \
"l54:     b l53      \n" \
"l55:     b l58      \n" \
"l56:     b l55      \n" \
"l57:     b l60      \n" \
"l58:     b l57      \n" \
"l59:     b l62      \n" \
"l60:     b l59      \n" \
"l61:     b l64      \n" \
"l62:     b l61      \n" \
"l63:     b l66      \n" \
"l64:     b l63      \n" \
"l65:     b l68      \n" \
"l66:     b l65      \n" \
"l67:     b l70      \n" \
"l68:     b l67      \n" \
"l69:     b l72      \n" \
"l70:     b l69      \n" \
"l71:     b l74      \n" \
"l72:     b l71      \n" \
"l73:     b l76      \n" \
"l74:     b l73      \n" \
"l75:     b l78      \n" \
"l76:     b l75      \n" \
"l77:     b l80      \n" \
"l78:     b l77      \n" \
"l79:     b l82      \n" \
"l80:     b l79      \n" \
"l81:     b l84      \n" \
"l82:     b l81      \n" \
"l83:     b l86      \n" \
"l84:     b l83      \n" \
"l85:     b l88      \n" \
"l86:     b l85      \n" \
"l87:     b l90      \n" \
"l88:     b l87      \n" \
"l89:     b l92      \n" \
"l90:     b l89      \n" \
"l91:     b l94      \n" \
"l92:     b l91      \n" \
"l93:     b l96      \n" \
"l94:     b l93      \n" \
"l95:     b l98      \n" \
"l96:     b l95      \n" \
"l97:     b l100     \n" \
"l98:     b l97      \n" \
"l99:     b l102     \n" \
"l100:    b l99      \n" \
"l101:    b l104     \n" \
"l102:    b l101     \n" \
"l103:    b l106     \n" \
"l104:    b l103     \n" \
"l105:    b l108     \n" \
"l106:    b l105     \n" \
"l107:    b l110     \n" \
"l108:    b l107     \n" \
"l109:    b l112     \n" \
"l110:    b l109     \n" \
"l111:    b l114     \n" \
"l112:    b l111     \n" \
"l113:    b l116     \n" \
"l114:    b l113     \n" \
"l115:    b l118     \n" \
"l116:    b l115     \n" \
"l117:    b l120     \n" \
"l118:    b l117     \n" \
"l119:    b l122     \n" \
"l120:    b l119     \n" \
"l121:    b l124     \n" \
"l122:    b l121     \n" \
"l123:    b l126     \n" \
"l124:    b l123     \n" \
"l125:    b l128     \n" \
"l126:    b l125     \n" \
"l127:    b l130     \n" \
"l128:    b l127     \n" \
"l129:    b l132     \n" \
"l130:    b l129     \n" \
"l131:    b l134     \n" \
"l132:    b l131     \n" \
"l133:    b l136     \n" \
"l134:    b l133     \n" \
"l135:    b l138     \n" \
"l136:    b l135     \n" \
"l137:    b l140     \n" \
"l138:    b l137     \n" \
"l139:    b l142     \n" \
"l140:    b l139     \n" \
"l141:    b l144     \n" \
"l142:    b l141     \n" \
"l143:    b l146     \n" \
"l144:    b l143     \n" \
"l145:    b l148     \n" \
"l146:    b l145     \n" \
"l147:    b l150     \n" \
"l148:    b l147     \n" \
"l149:    b l152     \n" \
"l150:    b l149     \n" \
"l151:    b l154     \n" \
"l152:    b l151     \n" \
"l153:    b l156     \n" \
"l154:    b l153     \n" \
"l155:    b l158     \n" \
"l156:    b l155     \n" \
"l157:    b l160     \n" \
"l158:    b l157     \n" \
"l159:    b l162     \n" \
"l160:    b l159     \n" \
"l161:    b l164     \n" \
"l162:    b l161     \n" \
"l163:    b l166     \n" \
"l164:    b l163     \n" \
"l165:    b l168     \n" \
"l166:    b l165     \n" \
"l167:    b l170     \n" \
"l168:    b l167     \n" \
"l169:    b l172     \n" \
"l170:    b l169     \n" \
"l171:    b l174     \n" \
"l172:    b l171     \n" \
"l173:    b l176     \n" \
"l174:    b l173     \n" \
"l175:    b l178     \n" \
"l176:    b l175     \n" \
"l177:    b l180     \n" \
"l178:    b l177     \n" \
"l179:    b l182     \n" \
"l180:    b l179     \n" \
"l181:    b l184     \n" \
"l182:    b l181     \n" \
"l183:    b l186     \n" \
"l184:    b l183     \n" \
"l185:    b l188     \n" \
"l186:    b l185     \n" \
"l187:    b l190     \n" \
"l188:    b l187     \n" \
"l189:    b l192     \n" \
"l190:    b l189     \n" \
"l191:    b l194     \n" \
"l192:    b l191     \n" \
"l193:    b l196     \n" \
"l194:    b l193     \n" \
"l195:    b l198     \n" \
"l196:    b l195     \n" \
"l197:    b l200     \n" \
"l198:    b l197     \n" \
"l199:    b l202     \n" \
"l200:    b l199     \n" \
"l201:    b l204     \n" \
"l202:    b l201     \n" \
"l203:    b l206     \n" \
"l204:    b l203     \n" \
"l205:    b l208     \n" \
"l206:    b l205     \n" \
"l207:    b l210     \n" \
"l208:    b l207     \n" \
"l209:    b l212     \n" \
"l210:    b l209     \n" \
"l211:    b l214     \n" \
"l212:    b l211     \n" \
"l213:    b l216     \n" \
"l214:    b l213     \n" \
"l215:    b l218     \n" \
"l216:    b l215     \n" \
"l217:    b l220     \n" \
"l218:    b l217     \n" \
"l219:    b l222     \n" \
"l220:    b l219     \n" \
"l221:    b l224     \n" \
"l222:    b l221     \n" \
"l223:    b l226     \n" \
"l224:    b l223     \n" \
"l225:    b l228     \n" \
"l226:    b l225     \n" \
"l227:    b l230     \n" \
"l228:    b l227     \n" \
"l229:    b l232     \n" \
"l230:    b l229     \n" \
"l231:    b l234     \n" \
"l232:    b l231     \n" \
"l233:    b l236     \n" \
"l234:    b l233     \n" \
"l235:    b l238     \n" \
"l236:    b l235     \n" \
"l237:    b l240     \n" \
"l238:    b l237     \n" \
"l239:    b l242     \n" \
"l240:    b l239     \n" \
"l241:    b l244     \n" \
"l242:    b l241     \n" \
"l243:    b l246     \n" \
"l244:    b l243     \n" \
"l245:    b l248     \n" \
"l246:    b l245     \n" \
"l247:    b l250     \n" \
"l248:    b l247     \n" \
"l249:    b l252     \n" \
"l250:    b l249     \n" \
"l251:    b l254     \n" \
"l252:    b l251     \n" \
"l253:    b l256     \n" \
"l254:    b l253     \n" \
"l255:    b l258     \n" \
"l256:    b l255     \n" \
"l257:    b l260     \n" \
"l258:    b l257     \n" \
"l259:    b l262     \n" \
"l260:    b l259     \n" \
"l261:    b l264     \n" \
"l262:    b l261     \n" \
"l263:    b l266     \n" \
"l264:    b l263     \n" \
"l265:    b l268     \n" \
"l266:    b l265     \n" \
"l267:    b l270     \n" \
"l268:    b l267     \n" \
"l269:    b l272     \n" \
"l270:    b l269     \n" \
"l271:    b l274     \n" \
"l272:    b l271     \n" \
"l273:    b l276     \n" \
"l274:    b l273     \n" \
"l275:    b l278     \n" \
"l276:    b l275     \n" \
"l277:    b l280     \n" \
"l278:    b l277     \n" \
"l279:    b l282     \n" \
"l280:    b l279     \n" \
"l281:    b l284     \n" \
"l282:    b l281     \n" \
"l283:    b l286     \n" \
"l284:    b l283     \n" \
"l285:    b l288     \n" \
"l286:    b l285     \n" \
"l287:    b l290     \n" \
"l288:    b l287     \n" \
"l289:    b l292     \n" \
"l290:    b l289     \n" \
"l291:    b l294     \n" \
"l292:    b l291     \n" \
"l293:    b l296     \n" \
"l294:    b l293     \n" \
"l295:    b l298     \n" \
"l296:    b l295     \n" \
"l297:    b l300     \n" \
"l298:    b l297     \n" \
"l299:    b l302     \n" \
"l300:    b l299     \n" \
"l301:    b l304     \n" \
"l302:    b l301     \n" \
"l303:    b l306     \n" \
"l304:    b l303     \n" \
"l305:    b l308     \n" \
"l306:    b l305     \n" \
"l307:    b l310     \n" \
"l308:    b l307     \n" \
"l309:    b l312     \n" \
"l310:    b l309     \n" \
"l311:    b l314     \n" \
"l312:    b l311     \n" \
"l313:    b l316     \n" \
"l314:    b l313     \n" \
"l315:    b l318     \n" \
"l316:    b l315     \n" \
"l317:    b l320     \n" \
"l318:    b l317     \n" \
"l319:    b l322     \n" \
"l320:    b l319     \n" \
"l321:    b l324     \n" \
"l322:    b l321     \n" \
"l323:    b l326     \n" \
"l324:    b l323     \n" \
"l325:    b l328     \n" \
"l326:    b l325     \n" \
"l327:    b l330     \n" \
"l328:    b l327     \n" \
"l329:    b l332     \n" \
"l330:    b l329     \n" \
"l331:    b l334     \n" \
"l332:    b l331     \n" \
"l333:    b l336     \n" \
"l334:    b l333     \n" \
"l335:    b l338     \n" \
"l336:    b l335     \n" \
"l337:    b l340     \n" \
"l338:    b l337     \n" \
"l339:    b l342     \n" \
"l340:    b l339     \n" \
"l341:    b l344     \n" \
"l342:    b l341     \n" \
"l343:    b l346     \n" \
"l344:    b l343     \n" \
"l345:    b l348     \n" \
"l346:    b l345     \n" \
"l347:    b l350     \n" \
"l348:    b l347     \n" \
"l349:    b l352     \n" \
"l350:    b l349     \n" \
"l351:    b l354     \n" \
"l352:    b l351     \n" \
"l353:    b l356     \n" \
"l354:    b l353     \n" \
"l355:    b l358     \n" \
"l356:    b l355     \n" \
"l357:    b l360     \n" \
"l358:    b l357     \n" \
"l359:    b l362     \n" \
"l360:    b l359     \n" \
"l361:    b l364     \n" \
"l362:    b l361     \n" \
"l363:    b l366     \n" \
"l364:    b l363     \n" \
"l365:    b l368     \n" \
"l366:    b l365     \n" \
"l367:    b l370     \n" \
"l368:    b l367     \n" \
"l369:    b l372     \n" \
"l370:    b l369     \n" \
"l371:    b l374     \n" \
"l372:    b l371     \n" \
"l373:    b l376     \n" \
"l374:    b l373     \n" \
"l375:    b l378     \n" \
"l376:    b l375     \n" \
"l377:    b l380     \n" \
"l378:    b l377     \n" \
"l379:    b l382     \n" \
"l380:    b l379     \n" \
"l381:    b l384     \n" \
"l382:    b l381     \n" \
"l383:    b l386     \n" \
"l384:    b l383     \n" \
"l385:    b l388     \n" \
"l386:    b l385     \n" \
"l387:    b l390     \n" \
"l388:    b l387     \n" \
"l389:    b l392     \n" \
"l390:    b l389     \n" \
"l391:    b l394     \n" \
"l392:    b l391     \n" \
"l393:    b l396     \n" \
"l394:    b l393     \n" \
"l395:    b l398     \n" \
"l396:    b l395     \n" \
"l397:    b l400     \n" \
"l398:    b l397     \n" \
"l399:    b l402     \n" \
"l400:    b l399     \n" \
"l401:    b l404     \n" \
"l402:    b l401     \n" \
"l403:    b l406     \n" \
"l404:    b l403     \n" \
"l405:    b l408     \n" \
"l406:    b l405     \n" \
"l407:    b l410     \n" \
"l408:    b l407     \n" \
"l409:    b l412     \n" \
"l410:    b l409     \n" \
"l411:    b l414     \n" \
"l412:    b l411     \n" \
"l413:    b l416     \n" \
"l414:    b l413     \n" \
"l415:    b l418     \n" \
"l416:    b l415     \n" \
"l417:    b l420     \n" \
"l418:    b l417     \n" \
"l419:    b l422     \n" \
"l420:    b l419     \n" \
"l421:    b l424     \n" \
"l422:    b l421     \n" \
"l423:    b l426     \n" \
"l424:    b l423     \n" \
"l425:    b l428     \n" \
"l426:    b l425     \n" \
"l427:    b l430     \n" \
"l428:    b l427     \n" \
"l429:    b l432     \n" \
"l430:    b l429     \n" \
"l431:    b l434     \n" \
"l432:    b l431     \n" \
"l433:    b l436     \n" \
"l434:    b l433     \n" \
"l435:    b l438     \n" \
"l436:    b l435     \n" \
"l437:    b l440     \n" \
"l438:    b l437     \n" \
"l439:    b l442     \n" \
"l440:    b l439     \n" \
"l441:    b l444     \n" \
"l442:    b l441     \n" \
"l443:    b l446     \n" \
"l444:    b l443     \n" \
"l445:    b l448     \n" \
"l446:    b l445     \n" \
"l447:    b l450     \n" \
"l448:    b l447     \n" \
"l449:    b l452     \n" \
"l450:    b l449     \n" \
"l451:    b l454     \n" \
"l452:    b l451     \n" \
"l453:    b l456     \n" \
"l454:    b l453     \n" \
"l455:    b l458     \n" \
"l456:    b l455     \n" \
"l457:    b l460     \n" \
"l458:    b l457     \n" \
"l459:    b l462     \n" \
"l460:    b l459     \n" \
"l461:    b l464     \n" \
"l462:    b l461     \n" \
"l463:    b l466     \n" \
"l464:    b l463     \n" \
"l465:    b l468     \n" \
"l466:    b l465     \n" \
"l467:    b l470     \n" \
"l468:    b l467     \n" \
"l469:    b l472     \n" \
"l470:    b l469     \n" \
"l471:    b l474     \n" \
"l472:    b l471     \n" \
"l473:    b l476     \n" \
"l474:    b l473     \n" \
"l475:    b l478     \n" \
"l476:    b l475     \n" \
"l477:    b l480     \n" \
"l478:    b l477     \n" \
"l479:    b l482     \n" \
"l480:    b l479     \n" \
"l481:    b l484     \n" \
"l482:    b l481     \n" \
"l483:    b l486     \n" \
"l484:    b l483     \n" \
"l485:    b l488     \n" \
"l486:    b l485     \n" \
"l487:    b l490     \n" \
"l488:    b l487     \n" \
"l489:    b l492     \n" \
"l490:    b l489     \n" \
"l491:    b l494     \n" \
"l492:    b l491     \n" \
"l493:    b l496     \n" \
"l494:    b l493     \n" \
"l495:    b l498     \n" \
"l496:    b l495     \n" \
"l497:    b l500     \n" \
"l498:    b l497     \n" \
"l499:    b l502     \n" \
"l500:    b l499     \n" \
"l501:    b l504     \n" \
"l502:    b l501     \n" \
"l503:    b l506     \n" \
"l504:    b l503     \n" \
"l505:    b l508     \n" \
"l506:    b l505     \n" \
"l507:    b l510     \n" \
"l508:    b l507     \n" \
"l509:    b l512     \n" \
"l510:    b l509     \n" \
"l511:    b l514     \n" \
"l512:    b l511     \n" \
"l513:    b l516     \n" \
"l514:    b l513     \n" \
"l515:    b l518     \n" \
"l516:    b l515     \n" \
"l517:    b l520     \n" \
"l518:    b l517     \n" \
"l519:    b l522     \n" \
"l520:    b l519     \n" \
"l521:    b l524     \n" \
"l522:    b l521     \n" \
"l523:    b l526     \n" \
"l524:    b l523     \n" \
"l525:    b l528     \n" \
"l526:    b l525     \n" \
"l527:    b l530     \n" \
"l528:    b l527     \n" \
"l529:    b l532     \n" \
"l530:    b l529     \n" \
"l531:    b l534     \n" \
"l532:    b l531     \n" \
"l533:    b l536     \n" \
"l534:    b l533     \n" \
"l535:    b l538     \n" \
"l536:    b l535     \n" \
"l537:    b l540     \n" \
"l538:    b l537     \n" \
"l539:    b l542     \n" \
"l540:    b l539     \n" \
"l541:    b l544     \n" \
"l542:    b l541     \n" \
"l543:    b l546     \n" \
"l544:    b l543     \n" \
"l545:    b l548     \n" \
"l546:    b l545     \n" \
"l547:    b l550     \n" \
"l548:    b l547     \n" \
"l549:    b l552     \n" \
"l550:    b l549     \n" \
"l551:    b l554     \n" \
"l552:    b l551     \n" \
"l553:    b l556     \n" \
"l554:    b l553     \n" \
"l555:    b l558     \n" \
"l556:    b l555     \n" \
"l557:    b l560     \n" \
"l558:    b l557     \n" \
"l559:    b l562     \n" \
"l560:    b l559     \n" \
"l561:    b l564     \n" \
"l562:    b l561     \n" \
"l563:    b l566     \n" \
"l564:    b l563     \n" \
"l565:    b l568     \n" \
"l566:    b l565     \n" \
"l567:    b l570     \n" \
"l568:    b l567     \n" \
"l569:    b l572     \n" \
"l570:    b l569     \n" \
"l571:    b l574     \n" \
"l572:    b l571     \n" \
"l573:    b l576     \n" \
"l574:    b l573     \n" \
"l575:    b l578     \n" \
"l576:    b l575     \n" \
"l577:    b l580     \n" \
"l578:    b l577     \n" \
"l579:    b l582     \n" \
"l580:    b l579     \n" \
"l581:    b l584     \n" \
"l582:    b l581     \n" \
"l583:    b l586     \n" \
"l584:    b l583     \n" \
"l585:    b l588     \n" \
"l586:    b l585     \n" \
"l587:    b l590     \n" \
"l588:    b l587     \n" \
"l589:    b l592     \n" \
"l590:    b l589     \n" \
"l591:    b l594     \n" \
"l592:    b l591     \n" \
"l593:    b l596     \n" \
"l594:    b l593     \n" \
"l595:    b l598     \n" \
"l596:    b l595     \n" \
"l597:    b l600     \n" \
"l598:    b l597     \n" \
"l599:    b l602     \n" \
"l600:    b l599     \n" \
"l601:    b l604     \n" \
"l602:    b l601     \n" \
"l603:    b l606     \n" \
"l604:    b l603     \n" \
"l605:    b l608     \n" \
"l606:    b l605     \n" \
"l607:    b l610     \n" \
"l608:    b l607     \n" \
"l609:    b l612     \n" \
"l610:    b l609     \n" \
"l611:    b l614     \n" \
"l612:    b l611     \n" \
"l613:    b l616     \n" \
"l614:    b l613     \n" \
"l615:    b l618     \n" \
"l616:    b l615     \n" \
"l617:    b l620     \n" \
"l618:    b l617     \n" \
"l619:    b l622     \n" \
"l620:    b l619     \n" \
"l621:    b l624     \n" \
"l622:    b l621     \n" \
"l623:    b l626     \n" \
"l624:    b l623     \n" \
"l625:    b l628     \n" \
"l626:    b l625     \n" \
"l627:    b l630     \n" \
"l628:    b l627     \n" \
"l629:    b l632     \n" \
"l630:    b l629     \n" \
"l631:    b l634     \n" \
"l632:    b l631     \n" \
"l633:    b l636     \n" \
"l634:    b l633     \n" \
"l635:    b l638     \n" \
"l636:    b l635     \n" \
"l637:    b l640     \n" \
"l638:    b l637     \n" \
"l639:    b l642     \n" \
"l640:    b l639     \n" \
"l641:    b l644     \n" \
"l642:    b l641     \n" \
"l643:    b l646     \n" \
"l644:    b l643     \n" \
"l645:    b l648     \n" \
"l646:    b l645     \n" \
"l647:    b l650     \n" \
"l648:    b l647     \n" \
"l649:    b l652     \n" \
"l650:    b l649     \n" \
"l651:    b l654     \n" \
"l652:    b l651     \n" \
"l653:    b l656     \n" \
"l654:    b l653     \n" \
"l655:    b l658     \n" \
"l656:    b l655     \n" \
"l657:    b l660     \n" \
"l658:    b l657     \n" \
"l659:    b l662     \n" \
"l660:    b l659     \n" \
"l661:    b l664     \n" \
"l662:    b l661     \n" \
"l663:    b l666     \n" \
"l664:    b l663     \n" \
"l665:    b l668     \n" \
"l666:    b l665     \n" \
"l667:    b l670     \n" \
"l668:    b l667     \n" \
"l669:    b l672     \n" \
"l670:    b l669     \n" \
"l671:    b l674     \n" \
"l672:    b l671     \n" \
"l673:    b l676     \n" \
"l674:    b l673     \n" \
"l675:    b l678     \n" \
"l676:    b l675     \n" \
"l677:    b l680     \n" \
"l678:    b l677     \n" \
"l679:    b l682     \n" \
"l680:    b l679     \n" \
"l681:    b l684     \n" \
"l682:    b l681     \n" \
"l683:    b l686     \n" \
"l684:    b l683     \n" \
"l685:    b l688     \n" \
"l686:    b l685     \n" \
"l687:    b l690     \n" \
"l688:    b l687     \n" \
"l689:    b l692     \n" \
"l690:    b l689     \n" \
"l691:    b l694     \n" \
"l692:    b l691     \n" \
"l693:    b l696     \n" \
"l694:    b l693     \n" \
"l695:    b l698     \n" \
"l696:    b l695     \n" \
"l697:    b l700     \n" \
"l698:    b l697     \n" \
"l699:    b l702     \n" \
"l700:    b l699     \n" \
"l701:    b l704     \n" \
"l702:    b l701     \n" \
"l703:    b l706     \n" \
"l704:    b l703     \n" \
"l705:    b l708     \n" \
"l706:    b l705     \n" \
"l707:    b l710     \n" \
"l708:    b l707     \n" \
"l709:    b l712     \n" \
"l710:    b l709     \n" \
"l711:    b l714     \n" \
"l712:    b l711     \n" \
"l713:    b l716     \n" \
"l714:    b l713     \n" \
"l715:    b l718     \n" \
"l716:    b l715     \n" \
"l717:    b l720     \n" \
"l718:    b l717     \n" \
"l719:    b l722     \n" \
"l720:    b l719     \n" \
"l721:    b l724     \n" \
"l722:    b l721     \n" \
"l723:    b l726     \n" \
"l724:    b l723     \n" \
"l725:    b l728     \n" \
"l726:    b l725     \n" \
"l727:    b l730     \n" \
"l728:    b l727     \n" \
"l729:    b l732     \n" \
"l730:    b l729     \n" \
"l731:    b l734     \n" \
"l732:    b l731     \n" \
"l733:    b l736     \n" \
"l734:    b l733     \n" \
"l735:    b l738     \n" \
"l736:    b l735     \n" \
"l737:    b l740     \n" \
"l738:    b l737     \n" \
"l739:    b l742     \n" \
"l740:    b l739     \n" \
"l741:    b l744     \n" \
"l742:    b l741     \n" \
"l743:    b l746     \n" \
"l744:    b l743     \n" \
"l745:    b l748     \n" \
"l746:    b l745     \n" \
"l747:    b l750     \n" \
"l748:    b l747     \n" \
"l749:    b l752     \n" \
"l750:    b l749     \n" \
"l751:    b l754     \n" \
"l752:    b l751     \n" \
"l753:    b l756     \n" \
"l754:    b l753     \n" \
"l755:    b l758     \n" \
"l756:    b l755     \n" \
"l757:    b l760     \n" \
"l758:    b l757     \n" \
"l759:    b l762     \n" \
"l760:    b l759     \n" \
"l761:    b l764     \n" \
"l762:    b l761     \n" \
"l763:    b l766     \n" \
"l764:    b l763     \n" \
"l765:    b l768     \n" \
"l766:    b l765     \n" \
"l767:    b l770     \n" \
"l768:    b l767     \n" \
"l769:    b l772     \n" \
"l770:    b l769     \n" \
"l771:    b l774     \n" \
"l772:    b l771     \n" \
"l773:    b l776     \n" \
"l774:    b l773     \n" \
"l775:    b l778     \n" \
"l776:    b l775     \n" \
"l777:    b l780     \n" \
"l778:    b l777     \n" \
"l779:    b l782     \n" \
"l780:    b l779     \n" \
"l781:    b l784     \n" \
"l782:    b l781     \n" \
"l783:    b l786     \n" \
"l784:    b l783     \n" \
"l785:    b l788     \n" \
"l786:    b l785     \n" \
"l787:    b l790     \n" \
"l788:    b l787     \n" \
"l789:    b l792     \n" \
"l790:    b l789     \n" \
"l791:    b l794     \n" \
"l792:    b l791     \n" \
"l793:    b l796     \n" \
"l794:    b l793     \n" \
"l795:    b l798     \n" \
"l796:    b l795     \n" \
"l797:    b l800     \n" \
"l798:    b l797     \n" \
"l799:    b l802     \n" \
"l800:    b l799     \n" \
"l801:    b l804     \n" \
"l802:    b l801     \n" \
"l803:    b l806     \n" \
"l804:    b l803     \n" \
"l805:    b l808     \n" \
"l806:    b l805     \n" \
"l807:    b l810     \n" \
"l808:    b l807     \n" \
"l809:    b l812     \n" \
"l810:    b l809     \n" \
"l811:    b l814     \n" \
"l812:    b l811     \n" \
"l813:    b l816     \n" \
"l814:    b l813     \n" \
"l815:    b l818     \n" \
"l816:    b l815     \n" \
"l817:    b l820     \n" \
"l818:    b l817     \n" \
"l819:    b l822     \n" \
"l820:    b l819     \n" \
"l821:    b l824     \n" \
"l822:    b l821     \n" \
"l823:    b l826     \n" \
"l824:    b l823     \n" \
"l825:    b l828     \n" \
"l826:    b l825     \n" \
"l827:    b l830     \n" \
"l828:    b l827     \n" \
"l829:    b l832     \n" \
"l830:    b l829     \n" \
"l831:    b l834     \n" \
"l832:    b l831     \n" \
"l833:    b l836     \n" \
"l834:    b l833     \n" \
"l835:    b l838     \n" \
"l836:    b l835     \n" \
"l837:    b l840     \n" \
"l838:    b l837     \n" \
"l839:    b l842     \n" \
"l840:    b l839     \n" \
"l841:    b l844     \n" \
"l842:    b l841     \n" \
"l843:    b l846     \n" \
"l844:    b l843     \n" \
"l845:    b l848     \n" \
"l846:    b l845     \n" \
"l847:    b l850     \n" \
"l848:    b l847     \n" \
"l849:    b l852     \n" \
"l850:    b l849     \n" \
"l851:    b l854     \n" \
"l852:    b l851     \n" \
"l853:    b l856     \n" \
"l854:    b l853     \n" \
"l855:    b l858     \n" \
"l856:    b l855     \n" \
"l857:    b l860     \n" \
"l858:    b l857     \n" \
"l859:    b l862     \n" \
"l860:    b l859     \n" \
"l861:    b l864     \n" \
"l862:    b l861     \n" \
"l863:    b l866     \n" \
"l864:    b l863     \n" \
"l865:    b l868     \n" \
"l866:    b l865     \n" \
"l867:    b l870     \n" \
"l868:    b l867     \n" \
"l869:    b l872     \n" \
"l870:    b l869     \n" \
"l871:    b l874     \n" \
"l872:    b l871     \n" \
"l873:    b l876     \n" \
"l874:    b l873     \n" \
"l875:    b l878     \n" \
"l876:    b l875     \n" \
"l877:    b l880     \n" \
"l878:    b l877     \n" \
"l879:    b l882     \n" \
"l880:    b l879     \n" \
"l881:    b l884     \n" \
"l882:    b l881     \n" \
"l883:    b l886     \n" \
"l884:    b l883     \n" \
"l885:    b l888     \n" \
"l886:    b l885     \n" \
"l887:    b l890     \n" \
"l888:    b l887     \n" \
"l889:    b l892     \n" \
"l890:    b l889     \n" \
"l891:    b l894     \n" \
"l892:    b l891     \n" \
"l893:    b l896     \n" \
"l894:    b l893     \n" \
"l895:    b l898     \n" \
"l896:    b l895     \n" \
"l897:    b l900     \n" \
"l898:    b l897     \n" \
"l899:    b l902     \n" \
"l900:    b l899     \n" \
"l901:    b l904     \n" \
"l902:    b l901     \n" \
"l903:    b l906     \n" \
"l904:    b l903     \n" \
"l905:    b l908     \n" \
"l906:    b l905     \n" \
"l907:    b l910     \n" \
"l908:    b l907     \n" \
"l909:    b l912     \n" \
"l910:    b l909     \n" \
"l911:    b l914     \n" \
"l912:    b l911     \n" \
"l913:    b l916     \n" \
"l914:    b l913     \n" \
"l915:    b l918     \n" \
"l916:    b l915     \n" \
"l917:    b l920     \n" \
"l918:    b l917     \n" \
"l919:    b l922     \n" \
"l920:    b l919     \n" \
"l921:    b l924     \n" \
"l922:    b l921     \n" \
"l923:    b l926     \n" \
"l924:    b l923     \n" \
"l925:    b l928     \n" \
"l926:    b l925     \n" \
"l927:    b l930     \n" \
"l928:    b l927     \n" \
"l929:    b l932     \n" \
"l930:    b l929     \n" \
"l931:    b l934     \n" \
"l932:    b l931     \n" \
"l933:    b l936     \n" \
"l934:    b l933     \n" \
"l935:    b l938     \n" \
"l936:    b l935     \n" \
"l937:    b l940     \n" \
"l938:    b l937     \n" \
"l939:    b l942     \n" \
"l940:    b l939     \n" \
"l941:    b l944     \n" \
"l942:    b l941     \n" \
"l943:    b l946     \n" \
"l944:    b l943     \n" \
"l945:    b l948     \n" \
"l946:    b l945     \n" \
"l947:    b l950     \n" \
"l948:    b l947     \n" \
"l949:    b l952     \n" \
"l950:    b l949     \n" \
"l951:    b l954     \n" \
"l952:    b l951     \n" \
"l953:    b l956     \n" \
"l954:    b l953     \n" \
"l955:    b l958     \n" \
"l956:    b l955     \n" \
"l957:    b l960     \n" \
"l958:    b l957     \n" \
"l959:    b l962     \n" \
"l960:    b l959     \n" \
"l961:    b l964     \n" \
"l962:    b l961     \n" \
"l963:    b l966     \n" \
"l964:    b l963     \n" \
"l965:    b l968     \n" \
"l966:    b l965     \n" \
"l967:    b l970     \n" \
"l968:    b l967     \n" \
"l969:    b l972     \n" \
"l970:    b l969     \n" \
"l971:    b l974     \n" \
"l972:    b l971     \n" \
"l973:    b l976     \n" \
"l974:    b l973     \n" \
"l975:    b l978     \n" \
"l976:    b l975     \n" \
"l977:    b l980     \n" \
"l978:    b l977     \n" \
"l979:    b l982     \n" \
"l980:    b l979     \n" \
"l981:    b l984     \n" \
"l982:    b l981     \n" \
"l983:    b l986     \n" \
"l984:    b l983     \n" \
"l985:    b l988     \n" \
"l986:    b l985     \n" \
"l987:    b l990     \n" \
"l988:    b l987     \n" \
"l989:    b l992     \n" \
"l990:    b l989     \n" \
"l991:    b l994     \n" \
"l992:    b l991     \n" \
"l993:    b l996     \n" \
"l994:    b l993     \n" \
"l995:    b l998     \n" \
"l996:    b l995     \n" \
"l997:    b l1000    \n" \
"l998:    b l997     \n" \
"l999:    b l1002    \n" \
"l1000:   b l999     \n" \
"l1001:   b l1004    \n" \
"l1002:   b l1001    \n" \
"l1003:   b l1006    \n" \
"l1004:   b l1003    \n" \
"l1005:   b l1008    \n" \
"l1006:   b l1005    \n" \
"l1007:   b l1010    \n" \
"l1008:   b l1007    \n" \
"l1009:   b l1012    \n" \
"l1010:   b l1009    \n" \
"l1011:   b l1014    \n" \
"l1012:   b l1011    \n" \
"l1013:   b l1016    \n" \
"l1014:   b l1013    \n" \
"l1015:   b l1018    \n" \
"l1016:   b l1015    \n" \
"l1017:   b l1020    \n" \
"l1018:   b l1017    \n" \
"l1019:   b l1022    \n" \
"l1020:   b l1019    \n" \
"l1021:   b l1023    \n" \
"l1022:   b l1021    \n" \
"l1023:  \n"

#endif /* M1_H */