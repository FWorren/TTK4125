#include "orderHandler.h"
#include "elevator.h"
#include "elev.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int **orderHandler_init( void ){
	int **orderlist;
	orderlist = (int**)malloc(N_BUTTONS*sizeof(int*));
	int i,j,k;
	for (i = 0; i < N_FLOORS; i++){
		orderlist[i] = (int*)malloc(N_FLOORS*sizeof(int));
	}
	for (j = 0; i < N_BUTTONS; j++){
		for (k = 0; k < N_FLOORS; k++){
			orderlist[j][k] = 0;
		}
	}
	return orderlist;	
}

void orderHandler_delete_all_orders(int **orderlist){
	int i,k;
	for (i = 0; i < N_BUTTONS; i++){
		for (k = 0; k < N_FLOORS; k++){
			orderlist[i][k] = 0;
		}
	}
}

void orderHandler_free_list(int **orderlist){
	int i;
	for (i = 0; i < N_BUTTONS; i++){
		free(orderlist[i]);
	}
	free(orderlist);
}

void orderHandler_search_for_orders(int **orderlist, state_t state){
	int i;
	for (i = 0; i < N_FLOORS; i++){
		if (elev_get_button_signal(BUTTON_COMMAND,i) && !orderlist[BUTTON_COMMAND][i]){
			orderlist[BUTTON_COMMAND][i] = 1;
			elev_set_button_lamp(BUTTON_COMMAND,i,1);
		}else if (elev_get_button_signal(BUTTON_CALL_UP,i) && !orderlist[BUTTON_CALL_UP][i] && i > 0 && state != STOPPED){
			orderlist[BUTTON_CALL_UP][i] = 1;
			elev_set_button_lamp(BUTTON_CALL_UP,i,1);
		}else if (elev_get_button_signal(BUTTON_CALL_DOWN,i) && !orderlist[BUTTON_CALL_DOWN][i] && i < N_FLOORS-1 && state != STOPPED){
			orderlist[BUTTON_CALL_DOWN][i] = 1;
			elev_set_button_lamp(BUTTON_CALL_DOWN,i,1);
		}
	}
}

int orderHandler_get_number_of_orders(int **orderlist){
	int i,k;
	int numb = 0;
	for (i = 0; i < N_BUTTONS; i++){
		for (k = 0; k < N_FLOORS; k++){
			if (orderlist[i][k]){
				numb += 1;
			}
		}
	}
	return numb;
}

void orderHandler_delete_order(int **orderlist, order_t *head_order){
	orderlist[BUTTON_COMMAND][(*head_order).floor] = 0;
	if ((*head_order).dir == 1)
		orderlist[BUTTON_CALL_UP][(*head_order).floor] = 0;
	else
		orderlist[BUTTON_CALL_DOWN][(*head_order).floor] = 0;
}

order_t orderHandler_set_head_order(int **orderlist, order_t *prev_order){
	order_t head_order;
	switch ((*prev_order).dir){
		case -1:
			head_order = orderHandler_state_down(orderlist,prev_order);
			if (head_order.floor == -1){
				head_order = orderHandler_state_up(orderlist,prev_order);
			}
			break;
		case 1:
			head_order = orderHandler_state_up(orderlist, prev_order);
			if (head_order.floor == -1){
				head_order = orderHandler_state_down(orderlist,prev_order);
			}
			break;
	}
	return head_order;
}

order_t orderHandler_state_up(int **orderlist, order_t *prev_order){
	order_t head_order;
	int i,k;
	for (i = 0; i < N_BUTTONS; i++){
		for (k = (*prev_order).floor; k < N_FLOORS; k++){
			if (orderlist[i][k]){
				head_order.floor = k;
				head_order.dir = 1;
				return head_order;
			}
		}
	}
	head_order.floor = -1;
	return head_order;
}

order_t orderHandler_state_down(int **orderlist, order_t *prev_order){
	order_t head_order;
	int i,k;
	for (i = 0; i < N_BUTTONS; i++){
		for (k = (*prev_order).floor; k >= 0; k--){
			if (orderlist[i][k]){
				head_order.floor = k;
				head_order.dir = -1;
				return head_order;
			}
		}
	}
	head_order.floor = -1;
	return head_order;
}

int orderHandler_check_current_floor(int **orderlist, int current_floor, int direction){
	if (direction == 1 && orderlist[BUTTON_CALL_UP][current_floor])
		return 1;
	else if (direction == -1 && orderlist[BUTTON_CALL_DOWN][current_floor])
		return 1;
	else if (orderlist[BUTTON_COMMAND][current_floor])
		return 1;
	else
		return 0;
}
