#include "main.h"
/*TASK MANAGEMENT*/
	//Create new async task
	//Run function in separate thread
	/*If your function requires parameters, you must make a function to create the task
	 * void myTask(int foo){
	 *	  void f(){
	 *	    motorSet(1,foo);
	 *	  }
	 *	  createAsyncTask(f);
	 * }
	 */
	TaskHandle createAsyncTask(void code());

	//Pause a task based on a condition
	void pauseTaskUntil(TaskHandle task, bool taskCondition);

	//Stop and delete a task
	void taskKill(TaskHandle task);


/*MOVEMENT*/
	//Constants
	#define FORWARD 90
	#define BACKWARD 270
	#define LEFT 180
	#define RIGHT 0
	#define MOTOR_THRESH 10

	//Port Definitions
	#define backLeft 8
	#define middleLeft 1
	#define frontLeft 2
	#define backRight 9
	#define middleRight 10
	#define frontRight 3

	//Displacement object for movement
	typedef struct{
		double x; //Horizontal displacement
		double y; //Vertical displacement
	}Displacement;

	//Set the speed of a motor using acceleration ramping
	void setMotor(unsigned char channel, int speed);

	//Strafe a given distance at a given angle (0=Straight right)
	void move(int angle, int distance, Encoder eLeft, Encoder eRight);

	//Get robot displacement using encoders (!!returns Displacement object!!)
	Displacement getDisplacement(Encoder eLeft, Encoder eRight);


/*FILE IO*/
	//Sends string to a file with the given name (fileName[] is string)
	void fsend(char fileName[], char content[]);

	//Retrieves character at a given position in file (0 to <fileLength>)
	char fgetChar(char fileName[], int pos);

