#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/procinfo.h"

static const char*
statestr(int state)
{
  switch (state) {
  case 0: return "UNUSED";
  case 1: return "EMBRYO";   // USED -> EMBRYO
  case 2: return "SLEEPING";
  case 3: return "RUNNABLE";
  case 4: return "RUNNING";
  case 5: return "ZOMBIE";
  default: return "UNKNOWN";
  }
}

static int
intlen(int v)
{
  int n = 0;
  if (v == 0) return 1;
  while (v > 0) {
    n++;
    v /= 10;
  }
  return n;
}

static void
print_ljust_int(int v, int width)
{
  printf("%d", v);
  int n = intlen(v);
  for (int i = n; i < width; i++)
    printf(" ");
}

static void
print_ljust_str(const char* s, int width)
{
  int n = 0;
  while (s[n] && n < width) {
    printf("%c", s[n]);
    n++;
  }
  for (int i = n; i < width; i++)
    printf(" ");
}



int
main(void)
{
  struct procinfo pinfo[64];
  int n = getprocs(pinfo, 64);

  if (n < 0) {
    fprintf(2, "ps: getprocs failed\n");
    exit(1);
  }

  // header (manual spacing)
  printf("PID    PPID   STATE      SIZE       NAME\n");
  for (int i = 0; i < n; i++) {
    print_ljust_int(pinfo[i].pid, 6);
    printf(" ");

    print_ljust_int(pinfo[i].ppid, 6);
    printf(" ");

    print_ljust_str(statestr(pinfo[i].state), 10);
    printf(" ");

    print_ljust_int((int)pinfo[i].sz, 10);
    printf(" ");

    printf("%s\n", pinfo[i].name);
  }


  exit(0);
}
