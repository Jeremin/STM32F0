/*
 * ArgParser.h
 *
 *  Created on: Nov 2, 2014
 *      Author: jonasbachli
 */

#ifndef ARGPARSER_H_
#define ARGPARSER_H_

#include <stdint.h>
#include <stdbool.h>

typedef void (*onArgParsed)(char* arg, uint8_t argLength, uint8_t argIndex);

typedef struct _ArgParser {
	char* argStrStart;
	char* currentArgStr;
	uint8_t argStrLength;
	uint8_t currentArgIndex;
	uint8_t currentArgLength;
	onArgParsed onArgParsedFct;
} ArgParser;

void ArgParser_init(ArgParser* argParser, char* argStr, uint8_t argStrLength,
		onArgParsed onArgParsedFct);
void ArgParser_parse(ArgParser* argParser);

uint32_t ArgParser_getUint(char* str, uint8_t strLen);

#endif /* ARGPARSER_H_ */
