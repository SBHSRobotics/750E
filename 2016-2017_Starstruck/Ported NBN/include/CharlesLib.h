//========================================================//
//         Charles Competition Library v2015.1.19
//========================================================//
#ifndef CHARLES_LIB_H_
#define CHARLES_LIB_H_
//===== LCD Menu =====//

//Menu Macros
#define MENU_MAIN 0
#define MENU_BATT 1
#define MENU_MST 2
#define MENU_MEMES 3
#define MENU_SHOOTER 4
#define MENU_POT 5

//Motor Macros
#define W_LF 1;
#define W_RF 2;
#define W_HF 3;
#define INTAKE 4;
#define S_L 5;
#define S_R 6;
#define CON 7;
#define W_HB 8;
#define W_LB 9;
#define W_RB 10;

//Sensor Ports
#define GYRO 2;
#define SEL 8;
#define QUADL 11;
#define QUADR 13;
#define SONAR 15;
#define SONAR_L 17;
#define SONAR_R 19;
//#define SQUAD_L 1; //I2C_1
//#define SQUAD_R 2; //I2C_2

//Power Expander
extern const unsigned char EXPANDER;

//Sensors
extern Encoder QuadL;
extern Encoder QuadR;
extern Ultrasonic Sonar;

//Functions
//Declared here because it's used outside of CharlesLib.c, used in multiple files
void joystickMap();

#endif
