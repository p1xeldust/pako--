#ifndef COPY_H
#define COPY_H

void copy_file(const char *source, const char *destination);

// АХТУНГ!! Рекурсивная функция
void copy_directory(const char *source, const char *destination);

#endif