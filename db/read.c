#include <linux/limits.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "../package/package_specs.h"
#include "../output/o.h"

bool is_in_database(struct pkg_data *pd)
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

    sqlite3_bind_text(stmt, 1, pd->name, -1, SQLITE_STATIC);

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

/* some old code we won't need anymore

    void get_database_package_data(struct pkg_data *pd)
    {
        char db_path[strlen(VAR_PATH) + strlen("/packages.db") + 1];
        sprintf(db_path, "%s/packages.db", VAR_PATH);

        sqlite3 *db;
        sqlite3_stmt *stmt;
        if (sqlite3_open(db_path, &db))
        {
            perror("read.c:is_in_database: Error opening database");
            exit(1);
        }

        sqlite3_prepare_v2(db, "SELECT * FROM packages WHERE name = ?;", -1, &stmt, 0);
        sqlite3_bind_text(stmt, 1, pd->name, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            strcpy(pd->name, (char *)sqlite3_column_text(stmt, 0));
            strcpy(pd->version, (char *)sqlite3_column_text(stmt, 0));
            strcpy(pd->arch.name, (char *)sqlite3_column_text(stmt, 0));
            strcpy(pd->name, (char *)sqlite3_column_text(stmt, 0));
            strcpy(pd->name, (char *)sqlite3_column_text(stmt, 0));
        }
        else
        {
            perror("read.c:is_in_database: Error ret");
            exit(1);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }
*/
