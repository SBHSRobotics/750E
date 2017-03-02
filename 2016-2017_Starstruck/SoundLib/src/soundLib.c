/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file contains functions corresponding to different sounds
 */

#include "main.h"

//This is where all of the sound functions are defined
void superMarioBro() {
	speakerPlayRtttl("Super Mario:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,8p.,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6");
}
void startUpSound() {
	speakerPlayRtttl("start: d=4, o=5, b=120: 8b, e, 8g#, b6");
}
void connectedSound() {
	speakerPlayRtttl("connect: d=4, o=5, b=120: 8c, 16a, 16g, 8e");
}
void lowBattery() {
	speakerPlayRtttl("Low Battery: d=4, o=7, b=120: 16b, 16b6, 8p, 16b, 16b6");
}
void success() {
	speakerPlayRtttl("success: d=4, o=5, b=120: 4c, 4f");
}
void fail() {
	speakerPlayRtttl("fail: d=4, o=5 b=120: 4d, 4c#, 2c");
}
void linkinParkIntro() {
	speakerPlayRtttl("linkin park intro: d=4, o=5, b=100: 4d, 4a6, 4a6, 4f, 4e, 4e, 4e, 8e, 8f, 4d, 4a6, 4a6, 4f, 4e, 4e, 4e, 8e, 8f, 4d, 4a6, 4a6, 4f, 4e, 4e, 4e, 8e, 8f, 4d, 4a6, 4a6, 4f, 2.e");
}
void linkinParkChorus() {//TODO: Fix this song, it is far from sounding right
	speakerPlayRtttl("linkin park chorus: d=4, o=5, b=100: c#, c#, d#, f, 8p, 8d#, 4.d#, 8.c#, 2c#, 16p, 16a#, 8c#, 8d#, 4.f, 8p, 8d#, 16f, 16d#, 8f, 8f#, 8f, d#");
}
void neverGonna() {
	speakerPlayRtttl("RickRoll:d=4,o=5,b=200:8g,8a,8c6,8a,e6,8p,e6,8p,d6.,p,8p,8g,8a,8c6,8a,d6,8p,d6,8p,c6,8b,a.,8g,8a,8c6,8a,2c6,d6,b,a,g.,8p,g,2d6,2c6.,p,8g,8a,8c6,8a,e6,8p,e6,8p,d6.,p,8p,8g,8a,8c6,8a,2g6,b,c6.,8b,a,8g,8a,8c6,8a,2c6,d6,b,a,g.,8p,g,2d6,2c6.");
}
void nyanCat() {
	speakerPlayRtttl("nyancat:d=4,o=5,b=90:16d#6,16e6,8f#6,8b6,16d#6,16e6,16f#6,16b6,16c#7,16d#7,16c#7,16a#6,8b6,8f#6,16d#6,16e6,8f#6,8b6,16c#7,16a#6,16b6,16c#7,16e7,16d#7,16e7,16c#7,8f#6,8g#6,16d#6,16d#6,16p,16b,16d6,16c#6,16b,16p,8b,8c#6,8d6,16d6,16c#6,16b,16c#6,16d#6,16f#6,16g#6,16d#6,16f#6,16c#6,16d#6,16b,16c#6,16b,8d#6,8f#6,16g#6,16d#6,16f#6,16c#6,16d#6,16b,16d6,16d#6,16d6,16c#6,16b,16c#6,8d6,16b,16c#6,16d#6,16f#6,16c#6,16d#6,16c#6,16b,8c#6,8b,8c#6,8f#6,8g#6,16d#6,16d#6,16p,16b,16d6,16c#6,16b,16p,8b,8c#6,8d6,16d6,16c#6,16b,16c#6,16d#6,16f#6,16g#6,16d#6,16f#6,16c#6,16d#6,16b,16c#6,16b,8d#6,8f#6,16g#6,16d#6,16f#6,16c#6,16d#6,16b,16d6,16d#6,16d6,16c#6,16b,16c#6,8d6,16b,16c#6,16d#6,16f#6,16c#6,16d#6,16c#6,16b,8c#6,8b,8c#6,8b,16f#,16g#,8b,16f#,16g#,16b,16c#6,16d#6,16b,16e6,16d#6,16e6,16f#6,8b,8b,16f#,16g#,16b,16f#,16e6,16d#6,16c#6,16b,16f#,16d#,16e,16f#,8b,16f#,16g#,8b,16f#,16g#,16b,16b,16c#6,16d#6,16b,16f#,16g#,16f#,8b,16b,16a#,16b,16f#,16g#,16b,16e6,16d#6,16e6,16f#6,8b,8a#,8b,16f#,16g#,8b,16f#,16g#,16b,16c#6,16d#6,16b,16e6,16d#6,16e6,16f#6,8b,8b,16f#,16g#,16b,16f#,16e6,16d#6,16c#6,16b,16f#,16d#,16e,16f#,8b,16f#,16g#,8b,16f#,16g#,16b,16b,16c#6,16d#6,16b,16f#,16g#,16f#,8b,16b,16a#,16b,16f#,16g#,16b,16e6,16d#6,16e6,16f#6,8b,8c#6");
}
void weAreNumberOne() {//TODO: Fix the first part of this song, sounds weird
	speakerPlayRtttl("we are number one: d=4, o=5, b=150: f4, 8p, 8c, 16b, 16c, 16b, 16c, 8b, 8c, b#4, f4, 8p, 8f4, 8b#4, 8c, c#, b#4, c#, d#, 8c, 8c#, 8c, 8d#, 8c, 8p, c6");
}
void superMarioBros() {
	const char * rtttl[] = {
		"Super Mario:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,8p.,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6"
		,// End track 1
	    "Super Mario:d=4,o=4,b=100:16d,16d,32p,8d,16d,8d,8g5,8p,8g,8p,8g5,16p,8e,16p,8c,16p,8f,8g,16f#,8f,16e.,16c5,16e5,8f5,16d5,8e5,8c5,16a,16b,8g,16p,8g5,16p,8e,16p,8c,16p,8f,8g,16f#,8f,16e.,16c5,16e5,8f5,16d5,8e5,8c5,16a,16b,8g,16p,8c,16p,16g,8p,8c5,8f,16p,16c5,16c5,16c5,8f,8c,16p,16e,8p,16g,16c5,p.,8g,8c,16p,16g,8p,8c5,8f,16p,16c5,16c5,16c5,8f,8c,16g#,8p,16a#,8p,16c5"
	    ,// End track 2
	    NULL
	    // No track 3 (NULL element indicates that there are no more tracks)
	};
	speakerPlayArray(rtttl);
}

//This is where the battery check funtion is defined. It checks the battery and if it is 20% or under, it will play a low battery sound
void batteryCheck() {
	if(powerLevelMain() <= 1440) {
		lowBattery();
	}
}
