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

int insert_contract(Contract *c) {
    sqlite3 *db;
    sqlite3_stmt *res;
    int rc = sqlite3_open(DB_NAME, &db);

    if (rc != SQLITE_OK) return 1;

    const char *sql = "INSERT INTO contracts (phone_number, company, service_plan, start_date, end_date, monthly_bill) VALUES (?, ?, ?, ?, ?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK) {
        sqlite3_bind_text(res, 1, c->phone_number, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 2, c->company, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 3, c->service_plan, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 4, c->start_date, -1, SQLITE_STATIC);
        sqlite3_bind_text(res, 5, c->end_date, -1, SQLITE_STATIC);
        sqlite3_bind_double(res, 6, c->monthly_bill);
    } else {
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_step(res);
    sqlite3_finalize(res);
    sqlite3_close(db);

    return (rc == SQLITE_DONE) ? 0 : 1;
}
