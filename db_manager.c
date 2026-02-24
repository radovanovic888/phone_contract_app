#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include "common.h"

#define DB_NAME "phone_contracts.db"

int init_db() {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(DB_NAME, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS contracts ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "phone_number TEXT NOT NULL UNIQUE,"
                      "company TEXT NOT NULL,"
                      "service_plan TEXT NOT NULL,"
                      "start_date TEXT NOT NULL,"
                      "end_date TEXT,"
                      "monthly_bill REAL NOT NULL);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}
