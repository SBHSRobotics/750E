
#include "main.h"

extern Ultrasonic front;
extern Ultrasonic back;
extern Encoder encoder;
extern Gyro gyro;
extern TaskHandle autonSelectorTask;

extern int autonPath;

//Directions
	//Respective
	#define FORWARD 0
	#define RIGHT 90
	#define BACKWARD 180
	#define LEFT 270
	#define CLOCKWISE 1
	#define RIGHT_TURN 1
	#define COUNTERCLOCKWISE -1
	#define LEFT_TURN -1
	//Compass
	#define NORTH 0
	#define EAST 90
	#define SOUTH 180
	#define WEST 270
	//Lift
	#define UP 1
	#define STOP 0
	#define DOWN -1

//Port Definitions
	//Wheels
	#define FL 9
	#define FR 8
	#define BL 3
	#define BR 2

	//Lift
	#define LB 4//D
	#define LF 5//C
	#define RB 6//B
	#define RF 7//A

	#define LT 1
	#define RT 10

	//Sensors
	#define IME_LEFT 0
	#define IME_RIGHT 1
	#define POT_LEFT 3
	#define POT_RIGHT 4
#define AUTON_POT 2
#define POT_MAX_VALUE 4095
#define SECTIONS 8.0
	#define LCD uart2
#define LINE_LEFT 1
#define LINE_RIGHT 2
#define ULTRA_FRONT_1 8
#define ULTRA_FRONT_2 9
#define ULTRA_BACK_1 11
#define ULTRA_BACK_2 12
#define ENCODER_1 3
#define ENCODER_2 4

#define RED_CUBE_1 0
#define RED_CUBE_2 1
#define RED_CUBE_3 2
#define RED_SKYRISE 3
#define BLUE_CUBE_1 4
#define BLUE_CUBE_2 5
#define BLUE_CUBE_3 6
#define BLUE_SKYRISE 7

	//Pneumatics
	#define SKYRISE_IN 5
	#define CUBE_IN 6

//Constants
	#define LIFT_MAXTILT 200
	#define ANALOG_THRESH 20
	#define MAX_SPEED_DIF 7

	int pid_arm(float measurement, float target);
	int pid_wheels(float measurement, float target);
	int pid_arm_right(float measurement, float target);

	void armTo(float value);
	void goToLine(int dir);
	void ultrasonicTo(int frontback, float value, float heading);
	void encoderTo(int dir, float value);
	void gyroTo(float value);
	void releaseCubes();

	void autonSelector();

	//Pre-init crash fix
	void imeReinitialize();

	//Returns value of ime and saves to eLeft or eRight
	//Parameter port should be IME_LEFT or IME_RIGHT
	int imeRead(int port);

/*TASK MANAGEMENT*/
	//Running a function in a separate thread
	/*If your function requires parameters, you must make a function to create the task
	 * void myTask(int foo){
	 *	  void f(){
	 *	    motorSet(1,foo);
	 *	  }
	 *	  createAsyncTask(f);
	 * }
	 */

	//Create new async task
	TaskHandle createAsyncTask(void code());

	//Pause a task based on a condition
	void pauseTaskUntil(TaskHandle task, bool pauseCondition);

	//Stop and delete a task
	void killTask(TaskHandle task);


/*MOVEMENT*/
	//Set the speed of a motor with acceleration ramping
	void setMotor(unsigned char channel, int speed);

	//Same as move() but with a custom speed
	void smove(int direction, int milliseconds, int speed);

	//Moves in a given direction at max speed for given amount of time
	void move(int direction, int milliseconds);

	//Set the speed of all drive motors to zero
	void stop();

/*LIFT*/
	//Same as lift, but with custom speed
	void slift(int dir, int time, int speed);

	//Moves scissor lift; dir is UP, DOWN, or STOP
	void lift(int dir, int time);

	//Stops the scissor lift
	void stopLift();

/*FILE IO*/
	//Send string of text content[] to file at path filename[]
	void fsend(char fileName[], char content[]);

	//Get string of text from file start to pos
	char fgetChar(char fileName[], int pos);

/*TELEOP*/
	//Map joysticks to chasis
	void driveMap(int stick);

	//Map the triggers/buttons to the lift
	void liftMap(int stick);

	//Map pneumatics to left stick on one joystick
	void pistonMap(int stick);
/*SELECTOR*/

