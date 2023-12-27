#include "../db/database.h"
#include "package_specs.h"
#include "../output/o.h"

void pk_list_packages() {
    struct pkg_data pd;
    get_package_data_from_db("flag", &pd);
    pk_msg("|'%s' |'%s' |'%s' |'%s' |'%s' |\n", pd.name, pd.version, pd.arch.name, pd.deps, pd.confs);
}

int main() {
    init_database();
    pk_list_packages();
}