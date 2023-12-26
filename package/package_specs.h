#include "../compat/linux/limits.h"

enum arch_meta
{
    ARCH_NATIVE,
    ARCH_FOREIGN,
    ARCH_NONE
};

enum pkg_meta_type
{
    DEFAULT,
    RIDICULOUS,
    META
};

struct pkg_arch_data
{
    char name[16];
    enum arch_meta meta;
};

struct pkg_data_files {
    char info_file_path[PATH_MAX];
    char list_file_path[PATH_MAX];
};

struct pkg_data
{
    char name[64];
    char version[16];
    char deps[512];
    char confs[512];
    struct pkg_arch_data arch;
    enum pkg_meta_type meta;
    struct pkg_data_files files;
};