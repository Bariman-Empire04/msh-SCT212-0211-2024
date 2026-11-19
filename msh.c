#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

// Global: last exit status (shown in prompt)
int last_status = 0;

int main(void) {
    char *line = NULL;    // getline will allocate this
    size_t len  = 0;      // getline will set this
    ssize_t nread;

    // Task 8: Shell itself ignores Ctrl-C
    signal(SIGINT, SIG_IGN);

    while (1) {
        // Task 6: Reap finished background jobs before each prompt
        while (waitpid(-1, NULL, WNOHANG) > 0);

        // Task 5: Show exit status in prompt
        printf("msh[%d]> ", last_status);
        fflush(stdout);       // force the prompt to appear

        nread = getline(&line, &len, stdin);

        if (nread == -1) {    // User pressed Ctrl-D (EOF)
            free(line);
            printf("\n");
            break;            // exit cleanly
        }

        // TODO: we'll add parsing and execution below
    }

    return 0;
}
