#ifndef orderHandler
#define orderHandler

#include "types.h"
#include "elev.h"

int **orderHandler_init( void );

int orderHandler_get_number_of_orders(int **orderlist);

int orderHandler_check_current_floor(int **orderlist, int current_floor, int direction);

void orderHandler_delete_all_orders(int **orderlist);

void orderHandler_free_list(int **orderlist);

void orderHandler_search_for_orders(int **orderlist, state_t state);

void orderHandler_delete_order(int **orderlist, order_t head_order);

order_t orderHandler_set_head_order(int **orderlist, order_t prev_order);

order_t orderHandler_state_up(int **orderlist, order_t prev_order);

order_t orderHandler_state_down(int **orderlist, order_t prev_order);

#endif
