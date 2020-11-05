#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void close_pipe(int *fd) {
    close(fd[0]);
    close(fd[1]);
}

void prime() {
    int first = -1, n;
    int fd[2];
    int have_child = 0;
    while (read(0, &n, 4) == 4) {
        if (first == -1) {
            first = n;
            printf("prime %d\n", first);
        } else if (n % first != 0) {
            if (have_child == 0) {
                // don't have child process
                // create pipe and child
                pipe(fd);
                int pid = fork();
                if (pid > 0) {
                    have_child = 1;
                    close(1);
                    dup(fd[1]);
                    close_pipe(fd);
                } else {
                    close(0);
                    dup(fd[0]);
                    close_pipe(fd);
                    prime();
                }
            }
            write(1, &n, 4);
        }
    }
    if (have_child == 1) {
        close(1);
        wait((int *)0);
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    int i, fd[2];
    pipe(fd);

    int pid = fork();
    if (pid > 0) {
        // it's parent, responsible for input
        close(1);
        dup(fd[1]);
        close_pipe(fd);
        for (i = 2; i <= 35; i++) {
            write(1, &i, 4);
        }
        close(1);
        wait((int *) 0);
    } else if (pid == 0) {
        // it's child, responsible for receiving and output
        close(0);
        dup(fd[0]);
        close_pipe(fd);
        prime();
    }
    exit(0);
}
