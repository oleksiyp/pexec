#include <sys/ptrace.h>
#include <sys/unistd.h>
#include <sys/wait.h>
#include <sys/user.h>
#include "exec_cmd.h"
#include <stdlib.h>

int copy_code(pid_t pid, const char *s, char *d, int len) {
    int i;

    for (i = 0; i < len; i += 4, s += 4, d += 4) {
        if (ptrace(PTRACE_POKETEXT, pid, d, *((__uint32_t *)s)) < 0) {
            return -1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        write(STDOUT_FILENO, "Usage: pexec <pid> <command>\n", 29);
        _exit(1);
    }

    pid_t pid = atoi(argv[1]);

    if (ptrace(PTRACE_ATTACH, pid) < 0) {
        _exit(2);
    }

    wait(NULL);

    struct user_regs_struct regs;

    if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) < 0) {
        _exit(3);
    }

    int len = exec_cmd_len + strlen(argv[2]) + 1;
    char *data = (char*)malloc(exec_cmd_len + strlen(argv[2]) + 1);
    memcpy(data, exec_cmd, exec_cmd_len);
    memcpy(data + exec_cmd_len, argv[2], strlen(argv[2]) + 1);

    if (copy_code(pid, data, (void *) regs.rip, len) < 0) {
        free(data);
        _exit(4);
    }
    free(data);

    regs.rip += 2;

    if (ptrace(PTRACE_SETREGS, pid, NULL, &regs) < 0) {
        _exit(5);
    }

    if (ptrace(PTRACE_DETACH, pid, NULL, NULL) < 0) {
        _exit(6);
    }

    return 0;
}
