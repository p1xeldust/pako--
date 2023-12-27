#include "../compat/linux/limits.h"
#include <dirent.h>
#include <stddef.h>

int remove_directory_recursively(const char* source_path)  {
    DIR* dir = opendir(source_path);
    if (dir == NULL) {
        return -1;
    }

    struct dirent* ent;
    while ((ent = readdir(dir))!= NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", source_path, ent->d_name);

        if (remove_directory_recursively(path)) {
            continue;
        }

        if (remove(path) == -1) {
            return -1;
        }
    }

    closedir(dir);
    return rmdir(source_path);
}