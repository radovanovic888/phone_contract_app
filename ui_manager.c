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
    attroff(COLOR_PAIR(2) | A_BOLD);
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

