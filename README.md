# Nexus Shell

A Unix shell built written in C, implementing core operating system concepts including process management, file descriptors, inter-process communication, and persistent state management.

## Features

- **Process execution** — runs any system command via `fork()` and `execvp()`
- **Built-in commands** — `cd`, `pwd`, `clear`, `history`, `exit` implemented directly in the shell process
- **I/O redirection** — `>` (overwrite), `>>` (append), `<` (input) using `dup2()` and file descriptors
- **Pipes** — `cmd1 | cmd2` inter-process communication via anonymous pipes
- **Persistent history** — command history saved to `~/.nexus_history` and restored across sessions

## Build and Run

**Requirements:** GCC, Linux or WSL (Ubuntu)

```bash
make
./nexus
```

**Clean build:**
```bash
make clean
```

## Usage

```bash
nexus> ls | grep main
nexus> ls > output.txt
nexus> cat < output.txt
nexus> echo hello >> log.txt
nexus> cd /tmp
nexus> history
nexus> exit
```

## Project Structure

```
nexus_shell/
  src/
    main.c        # Shell loop, input parsing, redirection, pipes, history
    builtins.c    # Built-in command implementations
  include/
    nexus.h       # Shared prototypes, defines, includes
  manual/
    guide.txt     # User guide (open with: more manual/guide.txt)
  Makefile
```

## Key Concepts

| Concept | Implementation |
|---|---|
| Process creation | `fork()` + `execvp()` |
| I/O redirection | `dup2()` replaces stdin/stdout with file descriptors |
| Pipes | `pipe()` creates read/write ends, two child processes connected |
| Persistent history | File I/O to `~/.nexus_history` on every command |
| Built-ins | Run in parent process — required for `cd` to affect shell state |
