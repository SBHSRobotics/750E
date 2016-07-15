/*
 * sensors.h
 *
 *  Created on: Sep 19, 2014
 *      Author: Matthew
 */

#ifndef SENSORS_H_
#define SENSORS_H_

typedef enum SensorType {
	GYROSCOPE,
	LINE_TRACKER,
	ULTRASONIC,
	SHAFT_ENCODER,
	BUMPER_SWITCH,
	LIMIT_SWITCH,
	POTENTIOMETER,
	ACCELEROMETER,
	TIME,
	IME
} SensorType;

typedef struct Sensor {
	SensorType type;
	int port;
	union {
		Encoder enc;
		Gyro gyr;
		Ultrasonic ult;
	};
} Sensor;

void startSensor(Sensor *s);
void shutdownSensor(Sensor *s);
Sensor createSensor(SensorType type, int port);
short getSensorValue(Sensor s);

#endif /* SENSORS_H_ */
