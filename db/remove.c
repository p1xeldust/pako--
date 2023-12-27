#include <sqlite3.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../package/package_specs.h"
#include "../output/o.h"

void remove_package_from_database(struct pkg_data *pd) {
    char db_path[strlen(VAR_PATH) + strlen("/packages.db") + 1];
    sprintf(db_path, "%s/packages.db", VAR_PATH);
    sqlite3* db; sqlite3_stmt* stmt;

    if (sqlite3_open(db_path, &db)) {
        pk_error(0, "remove.c:remove_package_from_database: Error while open database. SQLite error: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    if(sqlite3_prepare_v2(db, "DELETE FROM packages WHERE name = ?;", -1, &stmt, 0) != SQLITE_OK) {
        pk_error(0, "remove.c:remove_package_from_database: Error deleting package from database. SQLite error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    sqlite3_bind_text(stmt, 1, pd->name, -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) != SQLITE_DONE) {
        pk_error(0, "remove.c:remove_package_from_database: Error deleting package from database. SQLite error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return;
}
