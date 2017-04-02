#ifndef HOLO_H_
#define HOLO_H_

#include <main.h>

/**
 * Initializes holo module in 4-wheel Holonomic-X mode
 *
 * @param FL Front left wheel port
 * @param BL Back left wheel port
 * @param FR Front right wheel port
 * @param BR Back right wheel port
 *
 */
void holoInitX4(unsigned char FL, unsigned char BL, unsigned char FR, unsigned char BR);

/**
 * Initializes holo module in 6-wheel Holonomic-X mode
 *
 * @param FL Front left wheel port
 * @param BL Back left wheel port
 * @param FR Front right wheel port
 * @param BR Back right wheel port
 * @param CL Center left wheel port
 * @param CR Center right wheel port
 *
 */
void holoInitX6(unsigned char FL, unsigned char BL, unsigned char FR, unsigned char BR, unsigned char CL, unsigned char CR);

/**
 * Initializes holo module in 5-wheel Holonomic-H mode
 *
 * @param FL Front left wheel port
 * @param BL Back left wheel port
 * @param FR Front right wheel port
 * @param BR Back right wheel port
 * @param H Horizontal wheel port
 *
 */
void holoInitH5(unsigned char FL, unsigned char BL, unsigned char FR, unsigned char BR, unsigned char H);

/**
 * Initializes holo module in 6-wheel Holonomic-H mode (2 horizontal wheels, front and back)
 *
 * @param FL Front left wheel port
 * @param BL Back left wheel port
 * @param FR Front right wheel port
 * @param BR Back right wheel port
 * @param HF Front horizontal wheel port
 * @param HB Back horizontal wheel port
 *
 */
void holoInitH6(unsigned char FL, unsigned char BL, unsigned char FR, unsigned char BR, unsigned char HF, unsigned char HB);

/**
 * Initializes holo module in 5-wheel 6-motor Holonomic mode (2 motors on 1 center wheel)
 *
 * @param FL Front left wheel port
 * @param BL Back left wheel port
 * @param FR Front right wheel port
 * @param BR Back right wheel port
 * @param HF Front horizontal wheel port
 * @param HB Back horizontal wheel port
 *
 */
void holoInitH56(unsigned char FL, unsigned char BL, unsigned char FR, unsigned char BR, unsigned char HF, unsigned char HB);

/**
 * Stops and resets holo module
 */
void holoStop();

/**
 * Returns true if holo module is initialized, false otherwise
 */
bool isHoloInitialized();

/**
 * Sets drive speeds based on active configuration
 */
void holoSet(int xAxis, int yAxis, int rot);

#endif //LCD_H_
