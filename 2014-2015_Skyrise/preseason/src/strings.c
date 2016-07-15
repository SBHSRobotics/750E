/*
 * strings.c
 *
 *  Created on: Jul 7, 2014
 *      Author: Matthew
 */

#include "main.h"

char* readln(int maxLen) {
	static char word[100] = "";
	int currLen = 0;
	while (true) {
		char currChar = getchar();
		/*printf("%d", currChar);
		if (currChar == 271) {
			print("Key stroke detected!");
		}*/
		if (currChar == 13) { // enter
			word[currLen] = '\0';
			print("\n\r");
			break;
		} else if (currChar == 8 && currLen > 0) { // backspace
			word[--currLen] = '\0';
			print("\b \b");
		} else if (currLen < maxLen) {
			word[currLen++] = currChar;
			fputc(currChar, stdout);
		}
	}
	return word;
}
