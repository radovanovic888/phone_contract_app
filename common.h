#ifndef COMMON_H
#define COMMON_H

typedef struct {
    int id;
    char phone_number[20];
    char company[50];
    char service_plan[100];
    char start_date[11]; // YYYY-MM-DD
    char end_date[11];   // YYYY-MM-DD
    double monthly_bill;
} Contract;

#endif
