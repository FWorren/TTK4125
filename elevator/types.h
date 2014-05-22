#ifndef types
#define types

typedef struct order_t{
	int floor;
	int dir;
}order_t;

typedef enum{RUN,WAIT,DOOR,STOPS,STOP_OBS,STATE_UNDEF} state_t;

typedef enum{START, NEW_ORDER, FLOOR_REACHED, OBSTR, STOP, UNDEF} event_t;

#endif