# xv6-getprocs-syscall

Implemented a custom `getprocs()` system call and a `ps` user program in the xv6 operating system.

## Features

- Added a new `getprocs()` system call to retrieve process information from the xv6 kernel.
- Implemented a `ps` command to display active process details in a table format.
- Displays:
  - PID
  - PPID
  - STATE
  - SIZE
  - NAME

## Locking

The `getprocs()` system call iterates through the xv6 process table.

For each process, the kernel acquires the corresponding process lock before reading its fields.

This ensures safe access to process information and prevents race conditions while processes are being scheduled or modified concurrently.

## Memory Isolation

The kernel does not directly dereference user-space pointers.

For each process entry, information is first stored in a kernel-side structure.

The `copyout()` function is then used to safely copy this data from kernel space to the user-provided buffer.

If `copyout()` fails, the system call immediately returns `-1`.

## Testing

The project was tested by booting xv6 in QEMU and executing the `ps` user program.

Verified:

- Correct PID, PPID, STATE, SIZE, and NAME output
- Stable behavior after multiple executions
- No kernel panics or crashes
- Process state strings matched expected specification

## Usage

Boot xv6 in QEMU:

```bash
make qemu
Run: 
ps

Author: Hojun Jeon
