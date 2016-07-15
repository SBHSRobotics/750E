/*
 * autonWriter.h
 *
 *  Created on: Nov 7, 2014
 *      Author: Matthew
 */

#ifndef AUTONWRITER_H_
#define AUTONWRITER_H_

void createCodeFile(void);
void writeTreeToFile(char *str);
void loadTreeFromStart(char *filePrefix);
void loadTreeFromFile(char *filePrefix);

size_t strlen ( const char * str );

#endif /* AUTONWRITER_H_ */
