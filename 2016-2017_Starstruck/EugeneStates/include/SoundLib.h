#ifndef SOUNDLIB_H_
#define SOUNDLIB_H_

/*
 * Plays given sound in a separate task
 */
void playSoundAsync(const void *sound);

/*
* Plays low battery sound if battery is low
*/
void batteryCheck();

// Available Sounds:
extern const void *S_F;
extern const void *S_BFLAT;
extern const void *S_UP;
extern const void *S_SUPER_MARIO;
extern const void *S_STARTUP;
extern const void *S_CONNECTED;
extern const void *S_BATTERY_LOW;
extern const void *S_SUCCESS;
extern const void *S_FAIL;
extern const void *S_LINKIN_PARK_INTRO;
extern const void *S_LINKIN_PARK_CHORUS;
extern const void *S_NEVER_GONNA;
extern const void *S_NYAN;
extern const void *S_TAKE_ON_ME;
extern const void *S_WE_ARE_NUMBER_ONE;
extern const void *S_SUPER_MARIO_POLY;


#endif
