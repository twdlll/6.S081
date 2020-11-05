#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p2c[2], c2p[2];
    if (pipe(p2c) < 0 || pipe(c2p) < 0) {
        printf("pipe() error\n");
    }

    char buf[2];
    int pid = fork();
    if (pid > 0) {
        // it's parent
        write(p2c[1], "a", 1);
        read(c2p[0], buf, 1);
        printf("%d: received pong\n", getpid());
    } else if (pid == 0) {
        // it's child
        read(p2c[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(c2p[1], "a", 1);
    } else {
        printf("fork() error\n");
    }
    exit(0);
}