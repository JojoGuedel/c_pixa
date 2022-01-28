#include <stdio.h>

#include "Pixa/globals.h"
#include "Pixa/log.h"

void log_msg(char *msg_type, char *msg)
{
    int s = (int) elapsed_time % 60;
    int m = (int) (elapsed_time / 60) % 60;
    int h = (int) (elapsed_time / 360);
    
    printf("[%d:%d:%d] [%s]: %s\n",
            h, m, s,
            msg_type, msg);
}