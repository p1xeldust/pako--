#include <string.h>
#include <stdlib.h>

#include "../db/database.h"
#include "../output/o.h"
#include "../essentials/create.h"
#include "../essentials/copy.h"
#include "package_specs.h"
#include "parse.h"
#include "unpack.h"

int install(int argc, char* argv[]) {
    init_database();
    for (int i = 2; i < argc; i++)
    {
        char tmp_path[strlen(TMP_PATH) + strlen(argv[i]) + strlen("_tmp") + 1];
        sprintf(tmp_path, "%s/%s_tmp", TMP_PATH, argv[i]);
        char tmp_path_specfile[strlen(TMP_PATH) + strlen(argv[i]) + strlen("_tmp/PAKO/info") + 1];
        sprintf(tmp_path_specfile, "%s/%s_tmp/PAKO/info", TMP_PATH, argv[i]);
        struct pkg_data* pd = malloc(sizeof(struct pkg_data));
        memset(pd, 0, sizeof(struct pkg_data));

        unpack_package(argv[i], tmp_path);
        parse_specs(tmp_path_specfile, pd);
        printf("Name: %s\nVersion: %s\nArch: %s\nMeta-Type: %d\nArch-meta: %d\nDependencies: %s\nConfigurations: %s\nDescription: %s\n", pd->name, pd->version, pd->arch.name, pd->meta, pd->arch.meta, pd->deps, pd->confs, pd->description);


        free(pd);
    }
    return 0;
}

int main(int argc, char* argv[])
{
    install(argc, argv);
}