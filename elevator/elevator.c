#include "orderHandler.h"
#include "elevator.h"
#include "elev.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void elevator_clear_lights_current_floor(int current_floor){
	elev_set_button_lamp(BUTTON_COMMAND, current_floor, 0);
	if(current_floor < N_FLOORS - 1)
		elev_set_button_lamp(BUTTON_CALL_UP, current_floor, 0);
	if(current_floor > 0)
		elev_set_button_lamp(BUTTON_CALL_DOWN, current_floor, 0);
}

void elevator_clear_all_lights( void ){
	elev_set_door_open_lamp(0);
	elev_set_stop_lamp(0);
	int i;	
	for(i = 0; i < N_FLOORS; i++){
		elev_set_button_lamp(BUTTON_COMMAND, i, 0);
		if(i > 0)
			elev_set_button_lamp(BUTTON_CALL_DOWN, i, 0);
		if(i < N_FLOORS - 1)
			elev_set_button_lamp(BUTTON_CALL_UP, i, 0);
	}
}

void elevator_break(int direction){
	elev_set_speed(20 * (-direction));
	clock_t start = clock();
	clock_t finish;
	float interval = 0.0;
	float sec = 0.3;
	while (interval < sec){
		finish = clock();
		interval = (float)((finish - start)/(CLOCKS_PER_SEC));
	}
	elev_set_speed(0);
}

order_t elevator_init() {
	int current_floor = elev_get_floor_sensor_signal();
	if (current_floor != -1){
		elev_set_floor_indicator(current_floor);
		order_t current_pos;
		current_pos.floor = current_floor;
		current_pos.dir = -1;
		return current_pos;
	}
	elev_set_speed(-300);
	while (current_floor == -1){
		current_floor = elev_get_floor_sensor_signal();
	}
	elev_set_floor_indicator(current_floor);
	elevator_break(-1);
	order_t current_pos;
	current_pos.floor = current_floor;
	current_pos.dir = -1;
	return current_pos;
}

event_t elevator_wait(int **orderlist, state_t *state, order_t *head_order, order_t prev_order){
	if (*state != WAIT)
		*state = WAIT;	
	if (orderHandler_get_number_of_orders(orderlist) > 0){
		*head_order = orderHandler_set_head_order(orderlist, prev_order);
		if ((*head_order).floor == prev_order.floor){
			return FLOOR_REACHED;
		}
		return NEW_ORDER;
	}
	return NO_ORDERS;
}

event_t elevator_run(int **orderlist, state_t *state, order_t *head_order, order_t *prev_order){
	if (*state != RUN){
		*state = RUN;
		elev_set_speed(300*(*head_order).dir);	
	}
	int current_floor = elev_get_floor_sensor_signal();
	if (current_floor != -1){
		elev_set_floor_indicator(current_floor);
		if (orderHandler_check_current_floor(orderlist, current_floor, (*head_order).dir)){
			(*head_order).floor = current_floor;
			elevator_break((*head_order).dir);
			return FLOOR_REACHED;
		}
	}
	if (current_floor == (*head_order).floor){
		elevator_break((*head_order).dir);
		return FLOOR_REACHED;
	}
	if (elev_get_stop_signal()){
		elevator_break((*head_order).dir);
		return STOP;
	}
	if (elev_get_obstruction_signal()){
		elevator_break((*head_order).dir);
		return OBSTR;
	}
	return NEW_ORDER;
}

event_t elevator_door(int **orderlist, state_t *state, order_t *head_order){
	if (elev_get_floor_sensor_signal() != -1){
		*state = DOOR;
		clock_t start = clock();
		clock_t finish;
		float interval = 0.0;
		float sec = 3.0;
		elevator_clear_lights_current_floor((*head_order).floor);
		elev_set_door_open_lamp(1);
		orderHandler_delete_order(orderlist, (*head_order).floor);
		while (interval < sec){
			orderHandler_search_for_orders(orderlist, *state);
			if (elev_get_obstruction_signal()){interval = 0;}
			if(elev_get_stop_signal()){return STOP;}
			finish = clock();
			interval = (float)((finish - start)/(CLOCKS_PER_SEC));
		}
		order_t new_order = orderHandler_set_head_order(orderlist, *head_order);
		elev_set_door_open_lamp(0);
		if (new_order.floor == (*head_order).floor){
			return FLOOR_REACHED;	
		}else if (new_order.floor != -1){
			*head_order = new_order;
			return NEW_ORDER;
		}else{
			return NO_ORDERS;
		}
	}else{
		return UNDEF;
	}
}

event_t elevator_stop_obstruction(state_t *state){
	if (*state != STOP_OBS)
		*state = STOP_OBS;
	if (!elev_get_obstruction_signal())
		return NEW_ORDER;
	return OBSTR;
}

event_t elevator_stop(int **orderlist, state_t *state, order_t *head_order){
	if (*state != STOPPED){
		printf("The elevator has stopped! Press ctr + c to exit program or request a new order from command");
		*state = STOPPED;
		elevator_clear_all_lights();
		orderHandler_delete_all_orders(orderlist);
		elev_set_stop_lamp(1);
		if(elev_get_floor_sensor_signal() != -1)
		elev_set_door_open_lamp(1);
	}
	if (orderHandler_get_number_of_orders(orderlist) > 0){
		*head_order = orderHandler_set_head_order(orderlist, *head_order);
		elev_set_stop_lamp(0);
		return NEW_ORDER;
	}
	return STOP;
}

event_t elevator_undef(order_t head_order){
	return UNDEF;
}
