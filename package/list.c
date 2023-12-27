#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

#include "../db/database.h"
#include "parse.h"
#include "package_specs.h"
#include "../output/o.h"

// Имхо получился говнокод

void list_db_recursively(struct pkg_data *pd)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char db_path[strlen(VAR_PATH) + strlen("/packages.db") + 1];
    sprintf(db_path, "%s/packages.db", VAR_PATH);

    if (sqlite3_open(db_path, &db) != SQLITE_OK)
    {
        pk_error(0, "Error opening database. SQLite error: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    if (sqlite3_prepare_v2(db, "SELECT * FROM packages;", -1, &stmt, 0) != SQLITE_OK)
    {
        pk_error(0, "Error selecting package from db. SQLite error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        strcpy(pd->files.info_file_path, (char *)sqlite3_column_text(stmt, 1));
        parse_specs(pd);
        pk_msg("| %-16.*s | %-9.*s | %-9.*s | %-32.*s |\n",
               16, pd->name,
               8, pd->version,
               8, pd->arch.name,
               32, pd->desc);
        pk_msg("|__________________|___________|___________|__________________________________|\n");
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void pk_list_packages(int argc, char *argv[])
{
    struct pkg_data *pd = malloc(sizeof(struct pkg_data));
    if (pd == NULL)
    {
        pk_error(1, "Memory allocation error");
        exit(1);
    }
    pk_msg("._____________________________________________________________________________.\n");
    pk_msg("| %-16s | %-9s | %-9s | %-32s |\n", "Name", "Version", "Arch", "Comment");
    pk_msg("|------------------|-----------|-----------|----------------------------------|\n");

    if (argc > 2)
    {
        for (int i = 2; i < argc; i++)
        {
            if (!is_in_database(argv[i]))
            {
                pk_warn("Not found %s in database, skipping.\n", argv[i]);
                continue;
            }
            get_package_data_from_db(argv[i], pd);
            pk_msg("| %-16.*s | %-9.*s | %-9.*s | %-32.*s |\n",
                   16, pd->name,
                   8, pd->version,
                   8, pd->arch.name,
                   32, pd->desc);
            pk_msg("|__________________|___________|___________|__________________________________|\n");
        }
        return;
    }
    else
    {
        list_db_recursively(pd);
    }
    printf("\n");
    free(pd);
}

int main(int argc, char *argv[])
{
    init_database();
    pk_list_packages(argc, argv);
}