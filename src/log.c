#include <stdarg.h>
#include <stdio.h>

#include "Pixa/globals.h"
#include "Pixa/log.h"

void log_msg(char *msg_type, char *sender, char *msg_format, ...)
{
    if (sender == NULL)
        sender = LOG_USER;

    int s = (int) elapsed_time % 60;
    int m = (int) (elapsed_time / 60) % 60;
    int h = (int) (elapsed_time / 360);
    
    printf("[%d:%d:%d] [%s] [%s]: ",
            h, m, s,
            msg_type, sender);

    va_list args;
    va_start(args, msg_format);
    vprintf(msg_format, args);
    va_end(args);

    printf("\n");
}