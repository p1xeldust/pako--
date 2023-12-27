/**
 * Written by Paul Goldstein, December 2023.
 */

#include "../compat/sys/utsname.h"
#include <string.h>
#include <stdio.h>
#include "../output/o.h"

#include "package_specs.h"

int check_architecture(struct pkg_data *pd)
{
    struct utsname host_sysinfo;
    uname(&host_sysinfo);
    if(!strlen(pd->arch.name)) {
        pk_warn(0, "Incorrect architecture, skipping package.");
        return -1;
    }
    if (!strcmp(pd->arch.name, "none"))
        pd->arch.meta = ARCH_NONE;
    else
        pd->arch.meta = strcmp(host_sysinfo.machine, pd->arch.name) ? ARCH_FOREIGN : ARCH_NATIVE;
    return pd->arch.meta;
}
