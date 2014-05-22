#ifndef orderLogic
#define orderLogic

#include "elev.h"

typedef struct order_t{
	int floor;
	int dir;
	int set;
}order_t;

int **orderLogic_init( void );

void orderLogic_delete_all_orders(int **orderlist);

void orderLogic_free_list(int **orderlist);

int orderLogic_search_for_orders(int **orderlist);

int orderLogic_get_number_of_orders(int **orderlist);

struct orderLogic_set_head_order(int **orderlist, order_t prev_order);

struct orderLogic_state_up(int **orderlist, order_t prev_order);

struct orderLogic_state_down(int **orderlist, order_t prev_order);

int orderLogic_check_current_floor(int **orderlist, int current_floor, int direction);

#endif
