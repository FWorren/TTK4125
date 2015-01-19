#ifndef types
#define types

typedef struct order_t {
	int floor;
	int dir;
}order_t;

typedef enum{RUN,WAIT,DOOR,STOPPED,STOP_OBS} state_t;

typedef enum{NEW_ORDER, NO_ORDERS, FLOOR_REACHED, OBSTR, STOP, UNDEF} event_t;

#endif