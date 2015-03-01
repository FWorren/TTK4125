#ifndef types
#define types

typedef struct order_t {
	int floor;
	int dir;
}order_t;

typedef enum{RUN,WAIT,DOOR,STOPPED,OBSTRUCTION,UNDEF} state_t;

typedef enum{NEW_ORDER, NO_ORDERS, FLOOR_REACHED, STOP_OBS, STOP, FLOOR_MISSED} event_t;

#endif