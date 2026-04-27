#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main(void)
{
    printf("Note: The OS course number at York University is: %d\n", getcourseno());
    exit(0);
}

