#include "../compat/linux/limits.h"
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../package/parse.h"
#include "../output/o.h"

int is_in_database(const char* pkg_name)
{
    char db_path[strlen(VAR_PATH) + strlen("/packages.db") + 1];
    sprintf(db_path, "%s/packages.db", VAR_PATH);
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (sqlite3_open(db_path, &db))
    {
        pk_error(0, "read.c:is_in_database: Error opening database. SQLite error: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    if (sqlite3_prepare_v2(db, "SELECT * FROM packages WHERE name = ?;", -1, &stmt, 0) != SQLITE_OK)
    {
        pk_error(0, "read.c:is_in_database: Error selecting package from db. SQLite error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    sqlite3_bind_text(stmt, 1, pkg_name, -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    if (result == SQLITE_ROW)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    if (result != SQLITE_DONE)
    {
        pk_error(0, "read.c:is_in_database: Error selecting package from db. SQLite error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

int get_package_data_from_db(const char* pkg_name, struct pkg_data *pd) {
    char db_path[strlen(VAR_PATH) + strlen("/packages.db") + 1];
    sprintf(db_path, "%s/packages.db", VAR_PATH);

    pd->name[0] = '\0';
    pd->files.info_file_path[0] = '\0';
    pd->files.list_file_path[0] = '\0';
    sqlite3 *db;
    sqlite3_stmt *stmt;
    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        pk_error(0, "Error opening database. SQLite error: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    if (sqlite3_prepare_v2(db, "SELECT * FROM packages WHERE name = ?;", -1, &stmt, 0) != SQLITE_OK) {
        pk_error(0, "Error selecting package from db. SQLite error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }

    sqlite3_bind_text(stmt, 1, pkg_name, -1, SQLITE_STATIC);
    
    int step_result = sqlite3_step(stmt);
    if (sqlite3_step(stmt) == SQLITE_ROW == SQLITE_ROW) {
        strcpy(pd->name, (char *)sqlite3_column_text(stmt, 0));
        strcpy(pd->files.info_file_path, (char *)sqlite3_column_text(stmt, 1));
        strcpy(pd->files.list_file_path, (char *)sqlite3_column_text(stmt, 2));
    } else if(step_result != SQLITE_DONE) {
        pk_error(0, "Error selecting package from db. SQLite error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return parse_specs(pd);
}
