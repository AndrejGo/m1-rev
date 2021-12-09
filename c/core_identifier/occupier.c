#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

unsigned long* array;

void* heavy_thread(void* arg) {

    int index = *((int *) arg);
    pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);
    usleep(100);

    unsigned long count = 0;

    while(1) {

        for(int i = 0; i < 100000; i++) {
            count += 1;
            count *= 12;
            count /= 4;
            count = count % 3;

            count += 2;
            count *= 16;
            count /= 8;
            count = count % 5;

            count += 3;
            count *= 2;
            count /= 3;
            count = count % 2;
        }

        array[index] = count;

    }

}

int main(int argc, char* argv[]) {

    pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);
    usleep(100);

    int thread_count = 0;
    if(argc == 2) {
        thread_count = atoi(argv[1]);
    }

    array = malloc(thread_count * sizeof(unsigned long));

    // Start threads
    for(int i = 0; i < thread_count; i++) {
        int *arg = malloc(sizeof(int));
        pthread_t background_thread;

        *arg = i;
        if(pthread_create(&background_thread, NULL, heavy_thread, arg) != 0) {
            return 1; 
        }
    }

    unsigned long count = 0;

    while(1) {

        for(int i = 0; i < 100000; i++) {
            count += 1;
            count *= 12;
            count /= 4;
            count = count % 3;

            count += 2;
            count *= 16;
            count /= 8;
            count = count % 5;

            count += 3;
            count *= 2;
            count /= 3;
            count = count % 2;
        }

        for(int i = 0; i < thread_count; i++) {
            count += array[i];
        }

        printf("%lu\033[;H", count);

    }

    return 0;
}