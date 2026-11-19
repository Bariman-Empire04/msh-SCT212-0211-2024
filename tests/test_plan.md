# Test Plan for msh

| Input | Expected | Actual |
|-------|----------|--------|
| (blank line) | New prompt, no error | ✅ Works |
| `echo hello world` | Prints text; status 0 | ✅ Works |
| `ls /nonexistent` | Error from ls; nonzero status | ✅ Works |
| `nosuchcommand` | Error message; status 127 | ✅ Works |
| `cd /tmp` then `pwd` | Prints /tmp | ✅ Works |
| `sleep 3 &` | Prompt returns at once; PID shown | ✅ Works |
| `echo hi > f.txt` then `cat f.txt` | File contains "hi" | ✅ Works |
| `sleep 100` then Ctrl-C | Sleep stops; shell survives; status 130 | ✅ Works |
| Ctrl-D at prompt | Shell exits cleanly | ✅ Works |
| `help` | Prints help info | ✅ Works |
| `cd` with no arg | Goes to HOME dir | ✅ Works |
| `exit` | Shell exits | ✅ Works |
