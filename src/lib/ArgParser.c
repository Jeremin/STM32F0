/*
 * ArgParser.c
 *
 *  Created on: Nov 2, 2014
 *      Author: jonasbachli
 */

#include "ArgParser.h"

/////////////////////////////////////
// forward declarations
/////////////////////////////////////

static void callOnArgParsed(ArgParser* argParser);

/////////////////////////////////////
// private variables
/////////////////////////////////////

/////////////////////////////////////
// public functions
/////////////////////////////////////

void ArgParser_init(ArgParser* argParser, char* argStr, uint8_t argStrLength,
		onArgParsed onArgParsedFct) {
	argParser->argStrStart = argStr;
	argParser->argStrLength = argStrLength;
	argParser->currentArgStr = argStr;
	argParser->currentArgIndex = 0;
	argParser->currentArgLength = 0;
	argParser->onArgParsedFct = onArgParsedFct;
}

void ArgParser_parse(ArgParser* argParser) {
	uint8_t i;
	for (i=0; i<argParser->argStrLength; i++){
		if (' ' == argParser->argStrStart[i]){
			callOnArgParsed(argParser);
			argParser->currentArgIndex++;
			argParser->currentArgLength = 0;
			argParser->currentArgStr = argParser->argStrStart + i + 1;
		}
		else{
			argParser->currentArgLength++;
		}
	}
	callOnArgParsed(argParser);
}

uint32_t ArgParser_getUint(char* str, uint8_t strLen){
	uint8_t i;
	uint32_t value = 0;
	for (i=0; i<strLen; ++i){
		if ('0' <= str[i] && '9' >= str[i]){
			value *= 10;
			value += str[i] - '0';
		}
		else
			return 0;
	}
	return value;
}

/////////////////////////////////////
// private functions
/////////////////////////////////////

static void callOnArgParsed(ArgParser* argParser){
	if (argParser->onArgParsedFct)
		argParser->onArgParsedFct(argParser->currentArgStr,
				argParser->currentArgLength, argParser->currentArgIndex);
}
