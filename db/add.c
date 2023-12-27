#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "../package/package_specs.h"
#include "../output/o.h"

void add_package_to_database(struct pkg_data *pd, const char* listPath, const char* infoPath) {
    char db_path[strlen(VAR_PATH) + strlen("/packages.db") + 1];
    sprintf(db_path, "%s/packages.db", VAR_PATH);

    sqlite3* db; sqlite3_stmt* stmt;

    if (sqlite3_open(db_path, &db)) {
        pk_error(0, "add.c:add_package_to_database: Error opening database. SQLite error: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    if(sqlite3_prepare_v2(db, "INSERT INTO packages VALUES (?, ?, ?);", -1, &stmt, 0) != SQLITE_OK) {
        pk_error(0, "add.c:add_package_to_database: Unable to insert package into database. SQLite error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    
    sqlite3_bind_text(stmt, 1, pd->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, pd->files.list_file_path, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, pd->files.info_file_path, -1, SQLITE_STATIC);
    
    if(sqlite3_step(stmt) != SQLITE_DONE) {
        pk_error(0, "add.c:add_package_to_database: Unable to insert package into database. SQLite3 error: %s.\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return;
}
