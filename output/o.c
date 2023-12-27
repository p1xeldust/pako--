/**
 * Written by Paul Goldstein, December 2023.
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

void pk_msg(const char *message_format, ...)
{
    va_list args;
    va_start(args, message_format);
    printf("\033[0;1m");
    vfprintf(stdout, message_format, args);
    printf("\033[1;0m");
    va_end(args);
}

void pk_debug(const char *message_format, ...)
{
    va_list args;
    va_start(args, message_format);
    printf("\033[1;90m[DEBUG] ");
    vfprintf(stdout, message_format, args);
    printf("\033[1;0m");
    va_end(args);
}

void pk_warn(const char *message_format, ...)
{
    va_list args;
    va_start(args, message_format);
    fprintf(stdout, "\033[1;33mWarn\033[0;1m: ");
    vfprintf(stdout, message_format, args);
    fprintf(stdout, "\033[1;0m");
    va_end(args);
}
void pk_error(int errno_enable, const char *message_format, ...)
{
    va_list args;
    va_start(args, message_format);
    fprintf(stderr, "\033[1;31mError\033[0;1m: ");
    vfprintf(stderr, message_format, args);
    if (!errno_enable)
        fprintf(stderr, "\033[1;0m");
    else
        fprintf(stderr, ": %s\033[1;0m\n", strerror(errno));
    va_end(args);
}