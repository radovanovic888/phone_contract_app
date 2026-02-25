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

int get_contracts(Contract **contracts, int *count) {
    sqlite3 *db;
    sqlite3_stmt *res;
    int rc = sqlite3_open(DB_NAME, &db);

    if (rc != SQLITE_OK) return 1;

    const char *sql = "SELECT count(*) FROM contracts;";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) { sqlite3_close(db); return 1; }
    sqlite3_step(res);
    *count = sqlite3_column_int(res, 0);
    sqlite3_finalize(res);

    if (*count == 0) {
        *contracts = NULL;
        sqlite3_close(db);
        return 0;
    }

    *contracts = malloc(sizeof(Contract) * (*count));
    sql = "SELECT id, phone_number, company, service_plan, start_date, end_date, monthly_bill FROM contracts;";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    int i = 0;
    while (sqlite3_step(res) == SQLITE_ROW) {
        (*contracts)[i].id = sqlite3_column_int(res, 0);
        strncpy((*contracts)[i].phone_number, (const char*)sqlite3_column_text(res, 1), 19);
        strncpy((*contracts)[i].company, (const char*)sqlite3_column_text(res, 2), 49);
        strncpy((*contracts)[i].service_plan, (const char*)sqlite3_column_text(res, 3), 99);
        strncpy((*contracts)[i].start_date, (const char*)sqlite3_column_text(res, 4), 10);
        const char *end_d = (const char*)sqlite3_column_text(res, 5);
        if (end_d) strncpy((*contracts)[i].end_date, end_d, 10);
        else (*contracts)[i].end_date[0] = '\0';
        (*contracts)[i].monthly_bill = sqlite3_column_double(res, 6);
        i++;
    }

    sqlite3_finalize(res);
    sqlite3_close(db);
    return 0;
}

int delete_contract(int id) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open(DB_NAME, &db);
    if (rc != SQLITE_OK) return 1;

    char sql[100];
    sprintf(sql, "DELETE FROM contracts WHERE id = %d;", id);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    sqlite3_close(db);
    return (rc == SQLITE_OK) ? 0 : 1;
}
