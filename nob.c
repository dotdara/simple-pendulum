#define NOB_IMPLEMENTATION
#include "nob.h"
#include <stdio.h>

Nob_Cmd cmd = {0};

void cc(void)
{
    cmd_append(&cmd, "cc");
    cmd_append(&cmd, "-Wall");
    cmd_append(&cmd, "-Wextra");
}

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    
    cc();
    cmd_append(&cmd, "-o", "./main", "main.c");
    // cmd_append(&cmd, "-lplplot");
    cmd_append(&cmd, "-lraylib", "-lGL", "-lm", "-lpthread", "-ldl", "-lrt", "-lX11");
    if (!cmd_run(&cmd)) return 1;

    return 0;
}