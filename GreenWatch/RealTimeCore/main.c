﻿#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "lib/CPUFreq.h"
#include "lib/VectorTable.h"
#include "lib/NVIC.h"
#include "lib/GPIO.h"
#include "lib/GPT.h"
#include "lib/UART.h"
#include "lib/Print.h"
#include "lib/I2CMaster.h"

#include "resources/LSM6DSO.h"
#include "resources/LPS22HH.h"
#include "resources/ui_msg.h"

#define STARTUP_RETRY_COUNT  20
#define STARTUP_RETRY_PERIOD 500 // [ms]

static GPT* startUpTimer = NULL;

UART* uart_m4_debug = NULL;
static UART* uart_ui = NULL;

static I2CMaster* driver = NULL;

typedef struct CallbackNode {
	bool enqueued;
	struct CallbackNode* next;
	void (*cb)(void);
} CallbackNode;

static void EnqueueCallback(CallbackNode* node);

static void displaySensors_LSM()
{
	bool hasXL = false, hasG = false, hasTemp = false;

	// Wait for sensor board to be ready
	int32_t error;
	static bool initialised = false;
	uint32_t retryRemain = STARTUP_RETRY_COUNT;
	while (retryRemain > 0) {
		if (!LSM6DSO_Status(driver, &hasTemp, &hasG, &hasXL)) {
			UART_Print(uart_m4_debug, "ERROR: Failed to read accelerometer status register.\r\n");
		}
		if (hasTemp && hasG && hasXL) {
			initialised = true;
			break;
		}
		if ((error = GPT_WaitTimer_Blocking(
			startUpTimer, 500, GPT_UNITS_MILLISEC)) != ERROR_NONE) {
			UART_Printf(uart_m4_debug, "ERROR: Failed to start blocking wait (%ld).\r\n", error);
		}
		retryRemain--;
	}

	if (initialised) {
		float_t x, y, z;
		if (!hasXL) {
			UART_Print(uart_m4_debug, "INFO: No accelerometer data.\r\n");
		}
		else if (!LSM6DSO_ReadXLHuman(driver, &x, &y, &z)) {
			UART_Print(uart_m4_debug, "ERROR: Failed to read accelerometer data register.\r\n");
		}
		else {
			UART_Printf(uart_m4_debug, "INFO: Acceleration: [%.3f, %.3f, %.3f] * 10^-3 [g]\r\n",
				x, y, z);
		}

		if (!hasG) {
			UART_Print(uart_m4_debug, "INFO: No gyroscope data.\r\n");
		}
		else if (!LSM6DSO_ReadGHuman(driver, &x, &y, &z)) {
			UART_Print(uart_m4_debug, "ERROR: Failed to read gyroscope data register.\r\n");
		}
		else {
			UART_Printf(uart_m4_debug, "INFO: Gyroscope: [%.3f, %.3f, %.3f] * 10^-3 [dps]\r\n",
				x, y, z);
		}

		float_t t;
		if (!hasTemp) {
			UART_Print(uart_m4_debug, "INFO: No temperature data.\r\n");
		}
		else if (!LSM6DSO_ReadTempCelsius(driver, &t)) {
			UART_Print(uart_m4_debug, "ERROR: Failed to read temperature data register.\r\n");
		}
		else {
			UART_Printf(uart_m4_debug, "INFO: Temperature: %.3f [*C]\r\n", t);
		}
		UART_Print(uart_m4_debug, "\r\n");
	}
}

static void displaySensors_LPS()
{
	bool hasTemp = false, hasPressure = false, orTemp = false, orPressure = false;

	// Wait for sensor board to be ready
	int32_t error;
	static bool initialised = false;
	uint32_t retryRemain = STARTUP_RETRY_COUNT;
	while (retryRemain > 0) {
		if (!LPS22HH_Status(driver, &orTemp, &orPressure, &hasTemp, &hasPressure)) {
			UART_Print(uart_m4_debug, "ERROR: Failed to read sensor status register.\r\n");
		}
		if (hasTemp && hasPressure) {
			initialised = true;
			break;
		}
		if ((error = GPT_WaitTimer_Blocking(
			startUpTimer, 500, GPT_UNITS_MILLISEC)) != ERROR_NONE) {
			UART_Printf(uart_m4_debug, "ERROR: Failed to start blocking wait (%ld).\r\n", error);
		}
		retryRemain--;
	}

	if (initialised) {
		float_t temp, pressure;

		if (!hasTemp) {
			UART_Print(uart_m4_debug, "INFO: No temperature data.\r\n");
		}
		else if (!LPS22HH_ReadTempCelsius(driver, &temp)) {
			UART_Print(uart_m4_debug, "ERROR: Failed to read temperature sensor data register.\r\n");
		}
		else {
			UART_Printf(uart_m4_debug, "INFO: Temperature: %.3f [*C]\r\n", temp);
		}

		if (!hasPressure) {
			UART_Print(uart_m4_debug, "INFO: No barometric data.\r\n");
		}
		else if (!LPS22HH_ReadPressureHuman(driver, &pressure)) {
			UART_Print(uart_m4_debug, "ERROR: Failed to read pressure sensor data register.\r\n");
		}
		else {
			UART_Printf(uart_m4_debug, "INFO: Pressure: %.3f [hPa]\r\n", pressure);
		}

		UART_Print(uart_m4_debug, "\r\n");
	}
}

static CallbackNode* volatile callbacks = NULL;

static void EnqueueCallback(CallbackNode* node)
{
	uint32_t prevBasePri = NVIC_BlockIRQs();
	if (!node->enqueued) {
		CallbackNode* prevHead = callbacks;
		node->enqueued = true;
		callbacks = node;
		node->next = prevHead;
	}
	NVIC_RestoreIRQs(prevBasePri);
}

static void InvokeCallbacks(void)
{
	CallbackNode* node;
	do {
		uint32_t prevBasePri = NVIC_BlockIRQs();
		node = callbacks;
		if (node) {
			node->enqueued = false;
			callbacks = node->next;
		}
		NVIC_RestoreIRQs(prevBasePri);

		if (node) {
			(*node->cb)();
		}
	} while (node);
}

_Noreturn void RTCoreMain(void)
{
	//******************************************************************************************
	//************************************DELAY FOR OPENOCD*************************************
	volatile bool f = false;
	while (!f) {
		// empty.
	}
	//******************************************************************************************

	//******************************************************************************************
	//************************************BEGIN SYSTEM INIT*************************************

	VectorTableInit();
	CPUFreq_Set(26000000);

	// Open debugging UART and report status
	uart_m4_debug = UART_Open(MT3620_UNIT_UART_DEBUG, 115200, UART_PARITY_NONE, 1, NULL);
	if (uart_m4_debug != NULL) {
		UI_DebugWelcome(uart_m4_debug);
	}

	// Open UI UART and display menu
	uart_ui = UART_Open(MT3620_UNIT_ISU0, 115200, UART_PARITY_NONE, 1, NULL);
	if (uart_ui != NULL) {
		UI_DisplayMenu(uart_ui);
	}

	// Open and init startup timer
	if (!(startUpTimer = GPT_Open(MT3620_UNIT_GPT0, 1000, GPT_MODE_ONE_SHOT))) {
		UART_Print(uart_m4_debug, "ERROR: Opening startup timer\r\n");
	}

	// Open and setup I2C comm
	driver = I2CMaster_Open(MT3620_UNIT_ISU2);
	if (!driver) {
		UART_Print(uart_m4_debug,
			"ERROR: I2C initialisation failed\r\n");
	}
	I2CMaster_SetBusSpeed(driver, I2C_BUS_SPEED_STANDARD);

	// Verify connection for IMU, temp and pressure sensors and setup devices
	if (!LSM6DSO_CheckWhoAmI(driver)) {
		UART_Print(uart_m4_debug,
			"ERROR: CheckWhoAmI Failed for LSM6DSO.\r\n");
	}

	if (!LSM6DSO_Reset(driver)) {
		UART_Print(uart_m4_debug,
			"ERROR: Reset Failed for LSM6DSO.\r\n");
	}
	if (!LPS22HH_OpenViaHost(driver)) {
		UART_Print(uart_m4_debug,
			"ERROR: SHub Init Failed for LPS22HH.\r\n");
	}

	if (!LSM6DSO_ConfigXL(driver, 1, 4, false)) {
		UART_Print(uart_m4_debug,
			"ERROR: Failed to configure LSM6DSO accelerometer.\r\n");
	}

	if (!LSM6DSO_ConfigG(driver, 1, 500)) {
		UART_Print(uart_m4_debug,
			"ERROR: Failed to configure LSM6DSO accelerometer.\r\n");
	}

	displaySensors_LSM();

	if (!LPS22HH_CheckWhoAmI(driver)) {
		UART_Print(uart_m4_debug,
			"ERROR: CheckWhoAmI Failed for LPS22HH.\r\n");
	}

	if (!LPS22HH_Reset(driver)) {
		UART_Print(uart_m4_debug,
			"ERROR: Reset Failed for LPS22HH.\r\n");
	}

	if (!LPS22HH_Config(driver, 0x04, false, false, false, false)) {
		UART_Print(uart_m4_debug,
			"ERROR: Failed to configure LPS22HH.\r\n");
	}

	displaySensors_LPS();


	//*************************************END SYSTEM INIT**************************************
	//******************************************************************************************

	for (;;) {
		__asm__("wfi");
		InvokeCallbacks();
	}
}