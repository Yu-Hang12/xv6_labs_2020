#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1

int lpipe_first_data(int lpipe[2], int *dst) {
    if(read(lpipe[0], dst, sizeof(int)) == sizeof(int)) {
        printf("primes %d\n", *dst);
        return 0;
    }
    return -1;
}

void transmit_data(int lpipe[2], int rpipe[2], int first) {
    int data;

    while(read(lpipe[0], &data, sizeof(int)) == sizeof(int)) {
        if(data % first) {
            write(rpipe[1], &data, sizeof(int));
        }
    }
    close(lpipe[RD]);
    close(rpipe[WR]);
}

void primes(int lpipe[2]) {
    close(lpipe[WR]); 

    int first;
    if (lpipe_first_data(lpipe, &first) == 0) {
        int rpipe[2];
        pipe(rpipe);
        transmit_data(lpipe, rpipe, first);
        if(fork() == 0) {
            primes(rpipe);
        } else {
            close(rpipe[RD]);
            wait(0);
        }
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);

    for(int i = 2; i < 35; i++) {
        write(p[WR], &i, sizeof(int));
    }

    if(fork() == 0) {
        primes(p);
    } else {
        close(p[RD]);
        close(p[WR]);
        wait(0);
    }
    exit(0);
}