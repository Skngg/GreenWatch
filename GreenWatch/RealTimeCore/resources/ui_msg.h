#ifndef UI_MSG_H_
#define UI_MSG_H_

#include <stdbool.h>
#include <stdint.h>
#include "../lib/UART.h"
#include "../lib/Print.h"

void UI_DisplayMenu(UART* handle);

void UI_DebugWelcome(UART* handle);


#endif // #ifndef UI_MSG_H_
