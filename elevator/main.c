#include "orderLogic.h"
#include "elevator.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main( void ){
	// Initialize hardware
	if (!elev_init()) {
		printf(__FILE__ ": Unable to initialize elevator hardware\n");
	    return 1;
	}
	printf("Press STOP button to stop elevator and exit program.\n");

	int **orderlist;
	orderlist = orderLogic_init();
	state_t state = STATE_UNDEF;
	event_t event = START;
	order_t prev_order;
	prev_order = elevator_init();
	elevator_clear_all_lights();
	
	//System is up to date, start elevator
	while (true){
		switch(event){
			case START:
				event = elevator_wait(orderlist,event,&state);
				break;
			case NEW_ORDER:
				order_t new_order = orderLogic_set_head_order(orderlist);
				if (!new_order.set){
					event = elevator_wait(orderlist,event,&state);
				}else{
					prev_order = new_order;
					event = elevator_run(orderlist,event,&state,new_order,&prev_order);
				}
				break;
			case FLOOR_REACHED:
				event = elevator_door(orderlist, event, &state);
				break;
			case OBSTR:
				event = elevator_stop_obstruction(orderlist, event, &state);
				break;
			case STOP:
				event = elevator_stop(orderlist, event, state);
				break;
			case UNDEF:

				break;
		}
	}	
	return 0;
}
