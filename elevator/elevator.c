#include "elevator.h"
#include "elev.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct elevator_init() {
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

int elevator_wait(int **orderlist, event_t event, state_t *state){
	state = WAIT;
	while (event != NEW_ORDER){
		event = orderLogic_search_for_orders(orderlist, state);
	}
	return event;
}

int elevator_run(int **orderlist, event_t event, state_t *state, order_t head_order, order_t *prev_order){
	state = RUN;
	elev_set_speed(300*head_order.dir);
	while (event != FLOOR_REACHED){
		int current_floor = elev_get_floor_sensor_signal();
		if (current_floor != 0){
			elev_set_floor_indicator(current_floor);
			if (orderLogic_check_current_floor(orderlist, current_floor, head_order.dir)){
				elevator_break(head_order.dir);
				return FLOOR_REACHED;
			}
		}
		if (current_floor == head_order.floor){
			elevator_break(head_order.dir);
			return FLOOR_REACHED;
		}
		if (elev_get_stop_signal()){
			elevator_break(head_order.dir);
			return STOP;
		}
		if (elev_get_obstruction_signal()){
			elevator_break(head_order.dir);
			return OBSTR;
		}
		event = orderLogic_search_for_orders(orderlist, state);
	}
}

int elevator_door(int **orderlist, event_t event, state_t *state){
	if (elev_get_floor_sensor_signal() != -1){
		state = DOOR;
		clock_t start = clock();
		clock_t finish;
		float interval = 0.0;
		float sec = 3.0;
		elev_set_door_open_lamp(1);
		while (interval < sec){
			if (elev_get_obstruction_signal()){interval = 0;}
			if(elev_get_stop_signal()){return STOP;}
			finish = clock();
			interval = (float)((finish - start)/(CLOCKS_PER_SEC));
		}
		elev_set_door_open_lamp(0);
		return NEW_ORDER;
	}else{
		event = elevator_undef(event_t event, state_t state);
	}
}

int elevator_stop_obstruction(int **orderlist, event_t event, state_t *state){

}

int elevator_stop(int **orderlist, event_t event, state_t *state){
	elevator_clear_all_lights();
	orderLogic_delete_all_orders(orderlist);
	state = STOPS;
	elev_set_stop_lamp(1);
	if(elev_get_floor_sensor_signal() != -1){elev_set_door_open_lamp(1);}
	printf("The elevator has stopped! y/n to continue: ");
}

int elevator_undef(event_t event, state_t state){

}

void elevator_clear_lights_current_floor(int current_floor){
	elev_set_button_lamp(BUTTON_COMMAND, current_floor, 0);
	if(current_floor < N_FLOORS - 1){
		elev_set_button_lamp(BUTTON_CALL_UP, current_floor, 0);
	}
	if(current_floor > 0){
		elev_set_button_lamp(BUTTON_CALL_DOWN, current_floor, 0);
	}	
}

void elevator_clear_all_lights( void ){
	elev_set_door_open_lamp(0);
	elev_set_stop_lamp(0);
	int i;	
	for(i = 0; i < N_FLOORS; i++){
		elev_set_button_lamp(BUTTON_COMMAND, i, 0);
		if(i > 0){elev_set_button_lamp(BUTTON_CALL_DOWN, i, 0);}
		if(i < N_FLOORS - 1){elev_set_button_lamp(BUTTON_CALL_UP, i, 0);}
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