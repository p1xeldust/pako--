#include <stddef.h>
#include <stdio.h>

#include "package_specs.h"
#include "arch.h"
#include "../output/o.h"

int check_specs(struct pkg_data *pd) {
    if (pd->name[0] == '\0') {
        pk_warn(0, "specs_check.c: Incorrect package name. Skipping current package.\n");
        return -1;
    }

    if (pd->version[0] == '\0') {
        pk_error(0, "specs_check.c: Incorrect version. Skipping current package.\n");
        return -1;
    }
    
    if(pd->arch.name[0] == '\0') {
        pk_error(0, "specs_check.c: Incorrect arch. Skipping current package.\n");
        return -1;
    }
    return check_architecture(pd);
}