#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

volatile long counter = 2;

void handler1(int sig) {
    sigset_t mask, prev_mask;
    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, &prev_mask); /* Block sigs */
    Sio_putl(--counter);
    sigprocmask(SIG_SETMASK, &prev_mask, NULL); /* Restore sigs */
    _exit(0);
}

int main() {
    pid_t pid;
    sigset_t mask, prev_mask;

    printf("%ld", counter);
    fflush(stdout);

    signal(SIGUSR1, handler1);

    if ((pid = fork()) == 0) {
        while (1) {};
    }

    kill(pid, SIGUSR1);
    waitpid(-1, NULL, 0);

    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, &prev_mask); /* Block sigs */

    printf("%ld", ++counter);

    sigprocmask(SIG_SETMASK, &prev_mask, NULL); /* Restore sigs */

    exit(0);
}

