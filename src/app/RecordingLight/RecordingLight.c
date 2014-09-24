/*
 * RecordingLight.c
 *
 *  Created on: Jul 19, 2014
 *      Author: jonasbachli
 */

#include "RecordingLight.h"

#include "USART_SM.h"
#include "Settings.h"
#include "WS2812B.h"
#include "Tim1_Pwm.h"
#include "BinUtils.h"

/////////////////////////////////////
// forward declarations
/////////////////////////////////////

static void update(void);

static void handleCommand(RLMsg* msg);
static void sendResponse(RL_CMD cmd, uint32_t arg);

static void resetStateMachine();
static void setState(RLMsg* msg);
static void getState(RLMsg* msg);
static void setColor(RLMsg* msg);
static void getColor(RLMsg* msg);
static void sendInfo(RLMsg* msg);

void setLedColor(uint32_t rgba);

/////////////////////////////////////
// private variables
/////////////////////////////////////

static RLState currentState;
static Settings settings;

/////////////////////////////////////
// public functions
/////////////////////////////////////

void RecordingLight_init() {
	//WS2812B_init();
	//WS2812B_start();

	Tim1_Pwm_init();

	USART_SM_init();

	Settings_read(&settings);

	currentState = RLState_idle;
	update();
}

void RecordingLight_tick(uint32_t timestamp) {
	RLMsg msg;

	USART_SM_tick();
	if (USART_SM_getMessage(&msg)) {
		handleCommand(&msg);
	}
}

/////////////////////////////////////
// private functions
/////////////////////////////////////
static void update(void) {
	switch (currentState) {
	case RLState_idle:
		setLedColor(settings.idleColor);
		break;
	case RLState_ready:
		setLedColor(settings.readyColor);
		break;
	case RLState_cue:
		setLedColor(settings.cueColor);
		break;
	case RLState_rec:
		setLedColor(settings.recColor);
		break;
	default:
		setLedColor(0);
		break;
	}
}

static void handleCommand(RLMsg* msg) {
	uint8_t cmd = msg->cmd;
	if ((cmd >= RL_CMD_SET_IDLE) && (cmd <= RL_CMD_SET_CUE)) {
		setState(msg);
	} else if (cmd == RL_CMD_GET_STATE) {
		getState(msg);
	} else if ((cmd >= RL_CMD_SET_IDLE_COL) && (cmd <= RL_CMD_SET_CUE_COL)) {
		setColor(msg);
	} else if ((cmd >= RL_CMD_GET_IDLE_COL) && (cmd <= RL_CMD_GET_CUE_COL)) {
		getColor(msg);
	} else if (cmd == RL_CMD_RESET) {
		resetStateMachine(msg);
	} else if (cmd == RL_CMD_PING){
		sendResponse(msg->cmd, 0);
	} else if (cmd == RL_CMD_INFO){
		sendInfo(msg);
	}
}

static void sendResponse(RL_CMD cmd, uint32_t arg) {
	uint8_t b0, b1, b2, b3;
	BinUtils_get4BytesFrom32Bit(arg, &b0, &b1, &b2, &b3);

	USART1_sendByte(RL_RESPONSE_HEADER);
	USART1_sendByte((uint8_t) cmd);
	USART1_sendByte(b0);
	USART1_sendByte(b1);
	USART1_sendByte(b2);
	USART1_sendByte(b3);
}

static void resetStateMachine() {
	currentState = RLState_idle;
	update();
	sendResponse(RL_CMD_RESET, RL_RESPONSE_OK);
}

static void setState(RLMsg* msg) {
	currentState = msg->cmd - RL_CMD_SET_IDLE;
	update();
	sendResponse(msg->cmd, RL_RESPONSE_OK);
}

static void getState(RLMsg* msg) {
	sendResponse(msg->cmd, currentState);
}

static void setColor(RLMsg* msg) {
	switch (msg->cmd) {
	case RL_CMD_SET_IDLE_COL:
		settings.idleColor = msg->arg;
		break;
	case RL_CMD_SET_READY_COL:
		settings.readyColor = msg->arg;
		break;
	case RL_CMD_SET_CUE_COL:
		settings.readyColor = msg->arg;
		break;
	case RL_CMD_SET_REC_COL:
		settings.readyColor = msg->arg;
		break;
	default:
		break;
	}
	update();
	Settings_write(&settings);
	sendResponse(msg->cmd, RL_RESPONSE_OK);
}

static void getColor(RLMsg* msg) {
	switch (msg->cmd) {
	case RL_CMD_GET_IDLE_COL:
		sendResponse(msg->cmd, settings.idleColor);
		break;
	case RL_CMD_GET_READY_COL:
		sendResponse(msg->cmd, settings.readyColor);
		break;
	case RL_CMD_GET_CUE_COL:
		sendResponse(msg->cmd, settings.cueColor);
		break;
	case RL_CMD_GET_REC_COL:
		sendResponse(msg->cmd, settings.recColor);
		break;
	default:
		break;
	}
}

static void sendInfo(RLMsg* msg){
	sendResponse(msg->cmd, 0);
}

void setLedColor(uint32_t rgba) {
	uint8_t r, g, b, a;
	RGB color;

	BinUtils_get4BytesFrom32Bit(rgba, &r, &g, &b, &a);

	color.red = r;
	color.green = g;
	color.blue = b;

	Tim1_Pwm_setValueCh1(r);
	Tim1_Pwm_setValueCh2(g);
	Tim1_Pwm_setValueCh3(b);
	//WS2812B_fillColor(&color);
}
