#include "ui_msg.h"

void UI_DisplayMenu(UART* handle) {
    UART_Print(handle, "--------------------------------\r\n");
    UART_Print(handle, "-----------GreenWatch-----------\r\n");
    UART_Print(handle, "--------------------------------\r\n");
    UART_Print(handle, "[1] - Temperature Report\r\n");
    UART_Print(handle, "[2] - Pressure Report\r\n");
    UART_Print(handle, "[3] - Ambient Light Report\r\n");
    UART_Print(handle, "[4] - Settings\r\n");
    UART_Print(handle, "[X] - Turn off\r\n");
    UART_Print(handle, "--------------------------------\r\n");
}

void UI_DebugWelcome(UART* handle) {
    UART_Print(handle, "----------------------------------------\r\n");
    UART_Print(handle, "GreenWatch - Debug Interface Initialised\r\n");
    UART_Print(handle, "App built on: " __DATE__ " " __TIME__ "\r\n");
    UART_Print(handle, "----------------------------------------\r\n");
}