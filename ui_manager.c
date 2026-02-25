#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"

void init_ui() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
}

void cleanup_ui() {
    endwin();
}

void draw_menu(int highlight) {
    int x =2, y = 2;
    const char *choices[] = {
        "1. View All Contracts",
        "2. Add New Contract",
        "3. Delete Contract",
        "4. Exit"
    };
    int n_choices = 4;

    clear();
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(y++, x, "=== Phone Contract Manager ===");
    attroff(COLOR_PAIR(1) | A_BOLD);
    y++;

    for (int i = 0; i < n_choices; ++i) {
        if (highlight == i + 1) {
            attron(A_REVERSE);
            mvprintw(y, x, "%s", choices[i]);
            attroff(A_REVERSE);
        } else {
            mvprintw(y, x, "%s", choices[i]);
        }
        y++;
    }
    refresh();
}

void display_contracts(Contract *contracts, int count) {
    clear();
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(1, 2, "%-15s %-15s %-15s %-12s %-12s %-10s", "Phone", "Company", "Plan", "Start", "End", "Bill");
    attroff(COLOR_PAIR(1) | A_BOLD);
    mvprintw(2, 2, "-------------------------------------------------------------------------------");

    for (int i = 0; i < count; i++) {
        mvprintw(3 + i, 2, "%-15s %-15s %-15s %-12s %-12s %-10.2f",
        contracts[i].phone_number, contracts[i].company, contracts[i].service_plan, 
        contracts[i].start_date, contracts[i].end_date, contracts[i].monthly_bill);
    }

    mvprintw(count + 5, 2, "Press any key to return to menu...");
    refresh();
    getch();
}

void get_input(const char *prompt, char *buffer, int max_len, int y, int x) {
    mvprintw(y, x, "%s", prompt);
    echo();
    curs_set(1);
    getnstr(buffer, max_len);
    noecho();
    curs_set(0);
}

void add_contract_form(Contract *c) {
    clear();
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(1, 2, "--- Add New Contract ---");
    attroff(COLOR_PAIR(1) | A_BOLD);

    get_input("Phone Number: ", c->phone_number, 19, 3, 2);
    get_input("Company:      ", c->company, 49, 4, 2);
    get_input("Service Plan: ", c->service_plan, 99, 5, 2);
    get_input("Start Date (YYYY-MM-DD): ", c->start_date, 10, 6, 2);
    get_input("End Date (YYYY-MM-DD):   ", c->end_date, 10, 7, 2);

    char bill_str[20];
    get_input("Monthly Bill: ", bill_str, 19, 8, 2);
    c->monthly_bill = atof(bill_str);

    mvprintw(10, 2, "Contract added! Press any key...");
    refresh();
    getch();
}

int delete_contract_prompt() {
    clear();
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(1, 2, "--- Delete Contract ---");
    attroff(COLOR_PAIR(1) | A_BOLD);

    char id_str[10];
    get_input("Enter ID to delete (view list first to see IDs): ", id_str, 9, 3, 2);
    return atoi(id_str);
}
