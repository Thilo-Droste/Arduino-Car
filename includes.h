#define SPEED_STOP					0
#define SPEED_SLOW	 				128
#define SPEED_MODERATE				200
#define SPEED_FULL					255
//-------------------------------------------------
#define PIN_MOTORRECHTS_ENABLE 		2
#define PIN_MOTORRECHTS_POL1 		3
#define PIN_MOTORRECHTS_POL2		4

#define PIN_MOTORLINKS_ENABLE 		5
#define PIN_MOTORLINKS_POL1 		6
#define PIN_MOTORLINKS_POL2			7
//--------------------------------------------------
#define PIN_SONIC_VCC 				15
#define PIN_SONIC_GND				14
#define PIN_SONIC_TRIGGER			13
#define PIN_SONIC_ECHO 				12
//--------------------------------------------------
#define PIN_SERVO					11
//--------------------------------------------------
#define FORWARD                 	0
#define BACKWARD                	1
//--------------------------------------------------
#define stay                    	0
#define slow                    	1
#define moderate                	2
#define full                    	3
//--------------------------------------------------
#define left						1
#define half_left					2
#define half_right					3
#define right 						4
#define leftaround						5
#define rightaround						6
//--------------------------------------------------
#define STATE_UNDEFINED				0
#define STATE_DRIVE_FORWARD			1
#define STATE_STOP_MEASURE			2
#define STATE_TURN_LEFT				3
#define STATE_TURN_RIGHT			4
#define STATE_FINISHED        5
//--------------------------------------------------
#define STATE_SERVO_UNDEFINED		0
#define STATE_SERVO_TURNING_LEFT	1
#define STATE_SERVO_TURNING_RIGHT	2
#define STATE_SERVO_INPOSITION_LEFT	3
#define STATE_SERVO_INPOSITION_RIGHT 4
//--------------------------------------------------
#define MINIMUM_DIST              25


// Timer logik
#define TIMER_DECLARE(name) unsigned long g_timer_##name
#define TIMER_START(name, ms) g_timer_##name = millis() + (ms)
#define TIMER_ISDUE(name) (millis() > g_timer_##name) 
