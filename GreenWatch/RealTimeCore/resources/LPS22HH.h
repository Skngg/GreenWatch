#ifndef LPS22HH_H_
#define LPS22HH_H_

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../lib/I2CMaster.h"

// Variable names and comments come from the LPS22HH datasheet, which can be found here:
// https://www.st.com/resource/en/datasheet/lps22hh.pdf

/// <summary>This enum contains a set of registers that are used to control the behaviour of the LPS22HH device.</summary>
typedef enum {
    LPS22HH_REG_INTERRUPT_CFG = 0x0B,
    LPS22HH_REG_THS_P_L = 0x0C,
    LPS22HH_REG_THS_P_H = 0x0D,
    LPS22HH_REG_IF_CTRL = 0x0E,
    LPS22HH_REG_WHO_AM_I = 0x0F,
    LPS22HH_REG_CTRL_REG1 = 0x10,
    LPS22HH_REG_CTRL_REG2 = 0x11,
    LPS22HH_REG_CTRL_REG3 = 0x12,
    LPS22HH_REG_FIFO_CTRL = 0x13,
    LPS22HH_REG_FIFO_WTM = 0x14,
    LPS22HH_REG_REF_P_L = 0x15,
    LPS22HH_REG_REF_P_H = 0x16,
    LPS22HH_REG_PRDS_L = 0x18,
    LPS22HH_REG_PRDS_H = 0x19,
    LPS22HH_REG_INT_SOURCE = 0x24,
    LPS22HH_REG_FIFO_STATUS1 = 0x25,
    LPS22HH_REG_FIFO_STATUS2 = 0x26,
    LPS22HH_REG_STATUS = 0x27,
    LPS22HH_REG_PRESS_OUT_XL = 0x28,
    LPS22HH_REG_PRESS_OUT_L = 0x29,
    LPS22HH_REG_PRESS_OUT_H = 0x2A,
    LPS22HH_REG_TEMP_OUT_L = 0x2B,
    LPS22HH_REG_TEMP_OUT_H = 0x2C,
    LPS22HH_REG_FIFO_DATA_OUT_PRESS_XL = 0x78,
    LPS22HH_REG_FIFO_DATA_OUT_PRESS_L = 0x79,
    LPS22HH_REG_FIFO_DATA_OUT_PRESS_H = 0x7A,
    LPS22HH_REG_FIFO_DATA_OUT_TEMP_L = 0x7B,
    LPS22HH_REG_FIFO_DATA_OUT_TEMP_H = 0x7C
} LPS22HH_reg_e;

/// <summary>Bit field description for register STATUS.</summary>
typedef union __attribute__((__packed__)) {
    struct __attribute__((__packed__)) {
        /// <summary>
        /// <para>Pressure new data available.</para>
        /// <para>false: no set of data available at barometer output</para>
        /// <para>true: a new set of data is available at barometer output.</para>
        /// </summary>
        bool     p_da : 1;
        /// <summary>
        /// <para>Temperature new data available.</para>
        /// <para>false: no set of data available at temperature sensor output</para>
        /// <para>true: a new set of data is available at temperature sensor output.</para>
        /// </summary>
        bool     t_da : 1;
        
        unsigned res_3_2 : 2;

        /// <summary>
        /// <para>Pressure data overrun.</para>
        /// <para>false: no overrun has occured</para>
        /// <para>true: a new set of data has overwriten the previous data at barometer output.</para>
        /// </summary>
        bool     p_or : 1;
        
        /// <summary>
        /// <para>Temperature data overrun.</para>
        /// <para>false: no overrun has occured</para>
        /// <para>true: a new set of data has overwriten the previous data at temperature sensor output.</para>
        /// </summary>
        bool     t_or : 1;
        
        unsigned res_7_6 : 2;
    };

    uint8_t mask;
} LPS22HH_status_t;

/// <summary>Bit field description for register CTRL_REG1.</summary>
typedef union __attribute__((__packed__)) {
    struct __attribute__((__packed__)) {
        /// <summary>
        /// <para>SPI Serial Interface Mode selection. Default value: false.</para>
        /// <para>false: 4-wire interface</para>
        /// <para>true: 3-wire interface.</para>
        /// </summary>
        bool sim : 1;
        /// <summary>
        /// <para>Block data update. Default value: false.</para>
        /// <para>false: Continous update </para>
        /// <para>true: Output registers not updated untill MSB and LSB have been read.</para>
        /// </summary>
        bool bdu : 1;
        /// <summary>
        /// <para>Low-pass configuration register. Default value: false.</para>
        /// </summary>
        bool lpfp_cfg : 1;
        /// <summary>
        /// <para>Enable low-pass filter on pressure data when Continuous mode is used. Default value: false.</para>
        /// <para>false: Low-pass filter disabled</para>
        /// <para>true: Low-pass filter enabled.</para>
        /// </summary>
        bool en_lpfp : 1;

        /// <summary>Output data rate selection. Default value: 000.</summary>
        /// <para>(000: one-shot; 001: 1 Hz; 010: 10 Hz; 011: 25 Hz; 100: 50 Hz; 101: 75 Hz; 110: 100 Hz; 111: 200 Hz).</para>
        unsigned odr : 3;

        unsigned null_7 : 1;
    };

    uint8_t mask;
} LPS22HH_ctrl_reg1_t;

/// <summary>This is  from the WHO_AM_I register. Its value is fixed at B3h.</summary>
static const uint8_t LPS22HH_WHO_AM_I = 0xB3;

/// <summary>
/// <para>This is a subordinate device Address. Its value is fixed at 5Ch.</para>
/// <para>SDO is tied to ground so the least significant bit of the address is zero.</para>
/// </summary>
static const uint8_t LPS22HH_ADDRESS = 0x5C;

/// <summary>
/// <para>The application must call this function to implement a software reset.</para>
/// <para>This is a necessary function which will typically be used to reset the LPS22HH device.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LPS22HH_Reset(I2CMaster* driver);

/// <summary>
/// <para>The application must call this function to validate the device id.</para>
/// <para>This is a testing function which will typically be used in the beginning of the communication of the master with the subordinate device.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LPS22HH_CheckWhoAmI(I2CMaster* driver);

/// <summary>
/// <para>The application must call this function to configure the sensor control register.</para>
/// <para>This is a function which will typically be used to configure the sensor of the subordinate device.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <param name="odr">Selects the output data rate.</param>
/// <param name="en_lpfp">Enables the low-pass filter for Continous mode.</param>
/// <param name="lpfp_cfg">Selects the low-pass filter configuration.</param>
/// <param name="bdu">Selects the block data update mode.</param>
/// <param name="sim">Selects the SPI Serial Interface Mode.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LPS22HH_Config(I2CMaster* driver, unsigned odr, bool en_lpfp, bool lpfp_cfg, bool bdu, bool sim);

/// <summary>
/// <para>The application must call this function to check if new data are available in the temperature and pressure sensors or if overrun occured.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <param name="t_or">Reads temperature sensor for overrun. false: no overrun has occured; true: a new data for temperature has overwritten the previous data.</param>
/// <param name="p_or">Reads pressure sensor for overrun. false: no overrun has occured; true: a new data for pressure has overwritten the previous data.</param>
/// <param name="t_da">Reads temperature sensor for new data. false: no new data is available; true: a set of new data is available.</param>
/// <param name="p_da">Reads pressure sensor for new data. false: no new data is available; true: a set of new data is available.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LPS22HH_Status(I2CMaster* driver, bool* t_or, bool* p_or, bool* t_da, bool* p_da);

/// <summary>
/// <para>The application must call this function to read the temperature sensor.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <param name="temp">Reads temperature sensor data.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LPS22HH_ReadTemp(I2CMaster* driver, int16_t* temp);

/// <summary>
/// <para>The application must call this function to read the temperature sensor.</para>
/// <para>This function is a wrapper around <see cref="LPS22HH_ReadTemp"/> which provides
/// human readable output in Celsius.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <param name="temp">Reads temperature sensor data in Celsius degrees.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LPS22HH_ReadTempCelsius(I2CMaster* driver, float_t* temp);

/// <summary>
/// <para>The application must call this function to read the pressure sensor.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <param name="pressure">Reads pressure sensor data.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LPS22HH_ReadPressure(I2CMaster* driver, int32_t* pressure);

/// <summary>
/// <para>The application must call this function to read the pressure sensor.</para>
/// <para>This function is a wrapper around <see cref="LPS22HH_ReadPressure"/> which provides
/// human readable output in hPa.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <param name="pressure">Reads pressure sensor data in hPa.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LPS22HH_ReadPressureHuman(I2CMaster* driver, float_t* pressure);

/// <summary>
/// <para>The application must call this function to initialize LPS22HH via its gateway.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to communicate with the host.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LPS22HH_OpenViaHost(I2CMaster* driver);


#endif // #ifndef LPS22HH_H_