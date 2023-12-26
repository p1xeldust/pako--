#include <stdio.h>
#include "package_specs.h"

int parse_main_specs(FILE *specs_file, struct pkg_data *pd);
void parse_deps_confs(FILE *specs_file, struct pkg_data *pd);
int parse_specs(FILE *specs_file, struct pkg_data *pd);