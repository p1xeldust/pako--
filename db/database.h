#ifndef DATABASE_H
#define DATABASE_H

#include "../package/package_specs.h"

void init_database();
void add_package_to_database(struct pkg_data *pd, const char *listPath, const char *infoPath);
int is_in_database(struct pkg_data *pd);
void get_package_data_from_db(const char* pkg_name, struct pkg_data *pd);

#endif
