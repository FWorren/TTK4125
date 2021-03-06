#include "orderHandler.h"
#include "elevator.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main( void ) {
    // Initialize hardware
    if (!elev_init()) {
        printf(__FILE__ ": Unable to initialize elevator hardware\n");
        return 1;
    }
    printf("Press STOP button to stop elevator and exit program.\n");

    // Initialize elevator settings
    int **orderlist;
    orderlist = orderHandler_init();
    order_t prev_order, head_order;
    prev_order = elevator_init();
    elevator_clear_all_lights();
    state_t state = WAIT;
    event_t event = NO_ORDERS;
    clock_t start;
    
    // System is up to date, start elevator
    int system_active = 1;
    while (system_active){
        orderHandler_search_for_orders(orderlist, state);
        switch(event){
            case NEW_ORDER:
                event = elevator_run(orderlist, &state, &head_order, &prev_order);
                break;
            case NO_ORDERS:
                event = elevator_wait(orderlist, &state, &head_order, &prev_order);
                break;
            case FLOOR_REACHED:
                event = elevator_door(orderlist, &state, &head_order, &start);
                break;
            case STOP_OBS:
                event = elevator_stop_obstruction(&state);
                break;
            case STOP:
                event = elevator_stop(orderlist, &state, &head_order);
                break;
            case FLOOR_MISSED:
                event = elevator_undef(&head_order, &state);
                break;
        }
    }
    orderHandler_free_list(orderlist);  
    return 0;
}
