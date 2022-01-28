#ifndef PIXA_LOG_H
#define PIXA_LOG_H

#define LOG_INFO        "INFO"
#define LOG_WARNING     "WARNING"
#define LOG_ERROR       "ERROR"

#define LOG_USER        "User"
#define LOG_GLFW        "GLFW"
#define LOG_OPENGL      "OpenGL"

void log_msg(char *msg_t, char *sender, char *msg_format, ...);

#endif