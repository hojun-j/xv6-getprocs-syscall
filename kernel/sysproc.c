#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "procinfo.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    if(addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

extern struct proc proc[];

uint64
sys_getprocs(void)
{
  uint64 uaddr;
  int max;

  // NOTE: in this xv6 version, argaddr/argint return void
  argaddr(0, &uaddr);
  argint(1, &max);

  if (max <= 0)
    return -1;

  if (max > NPROC)
    max = NPROC;

  int count = 0;
  struct proc* p;
  struct proc* cur = myproc();

  for (p = proc; p < &proc[NPROC] && count < max; p++) {
    acquire(&p->lock);
    if (p->state != UNUSED) {
      struct procinfo kpi;
      kpi.pid = p->pid;
      kpi.ppid = (p->parent ? p->parent->pid : 0);
      kpi.state = p->state;
      kpi.sz = p->sz;
      safestrcpy(kpi.name, p->name, sizeof(kpi.name));

      if (copyout(cur->pagetable,
        uaddr + (uint64)count * sizeof(kpi),
        (char*)&kpi,
        sizeof(kpi)) < 0) {
        release(&p->lock);
        return -1;
      }
      count++;
    }
    release(&p->lock);
  }

  return count;
}



