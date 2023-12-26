#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <linux/limits.h>

#define MAX_PATH_LENGTH PATH_MAX
#define MAX_FILENAME_LENGTH NAME_MAX

void copy_files_by_list(const char *tmp_source_dir_path, FILE *list, const char *prefix)
{
    if (list == NULL)
    {
        perror("copy_files_by_list.c: Cannot open list file");
        exit(1);
    }

    char file_name[MAX_FILENAME_LENGTH];
    while (fgets(file_name, sizeof(file_name), list) != NULL)
    {
        DIR *dir = opendir(tmp_source_dir_path);
        if (dir == NULL)
        {
            perror("copy_files_by_list.c: Cannot open package source directory");
            exit(1);
        }

        struct dirent *entry;
        struct stat stat_buf;
        while ((entry = readdir(dir)) != NULL)
        {
            char source_path[MAX_PATH_LENGTH];
            sprintf(source_path, "%s/%s", tmp_source_dir_path, entry->d_name);
            if (stat(source_path, &stat_buf) == -1)
                continue;
            if (S_ISDIR(stat_buf.st_mode))
            {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                    continue;
                char dest_dir[MAX_PATH_LENGTH];
                sprintf(dest_dir, "%s/%s", prefix, entry->d_name);
                mkdir(dest_dir, 0777);
                copy_files_from_list_file(source_path, list, dest_dir);
            }
            else
            {
                if (!strcmp(entry->d_name, file_name))
                {
                    char destination_file_path[MAX_PATH_LENGTH];
                    sprintf(destination_file_path, "%s/%s", prefix, entry->d_name);

                    FILE *source_file = fopen(source_path, "rb");
                    FILE *dest_file = fopen(destination_file_path, "wb");

                    if (source_file && dest_file)
                    {
                        int ch;
                        while ((ch = fgetc(source_file)) != EOF)
                        {
                            fputc(ch, dest_file);
                        }
                        fclose(source_file);
                        fclose(dest_file);
                    }
                    else
                    {
                        pk_error(1, "copy_files_by_list.c: Cannot copy files");
                        exit(1);
                    }
                }
            }
        }
        closedir(dir);
    }
    rewind(list);
    return;
}
