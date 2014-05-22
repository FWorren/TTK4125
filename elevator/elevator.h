#ifndef elevator
#define elevator

#include "orderLogic.h"
#include "elev.h"

typedef enum{RUN,WAIT,DOOR,STOPS,STOP_OBS,STATE_UNDEF} state_t;

typedef enum{START, NEW_ORDER, FLOOR_REACHED, OBSTR, STOP, UNDEF} event_t;

struct elevator_init();

int elevator_run(int **orderlist, event_t event, state_t *state, order_t head_order, order_t *prev_order);

int elevator_wait(int **orderlist, event_t event, state_t *state);

int elevator_door(int **orderlist, event_t event, state_t *state);

int elevator_stop_obstruction(int **orderlist, event_t event, state_t *state);

int elevator_stop(int **orderlist, event_t event, state_t *state);

int elevator_undef(event_t event, state_t state);

void elevator_clear_lights_current_floor(int current_floor);

void elevator_clear_all_lights( void );

void elevator_break(int direction);

#endif