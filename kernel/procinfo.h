// kernel/procinfo.h
#ifndef PROCINFO_H
#define PROCINFO_H

struct procinfo {
    int pid;          // Process ID
    int ppid;         // Parent process ID
    int state;        // Process state (UNUSED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE)
    uint sz;          // Memory size in bytes
    char name[16];    // Process name
};

#endif

