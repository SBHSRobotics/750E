/****************\
 * opcontrol.c: *
\****************/

#include "main.h"
#include <string.h>

void operatorControl() {
	//Initialize 20 char string for read
	char *a = malloc(sizeof(char) * 500);
	char *b = malloc(sizeof(char) * 500);
	char *c = malloc(sizeof(char) * 500);
	//Open Test file in read mode
	FILE* f = fopen("Test.txt","r");
	//Read file contents to str
	fgets(a, 2048, f);
	fgets(b, 2048, f);
	fgets(c, 2048, f);
	//Close file
	fclose(f);

	const char * rtttl[] = {a,b,c};

	speakerPlayArray(rtttl);
	delay(1000);
}
