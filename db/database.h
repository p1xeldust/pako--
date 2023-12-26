#ifndef DATABASE_H
#define DATABASE_H

    bool init_database();
    void add_package_to_database(struct pkg_data *pd, const char* listPath, const char* infoPath);
    bool is_in_database(const char* packageName);
    bool is_in_database(struct pkg_data *pd);

#endif
