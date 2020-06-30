#ifndef UI_MSG_H_
#define UI_MSG_H_

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../lib/UART.h"
#include "../lib/Print.h"
#include "../lib/ADC.h"
#include "utilities.h"
#include "LPS22HH.h"

typedef struct {
    uint8_t mainMenu;
    uint8_t subMenu;
    void (*Callback)(UART*);
    bool refreshMenu;
} currentMenu;

void updateMenuCallback(currentMenu* handle);

void UART_ClearTerminal(UART* handle);

void UI_DisplayMenu(UART* handle);

void UI_TempReportCurrent(UART* handle);

void UI_TempReportInterval(UART* handle);

void UI_PressureReportCurrent(UART* handle);

void UI_PressureReportInterval(UART* handle);

void UI_LightReportCurrent(UART* handle);

void UI_LightReportInterval(UART* handle);

void UI_FullReportCurrent(UART* handle);

void UI_Settings(UART* handle);

void UI_SettingsInterval(UART* handle);

void UI_SettingsLogSize(UART* handle);

void UI_DebugWelcome(UART* handle);


#endif // #ifndef UI_MSG_H_
