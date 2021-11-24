#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/sysctl.h>
#include <inttypes.h>

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

int main(int argc, const char * argv[]) {
    
    uint64_t pmcr0 = get_sysctl("kern.pmcr0");
    uint64_t pmcr1 = get_sysctl("kern.pmcr1");
    uint64_t pmesr0 = get_sysctl("kern.pmesr0");
    uint64_t pmesr1 = get_sysctl("kern.pmesr1");
    
    FILE* outf = fopen("/Users/abhattac/Rev/sys_reg_dump/sys_reg_dump/sys_reg_dump/results.txt", "w");
    if(outf == NULL) {
        printf("Could not open file: %d\n", errno);
        return 0;
    }
    
    char temp[64];
    
    sprintf(temp, "PMCR0: %llx\n", pmcr0);
    fputs(temp, outf);
    
    sprintf(temp, "PMCR1: %llx\n", pmcr1);
    fputs(temp, outf);
    
    sprintf(temp, "PMESR0: %llx\n", pmesr0);
    fputs(temp, outf);
    
    sprintf(temp, "PMESR1: %llx\n", pmesr1);
    fputs(temp, outf);
    
    fclose(outf);
    
    set_sysctl("kern.pmcr1", 0x0);
    
    return 0;
}
