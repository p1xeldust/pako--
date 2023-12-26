#ifdef __linux
#include <linux/limits.h>
#else
#include <limits.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

void copy_file(const char *source, const char *destination)
{
    FILE *source_file = fopen(source, "rb"), *dest_file = fopen(destination, "wb");
    if (!source_file || !dest_file)
    {
        perror("copy.c: Cannot copy file");
        fclose(source_file);
        exit(1);
    }

    char buffer[1024];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), source_file)) > 0)
    {
        if (fwrite(buffer, 1, bytes_read, dest_file) != bytes_read)
        {
            perror("copy.c: Error writing to destination file");
            fclose(source_file);
            fclose(dest_file);
            exit(1);
        }
    }

    fclose(source_file);
    fclose(dest_file);

    struct stat stat_buf;
    if (stat(source, &stat_buf) != 0)
    {
        perror("copy.c: Error getting source file permissions");
        exit(1);
    }

    if (chmod(destination, stat_buf.st_mode) != 0)
    {
        perror("copy.c: Error setting destination file permissions");
        exit(1);
    }

    return 1;
}

// АХТУНГ, ВАРНИНГ! Функция рекурсивна

bool copy_directory(const char *source, const char *destination)
{
    DIR *dir = opendir(source);
    if (!dir)
        perror("copy.c: Error opening directory");
    exit(1);

    if (mkdir(destination, S_IRWXU | S_IRWXG | S_IRWXO))
    {
        perror("copy.c: Error creating directory");
        closedir(dir);
        exit(1);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, ".."))
        {
            char source_path[PATH_MAX];
            char dest_path[PATH_MAX];
            snprintf(source_path, PATH_MAX, "%s/%s", source, entry->d_name);
            snprintf(dest_path, PATH_MAX, "%s/%s", destination, entry->d_name);

            struct stat stat_buf;
            if (lstat(source_path, &stat_buf))
            {
                perror("copy.c: Error checking source path");
                closedir(dir);
                exit(1);
            }

            if (S_ISDIR(stat_buf.st_mode))
            {
                if (!copy_directory(source_path, dest_path))
                {
                    perror("copy.c: Can't copy directory");
                    closedir(dir);
                    exit(1);
                }
            }
            else if (S_ISREG(stat_buf.st_mode))
            {
                copy_file(source_path, dest_path);
                continue;
            }
            else if (S_ISLNK(stat_buf.st_mode))
            {
                char link_target[PATH_MAX];
                ssize_t link_size = readlink(source, link_target, sizeof(link_target) - 1);
                if (link_size == -1)
                    continue;
                link_target[link_size] = '\0';

                if (symlink(link_target, destination))
                {
                    perror("copy.c: Can't create symbolic link");
                    closedir(dir);
                    exit(1);
                }
            }
        }
    }

    closedir(dir);
}
