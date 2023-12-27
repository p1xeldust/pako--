#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../package/package_specs.h"
#include "../output/o.h"

void init_database() {
    char db_path[strlen(VAR_PATH) + strlen("/packages.db") + 1];
    sprintf(db_path, "%s/packages.db", VAR_PATH);
    sqlite3* db;
    
    if (sqlite3_open(db_path, &db)) {
        pk_error(0, "init.c:init_database: Error opening database. SQLite error: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    if(sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS packages (name TEXT, infofilepath TEXT, filelistpath TEXT);", 0, 0, 0)) {
        pk_error(0, "init.c:init_database: Error creating packages table. SQLite error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_close(db);
    return;
}
