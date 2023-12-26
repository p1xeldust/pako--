#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "specs_check.h"
#include "../output/o.h"
#include "arch.h"

#define SPECS_LINE_LENGTH 80
#define CONS_DEPS_LINE_LENGTH 528

int parse_specs(FILE *specs_file, struct pkg_data *pd) {

    memset(pd, 0, sizeof(struct pkg_data));

    if (!specs_file) {
        perror("Ошибка открытия файла");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), specs_file)) {
        char *token = strtok(line, ":");
        if (token) {
            if (strstr(token, "name")) {
                strtok(NULL, " ");
                strcpy(pd->name, strtok(NULL, " \n"));
            } else if (strstr(token, "arch")) {
                strtok(NULL, " ");
                strcpy(pd->arch.name, strtok(NULL, " \n"));
            } else if (strstr(token, "version")) {
                strtok(NULL, " ");
                strcpy(pd->version, strtok(NULL, " \n"));
            } else if (strstr(token, "type")) {
                strtok(NULL, " ");
                pd->meta = atoi(strtok(NULL, " \n"));
            } else if (strstr(token, "dependencies")) {
                strtok(NULL, " ");
                strcpy(pd->deps, strtok(NULL, " \n"));
            } else if (strstr(token, "conflicts")) {
                strtok(NULL, " ");
                strcpy(pd->confs, strtok(NULL, " \n"));
            }
        }
    }
    rewind(specs_file);
    return 0;
}