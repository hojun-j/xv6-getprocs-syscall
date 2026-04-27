------------------------------------------------------------
EECS3221 – Operating Systems
Project 1
Student: Hojun Jeon (218877696)
------------------------------------------------------------

Locking:
- The getprocs() system call iterates through the xv6 process table.
- For each process, the kernel acquires the corresponding process lock before reading its fields.
- This ensures safe access to process information and prevents race conditions while processes are being scheduled or modified concurrently.

Memory isolation:
- The kernel does not directly dereference user-space pointers.
- For each process entry, information is first stored in a kernel-side procinfo structure.
- The copyout() function is then used to safely copy this data from kernel space to the user-provided buffer.
- If copyout() fails, the system call immediately returns -1 to indicate an error.

Testing approach and test cases:
- The system was tested by booting xv6 in QEMU and executing the ps user program.
- The output was verified to include correct PID, PPID, STATE, SIZE, and NAME fields.
- The ps command was executed multiple times consecutively to ensure stable behavior without kernel panics or crashes.
- Process state strings were confirmed to match the required specification.

Usage instructions:
- Boot xv6 using QEMU.
- From the xv6 shell prompt, run the command:
  ps
- The ps program will invoke getprocs() and display information about currently running processes in a table format.