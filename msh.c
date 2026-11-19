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

int last_status = 0;


// Returns a NULL-terminated array of argument strings
// The returned array must be free()'d by the caller
char **parse_args(char *line) {
    int max = 128;
    char **argv = malloc(max * sizeof(char *));
    int argc = 0;

    // strtok splits on spaces and tabs
    char *token = strtok(line, " \t\n");
    while (token != NULL && argc < max - 1) {
        argv[argc++] = token;
        token = strtok(NULL, " \t\n");   // NULL = continue from last position
    }
    argv[argc] = NULL;    // MUST end with NULL (execvp requires this)
    return argv;
}
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

                char **argv = parse_args(line);

        if (argv[0] == NULL) {    // blank line — skip it
            free(argv);
            continue;
        }

        // TODO: execute argv
        free(argv);
    }

    return 0;
}
