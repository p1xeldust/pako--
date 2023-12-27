#include "../compat/linux/limits.h"
#include <stdio.h>

#include "tar.h"
#include "../essentials/create.h"

char tmp_path[PATH_MAX];
char *unpack_package(const char *filename)
{
    snprintf(tmp_path, sizeof(tmp_path), "%s/%s_tmp", TMP_PATH, filename);
    create_directories(tmp_path);
    extract_tar(filename, tmp_path);
    return tmp_path;
}