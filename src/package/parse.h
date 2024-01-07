#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include "package_specs.h"

int parse_specs(char* specs_file_path, struct pkg_data *pd);

#endif