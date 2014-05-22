#ifndef orderLogic
#define orderLogic

#include "types.h"
#include "elev.h"

int **orderLogic_init( void );

void orderLogic_delete_all_orders(int **orderlist);

void orderLogic_free_list(int **orderlist);

void orderLogic_search_for_orders(int **orderlist, state_t *state);

int orderLogic_get_number_of_orders(int **orderlist);

order_t orderLogic_set_head_order(int **orderlist, order_t prev_order);

order_t orderLogic_state_up(int **orderlist, order_t prev_order);

order_t orderLogic_state_down(int **orderlist, order_t prev_order);

int orderLogic_check_current_floor(int **orderlist, int current_floor, int direction);

#endif
