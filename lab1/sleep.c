#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: sleep number\n");
        exit(1);
    }

    int tick_num = atoi(argv[1]);
    if (tick_num == 0) {
        printf("The number of ticks is 0\n");
        exit(1);
    }
    
    sleep(tick_num);
    exit(0);
}
