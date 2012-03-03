#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "zpu.h"


// Print usage instructions
void usage()
{
    printf ("ZOG v0.11\n");
}


int main(int argc, char *argv[])
{
    usage();
    zpu_reset();
    zpu_execute();
}
