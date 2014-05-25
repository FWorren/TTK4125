#ifndef elevator
#define elevator

#include "types.h"
#include "elev.h"

void elevator_clear_lights_current_floor(int current_floor);

void elevator_clear_all_lights( void );

void elevator_break(int direction);

order_t elevator_init();

event_t elevator_run(int **orderlist, state_t *state, order_t *head_order, order_t *prev_order);

event_t elevator_wait(int **orderlist, state_t *state, order_t *head_order, order_t prev_order);

event_t elevator_door(int **orderlist, state_t *state, order_t *head_order);

event_t elevator_stop_obstruction(state_t *state);

event_t elevator_stop(int **orderlist, state_t *state, order_t *head_order);

event_t elevator_undef(order_t head_order);

#endif