#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <mach/vm_statistics.h>
#include <errno.h>

#define SUPERPAGE_SIZE (2*1024*1024)

int main() {

    //pthread_set_qos_class_self_np(QOS_CLASS_BACKGROUND, 0);
    pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);

    char* working_memory = (char*)mmap(
        NULL,
        SUPERPAGE_SIZE,
        PROT_READ,
        MAP_ANON | MAP_PRIVATE,
        VM_FLAGS_SUPERPAGE_SIZE_2MB,
        0
    );
    if(working_memory == MAP_FAILED) {
        printf("Error allocating memory: %d\n", errno);
    }

    printf("num: %u\n", working_memory[0]);

    //working_memory[0] = 64;
    //printf("num: %u\n", working_memory);

    return 0;
}