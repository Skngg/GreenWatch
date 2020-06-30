#include "ui_msg.h"

extern uint8_t sampleInterval;
extern uint8_t logSize;
extern ringBuffer_int16 temperatureLog;
extern ringBuffer_int16 pressureLog;
extern ringBuffer_uint32 lightLog;

extern I2CMaster* driver;
extern ADC_Data lightData[ADC_DATA_SIZE];

void updateMenuCallback(currentMenu* handle)
{
    switch (handle->mainMenu)
    {
    case 0:
        handle->Callback = &UI_DisplayMenu;
        break;
    case 1:
        handle->Callback = &UI_TempReportCurrent;
        break;
    case 2:
        handle->Callback = &UI_TempReportInterval;
        break;
    case 3:
        handle->Callback = &UI_PressureReportCurrent;
        break;
    case 4:
        handle->Callback = &UI_PressureReportInterval;
        break;
    case 5:
        handle->Callback = &UI_LightReportCurrent;
        break;
    case 6:
        handle->Callback = &UI_LightReportInterval;
        break;
    case 7:
        handle->Callback = &UI_FullReportCurrent;
        break;
    case 8:
        switch (handle->subMenu)
        {
        case 0:
            handle->Callback = &UI_Settings;
            break;
        case 1:
            handle->Callback = &UI_SettingsInterval;
            break;
        case 2:
            handle->Callback = &UI_SettingsLogSize;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    handle->refreshMenu = true;
}

void UART_ClearTerminal(UART* handle) {
    UART_Print(handle, "\033");
    UART_Print(handle, "[2J");
    UART_Print(handle, "\033");
    UART_Print(handle, "[H");
}

void UI_DisplayMenu(UART* handle) {
    UART_ClearTerminal(handle);
    UART_Print(handle, "--------------------------------------------\r\n");
    UART_Print(handle, "-----------------GreenWatch-----------------\r\n");
    UART_Print(handle, "--------------------------------------------\r\n");
    UART_Print(handle, "[1] - Temperature Report - Current\r\n");
    UART_Print(handle, "[2] - Temperature Report - Logged data\r\n");
    UART_Print(handle, "[3] - Pressure Report - Current\r\n");
    UART_Print(handle, "[4] - Pressure Report - Logged data\r\n");
    UART_Print(handle, "[5] - Ambient Light Report - Current\r\n");
    UART_Print(handle, "[6] - Ambient Light Report - Logged data\r\n");
    UART_Print(handle, "[7] - Full Report - Current\r\n");
    UART_Print(handle, "[8] - Settings\r\n");
    UART_Print(handle, "--------------------------------------------\r\n");
}

void UI_TempReportCurrent(UART* handle) {
    float_t temp = 0;
    LPS22HH_ReadTempCelsius(driver, &temp);
    UART_ClearTerminal(handle);
    UART_Print(handle, "------------------------------------------\r\n");
    UART_Printf(handle, "Temperature:   %.3f [*C]\r\n", temp);
    UART_Print(handle, "[X] - Go back\r\n");
    UART_Print(handle, "------------------------------------------\r\n");

}

void UI_TempReportInterval(UART* handle) {
    UART_ClearTerminal(handle);
    UART_Print(handle, "------------------------------------------\r\n");
    UART_Print(handle, "Temperature log:\r\n");
    UART_Print(handle, "------------------------------------------\r\n");
    
    uint8_t i = 0;
    int16_t temp = 0;
    for (i = 0; i < logSize; ++i) {
        ringBuffer_int16_Read(&temperatureLog, &temp);
        UART_Printf(handle, "T-%3d s:        %.3f [*C]\r\n", \
            ((logSize * sampleInterval) - (i * sampleInterval)), \
            (float_t)temp / 100.0f);
    }
    UART_Print(handle, "------------------------------------------\r\n");
    UART_Print(handle, "[X] - Go back\r\n");
    UART_Print(handle, "------------------------------------------\r\n");
}

void UI_PressureReportCurrent(UART* handle) {
    float_t press = 0;
    LPS22HH_ReadPressureHuman(driver, &press);
    UART_ClearTerminal(handle);
    UART_Print(handle, "------------------------------------------\r\n");
    UART_Printf(handle, "Pressure:      %.3f [hPa]\r\n", press);
    UART_Print(handle, "[X] - Go back\r\n");
    UART_Print(handle, "------------------------------------------\r\n");
}

void UI_PressureReportInterval(UART* handle) {
    UART_ClearTerminal(handle);
    UART_Print(handle, "------------------------------------------\r\n");
    UART_Print(handle, "Pressure log:\r\n");
    UART_Print(handle, "------------------------------------------\r\n");

    uint8_t i = 0;
    int32_t press = 0;
    for (i = 0; i < logSize; ++i) {
        ringBuffer_uint32_Read(&pressureLog, (uint32_t)&press);
        UART_Printf(handle, "T-%3d s:        %.3f [hPa]\r\n", \
            ((logSize * sampleInterval) - (i * sampleInterval)), \
            (float_t)press / 4096.0f);
    }
    UART_Print(handle, "------------------------------------------\r\n");
    UART_Print(handle, "[X] - Go back\r\n");
    UART_Print(handle, "------------------------------------------\r\n");
}

void UI_LightReportCurrent(UART* handle) {
    float_t V = ((float_t)(lightData[0].value) * 2.5f) / ADC_MAX_VAL;
    UART_ClearTerminal(handle);
    UART_Print(handle, "------------------------------------------\r\n");
    UART_Printf(handle, "Ambient light: %.3f [V]\r\n", V);
    UART_Print(handle, "[X] - Go back\r\n");
    UART_Print(handle, "------------------------------------------\r\n");
}

void UI_LightReportInterval(UART* handle) {
    UART_ClearTerminal(handle);
    UART_Print(handle, "------------------------------------------\r\n");
    UART_Print(handle, "Ambient light log:\r\n");
    UART_Print(handle, "------------------------------------------\r\n");

    uint8_t i = 0;
    uint32_t light = 0;
    for (i = 0; i < logSize; ++i) {
        ringBuffer_uint32_Read(&lightLog, &light);
        UART_Printf(handle, "T-%3d s:        %.3f [V]\r\n", \
            ((logSize * sampleInterval) - (i * sampleInterval)), \
            ((float_t)light * 2.5f) / ADC_MAX_VAL);
    }
    UART_Print(handle, "------------------------------------------\r\n");
    UART_Print(handle, "[X] - Go back\r\n");
    UART_Print(handle, "------------------------------------------\r\n");
}

void UI_FullReportCurrent(UART* handle) {
    float_t temp = 0, press = 0;
    LPS22HH_ReadTempCelsius(driver, &temp);
    LPS22HH_ReadPressureHuman(driver, &press);
    float_t V = ((float_t)(lightData[0].value) * 2.5f) / ADC_MAX_VAL;
    UART_ClearTerminal(handle);
    UART_Print(handle, "------------------------------------------\r\n");
    UART_Printf(handle, "Temperature:   %.3f [*C]\r\n", temp);
    UART_Printf(handle, "Pressure:      %.3f [hPa]\r\n", press);
    UART_Printf(handle, "Ambient light: %.3f [V]\r\n", V);
    UART_Print(handle, "[X] - Go back\r\n");
    UART_Print(handle, "------------------------------------------\r\n");
}

void UI_Settings(UART* handle) {
    UART_ClearTerminal(handle);
    UART_Print(handle, "------------------------------------------\r\n");
    UART_Print(handle, "[1] - Change logging interval\r\n");
    UART_Print(handle, "[2] - Change no. logged data display\r\n");
    UART_Print(handle, "[X] - Go back\r\n");
    UART_Print(handle, "------------------------------------------\r\n");
}

void UI_SettingsInterval(UART* handle) {
    UART_ClearTerminal(handle);
    UART_Print(handle, "------------------------------------------\r\n");
    UART_Printf(handle, "Current logging interval: %d s\r\n", sampleInterval);
    UART_Print(handle, "Enter logging interval in seconds.\r\n");
    UART_Print(handle, "Upon entering the right value you will \r\n" \
                       "be returned to the previous menu.\r\n");
    UART_Print(handle, "Value will be changed after assertion.\r\n");
    UART_Print(handle, "------------------------------------------\r\n");
}

void UI_SettingsLogSize(UART* handle) {
    UART_ClearTerminal(handle);
    UART_Print(handle, "------------------------------------------\r\n");
    UART_Printf(handle, "Current log size: %d samples\r\n", logSize);
    UART_Print(handle, "Enter size of logs in No. samples.\r\n");
    UART_Print(handle, "Upon entering the right value you will \r\n" \
                       "be returned to the previous menu.\r\n");
    UART_Print(handle, "Value will be changed after assertion.\r\n");
    UART_Print(handle, "------------------------------------------\r\n");
}
void UI_DebugWelcome(UART* handle) {
    UART_Print(handle, "----------------------------------------\r\n");
    UART_Print(handle, "GreenWatch - Debug Interface Initialised\r\n");
    UART_Print(handle, "App built on: " __DATE__ " " __TIME__ "\r\n");
    UART_Print(handle, "----------------------------------------\r\n");
}
