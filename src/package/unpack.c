#include <string.h>
#include <stdio.h>

#include "../compat/linux/limits.h"
#include "../essentials/create.h"
#include "tar.h"

void unpack_package(const char* filename, const char *tmp_path)
{
    create_directories(tmp_path);
    extract_tar(filename, tmp_path);
    return;
}