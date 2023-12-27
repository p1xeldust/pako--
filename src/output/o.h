#ifndef OUTPUT_H
#define OUTPUT_H

void pk_error(int errno_en, const char *message_format, ...);
void pk_warn(const char *message_format, ...);
void pk_debug(const char *message_format, ...);
void pk_msg(const char *message_format, ...);

#endif