#include <ncurses.h>
#include <stdlib.h>
#include "common.h"

int init_db();
int insert_contract(Contract *c);
int get_contracts(Contract **contracts, int *count);
int delete_contract(int id);

void init_ui();
void cleanup_ui();
void draw_menu(int highlight);
void display_contracts(Contract *contracts, int count);
void add_contract_form(Contract *c);
int delete_contract_prompt();

int main() {
    if (init_db() != 0) {
        return 1;
    }

    init_ui();

    int choice = 0;
    int highlight = 1;
    int c;

    while(1) {
        draw_menu(highlight);
        c = getch();

        switch(c) {
            case KEY_UP:
                if (highlight == 1) highlight = 4;
                else --highlight;
                break;
            case KEY_DOWN:
                if (highlight == 4) highlight = 1;
                else ++highlight;
                break;
            case 10: // Enter key
                choice = highlight;
                break;
            default:
                break;
        }

        if (choice != 0) {
            if (choice == 1) {
                Contract *contracts;
                int count;
                if (get_contracts(&contracts, &count) == 0) {
                    display_contracts(contracts, count);
                    if (contracts) free(contracts);
                }
            } else if (choice == 2) {
                Contract new_c;
                add_contract_form(&new_c);
                insert_contract(&new_c);
            } else if (choice == 3) {
                int id = delete_contract_prompt();
                if (id > 0) delete_contract(id);
            } else if (choice == 4) {
                break;
            }
            choice = 0; // Reset choice
        }
    }

    cleanup_ui();
    return 0;
}
