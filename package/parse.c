#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "specs_check.h"
#include "../output/o.h"
#include "arch.h"

#define SPECS_LINE_LENGTH 80
#define CONS_DEPS_LINE_LENGTH 528

int parse_specs(struct pkg_data *pd) {
    FILE* specs_file = fopen(pd->files.info_file_path, "r");

    if (!specs_file) {
        pk_error(1, "parse.c:parse_specs Error opening '%s'", pd->files.info_file_path);
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), specs_file)) {
        char *token = strtok(line, " ");
        if (token) {
            char* value = strtok(NULL, "\n");
            if (strstr(token, "name")) {
                strcpy(pd->name, value);
            } else if (strstr(token, "arch")) {
                strcpy(pd->arch.name, value);
            } else if (strstr(token, "version")) {
                strcpy(pd->version, value);
            } else if (strstr(token, "type")) {
                pd->meta = atoi(value);
            } else if (strstr(token, "dependencies")) {
                strcpy(pd->deps, value);
            } else if (strstr(token, "conflicts")) {
                strcpy(pd->confs, value);
            } else if (strstr(token, "description")) {
                strcpy(pd->desc, value);
            }
        }
    }
    fclose(specs_file);
    return 0;
}