#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

int pid;
volatile int flag = 0;
volatile int parentCount = 0;
volatile int childCount = 0;
volatile int finished = 0;

void signalParent(int sig) {
    if (sig == SIGUSR1) {
        parentCount += 1;
        kill(pid, SIGUSR2);
    } else if (sig == SIGCHLD) {
        finished = 1;
    }
}

void signalChild(int sig) {
    childCount += 1;
    flag = 1;
}

int main() {
    signal(SIGUSR1, signalParent);
    signal(SIGCHLD, signalParent);
    signal(SIGUSR2, signalChild);

    pid = fork();

    if (pid == 0) {
        srand(time(NULL));
        int const n = rand() % 500;

        for(int i = 0; i < n; i++) {
            flag = 0;
            // childCount += 1;
            kill(getppid(), SIGUSR1);
            while(flag == 0);
        }

        printf("Child: Sent %d\n", childCount);
        exit(n);
    } else {
        while(!finished) {
            pause();
        }

        int status;
        wait(&status);

        const int totalSignalCount = WEXITSTATUS(status);               // numarul total de semnale trimis de catre exit este uneori diferit fata de ce numaralerii
        printf("Parent: Received %d. (Sent: %d)\n", parentCount, totalSignalCount);
    }

    return 0;
}