Baristone Joseph
# msh-SCT212-0211-2024
# msh — Minimal Unix Shell

## Build
```bash
make
./msh
```

## Features
| Task | Description | Status |
|------|-------------|--------|
| 1 | Prompt + getline input | ✅ Complete |
| 2 | Argument parsing (strtok) | ✅ Complete |
| 3 | fork/execvp/waitpid | ✅ Complete |
| 4 | Built-ins: cd, help, exit | ✅ Complete |
| 5 | Exit status in prompt | ✅ Complete |
| 6 | Background jobs (&) | ✅ Complete |
| 7 | Output redirection (>) | ✅ Complete |
| 8 | Ctrl-C handling (SIGINT) | ✅ Complete |

## Why cd cannot be external
When a child process calls `chdir()`, it changes *its own* working
directory. When the child exits, that change is lost — the parent shell
is completely unaffected. `cd` must run *inside* the shell process to
actually change the shell's directory.

## Why exit must be a built-in
An external `exit` process would exit its own child process, not the shell.

## How zombie processes are avoided
Before each prompt, the shell calls `waitpid(-1, NULL, WNOHANG)` in a
loop. This reaps any finished background processes without blocking.

## Design (fork/exec/wait cycle)
Every external command follows this pattern:
1. `fork()` creates a child copy of the shell
2. The child calls `signal(SIGINT, SIG_DFL)` to restore default Ctrl-C
3. If `>` is found in argv, the child opens the file and uses `dup2()`
   to redirect stdout, then removes `>` from argv
4. The child calls `execvp()` which replaces it with the target program
5. The parent either waits with `waitpid()` (foreground) or prints the
   PID and continues (background)
6. Exit status is stored and shown in the next prompt

## Known Limitations
- No pipe (`|`) support
- No input redirection (`<`)
- No command history

## Sources
- fork(2), execvp(3), waitpid(2), getline(3), signal(7) man pages
- AI assistance: used for code review and explanation
