#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"

void myexec(char *argv[], int argc, char *buf) {
    char largv[MAXARG - argc][32];
    memset(largv, 0, (MAXARG - argc) * 32);
    // split the buf by ' '
    int li = 0, lii = 0, bi = 0;
    while (buf[bi] != '\0') {
        if (buf[bi] == ' ') {
            if (lii != 0) {
                li++;
                lii = 0;
            }
        } else {
            largv[li][lii++] = buf[bi];
        }
        bi++;
    }
    if (lii == 0) {
        li--;
    }
    // copy largv to argv
    for (int i = 0; i <= li; i++) {
        argv[argc + i] = largv[i];
    }
    // identify the end
    argv[argc + li + 1] = 0;
    
    if (fork() == 0) {
        // exec
        exec(argv[0], argv);
        exit(0);
    }
    wait(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: xargs cmd\n");
        exit(1);
    }

    char *targv[MAXARG];
    for (int i = 1; i < argc; i++) {
        targv[i - 1] = argv[i];
    }

    char buf[512], tbuf[512];
    int len, idx = 0;
    while ( (len = read(0, buf + idx, 512 - idx)) >= 0) {
        int prev = 0, cur = 0;
        while (cur < idx + len) {
            if (buf[cur] == '\n') {
                if (prev != cur) {
                    // there is an extra parameter
                    memmove(tbuf, buf + prev, cur - prev);
                    tbuf[cur - prev] = 0;
                    myexec(targv, argc - 1, tbuf);
                }
                prev = cur + 1;
            }
            cur++;
        }

        memmove(buf, buf + prev, cur - prev);
        idx = cur - prev;
        buf[idx] = '\0';

        if (len == 0) {
            if (idx != 0) {
                myexec(targv, argc - 1, buf);
            }
            break;
        }
    }
    exit(0);
}
