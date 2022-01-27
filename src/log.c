#include <stdio.h>

#include "Pixa/params.h"
#include "Pixa/log.h"

void log_msg(char *msg_type, char *msg)
{
    int s = (int) ellapsed_time % 60;
    int m = (int) (ellapsed_time / 60) % 60;
    int h = (int) (ellapsed_time / 360);
    
    printf("[%d:%d:%d] [%s]: %s\n",
            h, m, s,
            msg_type, msg);
}