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
void run_command(char **argv, int bg) {
    pid_t pid = fork();       // create a child process

    if (pid < 0) {
        perror("fork");       // fork failed
        return;
    }

    if (pid == 0) {

        // Task 8: Child restores default Ctrl-C behaviour
        signal(SIGINT, SIG_DFL);

        // Task 7: Handle output redirection (> filename)
        for (int i = 0; argv[i] != NULL; i++) {
            if (strcmp(argv[i], ">") == 0) {
                if (argv[i+1] == NULL) {
                    fprintf(stderr, "msh: expected filename after >\n");
                    exit(1);
                }
                int fd = open(argv[i+1],
                              O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror(argv[i+1]);
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);   // redirect stdout to file
                close(fd);
                argv[i] = NULL;            // cut off ">" and filename
                break;
            }
        }

        // Replace child with the requested program
        execvp(argv[0], argv);

        // execvp only returns if it FAILED
        perror(argv[0]);
        exit(127);    // 127 = command not found
    }

 
    if (bg) {
        // Background: don't wait, just print the PID
        printf("[bg] PID %d\n", pid);
    } else {
        // Foreground: wait for child to finish
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
            last_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            last_status = 128 + WTERMSIG(status);
    }
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

        // Task 6: Check for background '&'
        int bg = 0;
        int i = 0;
        while (argv[i+1] != NULL) i++;   // find last arg
        if (strcmp(argv[i], "&") == 0) {
            bg = 1;
            argv[i] = NULL;            
        }

                if (strcmp(argv[0], "exit") == 0) {
            free(argv);
            free(line);
            exit(0);
        } else if (strcmp(argv[0], "cd") == 0) {
            char *dir = argv[1] ? argv[1] : getenv("HOME");
            if (chdir(dir) != 0)
                perror("cd");
        } else if (strcmp(argv[0], "help") == 0) {
            printf("msh — Minimal Shell\n");
            printf("Built-ins: cd [dir], help, exit\n");
            printf("Features: background (&), redirection (>), Ctrl-C\n");
        } else {
            // External command: use fork/exec/wait
            run_command(argv, bg);
        }
        free(argv);
    }

    return 0;
}
