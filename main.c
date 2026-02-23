#include <stdlib.h>
#include "common.h"

int init_db();
int insert_contract(Contract *c);
int get_contracts(Contract **contracts, int *count);
int delete_contract(int id);

void init_ui();
void cleanup_ui();
void draw_menu(int highlight);
void display_contracts(Contract *contracts, int *count);
void add_contract_form(Contract *c);
int delete_contract_prompt();

