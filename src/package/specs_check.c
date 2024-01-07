/**
 * Written by Paul Goldstein, December 2023.
 */

#include <stdlib.h>

#include "package_specs.h"
#include "arch.h"
#include "../output/o.h"

int check_specs(struct pkg_data *pd) {
    if (pd->name[0] == '\0') {
        pk_error(0, "specs_check.c: Incorrect package name. Stopping.\n");
        exit(1);
    }

    if (pd->version[0] == '\0') {
        pk_error(0, "specs_check.c: Incorrect version. Stopping.\n");
        exit(1);
    }
    
    if(pd->arch.name[0] == '\0') {
        pk_error(0, "specs_check.c: Incorrect arch. Stopping.\n");
        exit(1);
    }
    return check_architecture(pd);
}