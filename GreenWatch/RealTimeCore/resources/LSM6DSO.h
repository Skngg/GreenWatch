#ifndef LSM6DSO_H_
#define LSM6DSO_H_

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../lib/I2CMaster.h"

// Variable names and comments come from the LSM6DSO datasheet, which can be found here:
// https://www.st.com/resource/en/datasheet/LSM6DSO.pdf

/// <summary>This enum contains a set of registers that are used to control the behaviour of the LSM6DSO device.</summary>
typedef enum {
	LSM6DSO_REG_FUNC_CFG_ACCESS = 0x01,
	LSM6DSO_REG_PIN_CTRL = 0x02,
	LSM6DSO_REG_FIFO_CTRL1 = 0x07,
	LSM6DSO_REG_FIFO_CTRL2 = 0x08,
	LSM6DSO_REG_FIFO_CTRL3 = 0x09,
	LSM6DSO_REG_FIFO_CTRL4 = 0x0A,
	LSM6DSO_REG_COUNTER_BDR_REG1 = 0x0B,
	LSM6DSO_REG_COUNTER_BDR_REG2 = 0x0C,
	LSM6DSO_REG_INT1_CTRL = 0x0D,
	LSM6DSO_REG_INT2_CTRL = 0x0E,
	LSM6DSO_REG_WHO_AM_I = 0x0F,
	LSM6DSO_REG_CTRL1_XL = 0x10,
	LSM6DSO_REG_CTRL2_G = 0x11,
	LSM6DSO_REG_CTRL3_C = 0x12,
	LSM6DSO_REG_CTRL4_C = 0x13,
	LSM6DSO_REG_CTRL5_C = 0x14,
	LSM6DSO_REG_CTRL6_C = 0x15,
	LSM6DSO_REG_CTRL7_G = 0x16,
	LSM6DSO_REG_CTRL8_XL = 0x17,
	LSM6DSO_REG_CTRL9_XL = 0x18,
	LSM6DSO_REG_CTRL10_C = 0x19,
	LSM6DSO_REG_ALL_INT_SRC = 0x1A,
	LSM6DSO_REG_WAKE_UP_SRC = 0x1B,
	LSM6DSO_REG_TAP_SRC = 0x1C,
	LSM6DSO_REG_D6D_SRC = 0x1D,
	LSM6DSO_REG_STATUS_REG = 0x1E,
	LSM6DSO_REG_OUT_TEMP_L = 0x20,
	LSM6DSO_REG_OUT_TEMP_H = 0x21,
	LSM6DSO_REG_OUTX_L_G = 0x22,
	LSM6DSO_REG_OUTX_H_G = 0x23,
	LSM6DSO_REG_OUTY_L_G = 0x24,
	LSM6DSO_REG_OUTY_H_G = 0x25,
	LSM6DSO_REG_OUTZ_L_G = 0x26,
	LSM6DSO_REG_OUTZ_H_G = 0x27,
	LSM6DSO_REG_OUTX_L_A = 0x28,
	LSM6DSO_REG_OUTX_H_A = 0x29,
	LSM6DSO_REG_OUTY_L_A = 0x2A,
	LSM6DSO_REG_OUTY_H_A = 0x2B,
	LSM6DSO_REG_OUTZ_L_A = 0x2C,
	LSM6DSO_REG_OUTZ_H_A = 0x2D,
	LSM6DSO_REG_EMB_FUNC_STATUS_MAINPAGE = 0x35,
	LSM6DSO_REG_FSM_STATUS_A_MAINPAGE = 0x36,
	LSM6DSO_REG_FSM_STATUS_B_MAINPAGE = 0x37,
	LSM6DSO_REG_STATUS_MASTER_MAINPAGE = 0x39,
	LSM6DSO_REG_FIFO_STATUS1 = 0x3A,
	LSM6DSO_REG_FIFO_STATUS2 = 0x3B,
	LSM6DSO_REG_TIMESTAMP0 = 0x40,
	LSM6DSO_REG_TIMESTAMP1 = 0x41,
	LSM6DSO_REG_TIMESTAMP2 = 0x42,
	LSM6DSO_REG_TIMESTAMP3 = 0x43,
	LSM6DSO_REG_TAP_CFG0 = 0x56,
	LSM6DSO_REG_TAP_CFG1 = 0x57,
	LSM6DSO_REG_TAP_CFG2 = 0x58,
	LSM6DSO_REG_TAP_THS_6D = 0x59,
	LSM6DSO_REG_INT_DUR2 = 0x5A,
	LSM6DSO_REG_WAKE_UP_THS = 0x5B,
	LSM6DSO_REG_WAKE_UP_DUR = 0x5C,
	LSM6DSO_REG_FREE_FALL = 0x5D,
	LSM6DSO_REG_MD1_CFG = 0x5E,
	LSM6DSO_REG_MD2_CFG = 0x5F,
	LSM6DSO_REG_I3C_BUS_AVB = 0x62,
	LSM6DSO_REG_INTERNAL_FREQ_FINE = 0x63,
	LSM6DSO_REG_INT_OIS = 0x6F,
	LSM6DSO_REG_CTRL1_OIS = 0x70,
	LSM6DSO_REG_CTRL2_OIS = 0x71,
	LSM6DSO_REG_CTRL3_OIS = 0x72,
	LSM6DSO_REG_X_OFS_USR = 0x73,
	LSM6DSO_REG_Y_OFS_USR = 0x74,
	LSM6DSO_REG_Z_OFS_USR = 0x75,
	LSM6DSO_REG_FIFO_DATA_OUT_TAG = 0x78,
	LSM6DSO_REG_FIFO_DATA_OUT_X_L = 0x79,
	LSM6DSO_REG_FIFO_DATA_OUT_X_H = 0x7A,
	LSM6DSO_REG_FIFO_DATA_OUT_Y_L = 0x7B,
	LSM6DSO_REG_FIFO_DATA_OUT_Y_H = 0x7C,
	LSM6DSO_REG_FIFO_DATA_OUT_Z_L = 0x7D,
	LSM6DSO_REG_FIFO_DATA_OUT_Z_H = 0x7E
} LSM6DSO_reg_e;

/// <summary>This enum contains a set of registers that are used to control the sensor hub feature of the LSM6DSO device.</summary>
typedef enum {
	LSM6DSO_SHUB_REG_SENSOR_HUB_1 = 0x02,
	LSM6DSO_SHUB_REG_SENSOR_HUB_2 = 0x03,
	LSM6DSO_SHUB_REG_SENSOR_HUB_3 = 0x04,
	LSM6DSO_SHUB_REG_SENSOR_HUB_4 = 0x05,
	LSM6DSO_SHUB_REG_SENSOR_HUB_5 = 0x06,
	LSM6DSO_SHUB_REG_SENSOR_HUB_6 = 0x07,
	LSM6DSO_SHUB_REG_SENSOR_HUB_7 = 0x08,
	LSM6DSO_SHUB_REG_SENSOR_HUB_8 = 0x09,
	LSM6DSO_SHUB_REG_SENSOR_HUB_9 = 0x0A,
	LSM6DSO_SHUB_REG_SENSOR_HUB_10 = 0x0B,
	LSM6DSO_SHUB_REG_SENSOR_HUB_11 = 0x0C,
	LSM6DSO_SHUB_REG_SENSOR_HUB_12 = 0x0D,
	LSM6DSO_SHUB_REG_SENSOR_HUB_13 = 0x0E,
	LSM6DSO_SHUB_REG_SENSOR_HUB_14 = 0x0F,
	LSM6DSO_SHUB_REG_SENSOR_HUB_15 = 0x10,
	LSM6DSO_SHUB_REG_SENSOR_HUB_16 = 0x11,
	LSM6DSO_SHUB_REG_SENSOR_HUB_17 = 0x12,
	LSM6DSO_SHUB_REG_SENSOR_HUB_18 = 0x13,
	LSM6DSO_SHUB_REG_MASTER_CONFIG = 0x14,
	LSM6DSO_SHUB_REG_SLV0_ADD = 0x15,
	LSM6DSO_SHUB_REG_SLV0_SUBADD = 0x16,
	LSM6DSO_SHUB_REG_SLAVE0_CONFIG = 0x17,
	LSM6DSO_SHUB_REG_SLV1_ADD = 0x18,
	LSM6DSO_SHUB_REG_SLV1_SUBADD = 0x19,
	LSM6DSO_SHUB_REG_SLAVE1_CONFIG = 0x1A,
	LSM6DSO_SHUB_REG_SLV2_ADD = 0x1B,
	LSM6DSO_SHUB_REG_SLV2_SUBADD = 0x1C,
	LSM6DSO_SHUB_REG_SLAVE2_CONFIG = 0x1D,
	LSM6DSO_SHUB_REG_SLV3_ADD = 0x1E,
	LSM6DSO_SHUB_REG_SLV3_SUBADD = 0x1F,
	LSM6DSO_SHUB_REG_SLAVE3_CONFIG = 0x20,
	LSM6DSO_SHUB_REG_DATAWRITE_SLV0 = 0x21,
	LSM6DSO_SHUB_REG_STATUS_MASTER = 0x22,
} LSM6DSO_shub_reg_e;

/// <summary>Bit field description for register STATUS_REG.</summary>
typedef union __attribute__((__packed__)) {
	struct __attribute__((__packed__)) {
		/// <summary>
		/// <para>Accelerometer new data available. Default value: false.</para>
		/// <para>false: no set of data available at accelerometer output</para>
		/// <para>true: a new set of data is available at accelerometer output.</para>
		/// </summary>
		bool     xlda : 1;

		/// <summary>
		/// <para>Gyroscope new data available. Default value: false.</para>
		/// <para>false: no set of data available at gyroscope output</para>
		/// <para>true: a new set of data is available at gyroscope output.</para>
		/// </summary>
		bool     gda : 1;

		/// <summary>
		/// <para>Temperature new data available. Default: false.</para>
		/// <para>false: no set of data is available at temperature sensor output</para>
		/// <para>true: a new set of data is available at temperature sensor output.</para>
		/// </summary>
		bool     tda : 1;

		unsigned res_7_3 : 5;
	};

	uint8_t mask;
} LSM6DSO_status_t;

/// <summary>Bit field description for register CTRL1_XL.</summary>
typedef union __attribute__((__packed__)) {
	struct __attribute__((__packed__)) {
		unsigned res_0 : 1;

		/// <summary>
		/// <para>Accelerometer high-resolution selection. Default value: 0.</para>
		/// <para>false: output from first stage digital filtering</para>
		/// <para>true: output from LPF2 second filtering.</para>
		/// </summary>
		bool lpf2_xl_en : 1;

		/// <summary>
		/// <para>Accelerometer full-scale selection. Default value: 00.</para>
		/// <para>Depends on the XL_FS_MODE in CTRL8_XL.</para>
		/// </summary>
		unsigned fs_xl : 2;

		/// <summary>
		/// <para>Output data rate and power mode selection. Default value: 0000.</para>
		/// <para>Depends on the XL_HM_MODE in CTRL6_C.</para>
		/// </summary>
		unsigned odr_xl : 4;
	};

	uint8_t mask;
} LSM6DSO_ctrl1_xl_t;

/// <summary>Bit field description for register CTRL2_G.</summary>
typedef union __attribute__((__packed__)) {
	struct __attribute__((__packed__)) {
		unsigned res_0 : 1;

		/// <summary>
		/// <para>Gyroscope full-scale at 125 dps. Default value: false</para>
		/// <para>(false: disabled; true: enabled)</para>
		/// </summary>
		bool     fs_125 : 1;

		/// <summary>
		/// <para>Gyroscope full-scale selection. Default value: 00</para>
		/// <para>(00: 250 dps; 01: 500 dps; 10: 1000 dps; 11: 2000 dps)</para>
		/// </summary>
		unsigned fs_g : 2;

		/// <summary>
		/// <para>Gyroscope output data rate selection. Default value: 0000</para>
		/// <para>Depends on the XL_HM_MODE in CTRL6_C.</para>
		/// </summary>
		unsigned odr_g : 4;
	};

	uint8_t mask;
} LSM6DSO_ctrl2_g_t;

/// <summary>This is  from the WHO_AM_I register. Its value is fixed at 6Ch.</summary>
static const uint8_t LSM6DSO_WHO_AM_I = 0x6C;

/// <summary>
/// <para>This is a subordinate device Address. Its value is fixed at 6Ah.</para>
/// <para>SA0 is tied to ground so the least significant bit of the address is zero.</para>
/// </summary>
static const uint32_t LSM6DSO_ADDRESS = 0x6A;

/// <summary>
/// <para>The application must call this function to implement a software reset.</para>
/// <para>This is a necessary function which will typically be used to reset the LSM6DSO device.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LSM6DSO_Reset(I2CMaster* driver);

/// <summary>
/// <para>The application must call this function to validate the device id.</para>
/// <para>This is a testing function which will typically be used in the beginning of the communication of the master with the subordinate device.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LSM6DSO_CheckWhoAmI(I2CMaster* driver);

/// <summary>
/// <para>The application must call this function to configure the linear acceleration sensor control register.</para>
/// <para>This is a function which will typically be used to configure the accelerometer of the subordinate device.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <param name="odr">Selects the output data rate and power mode.</param>
/// <param name="fs">Selects the full-scale of the accelerometer.</param>
/// <param name="lpf2_xl_en">Selects the LPF2 filter.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LSM6DSO_ConfigXL(I2CMaster* driver, unsigned odr, unsigned fs, bool lpf2_xl_en);

/// <summary>
/// <para>The application must call this function to configure the gyroscope sensor control register.</para>
/// <para>This is a function which will typically be used to configure the gyroscope of the subordinate device.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <param name="odr">Selects the output data rate and power mode.</param>
/// <param name="fs">Selects the full-scale of the gyroscope.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LSM6DSO_ConfigG(I2CMaster* driver, unsigned odr, unsigned fs);

/// <summary>
/// <para>The application must call this function to check if new data are available in the temperature, gyroscore and accelerometer sensors.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <param name="tda">Reads temperature sensor for new data. false: no new data is available; true: a set of new data is available.</param>
/// <param name="gda">Reads gyroscope for new data. false: no new data is available; true: a set of new data is available.</param>
/// <param name="xlda">Reads accelerometer for new data. false: no new data is available; true: a set of new data is available.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LSM6DSO_Status(I2CMaster* driver, bool* tda, bool* gda, bool* xlda);

/// <summary>
/// <para>The application must call this function to read the temperature sensor.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <param name="temp">Reads temperature sensor data.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LSM6DSO_ReadTemp(I2CMaster* driver, int16_t* temp);

/// <summary>
/// <para>The application must call this function to read the temperature sensor.</para>
/// <para>This function is a wrapper around <see cref="LSM6DSO_ReadTemp"/> which provides
/// human readable output.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <param name="temp">Reads temperature sensor data in Celsius degrees.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LSM6DSO_ReadTempCelsius(I2CMaster* driver, float_t* temp);

/// <summary>
/// <para>The application must call this function to read the gyroscope.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <param name="x">Reads angular rate for X axis.</param>
/// <param name="y">Reads angular rate for Y axis.</param>
/// <param name="z">Reads angular rate for Z axis.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LSM6DSO_ReadG(I2CMaster* driver, int16_t* x, int16_t* y, int16_t* z);

/// <summary>
/// <para>The application must call this function to read the gyroscope.</para>
/// <para>This function is a wrapper around <see cref="LSM6DSO_ReadG"/> which provides
/// human readable output.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <param name="x">Reads angular rate for X axis in mdps.</param>
/// <param name="y">Reads angular rate for Y axis in mdps.</param>
/// <param name="z">Reads angular rate for Z axis in mdps.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LSM6DSO_ReadGHuman(I2CMaster* driver, float_t* x, float_t* y, float_t* z);

/// <summary>
/// <para>The application must call this function to read the accelerometer.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <param name="x">Reads linear acceleration for x axis.</param>
/// <param name="y">Reads linear acceleration for Y axis.</param>
/// <param name="z">Reads linear acceleration for Z axis.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LSM6DSO_ReadXL(I2CMaster* driver, int16_t* x, int16_t* y, int16_t* z);

/// <summary>
/// <para>The application must call this function to read the accelerometer.</para>
/// <para>This function is a wrapper around <see cref="LSM6DSO_ReadXL"/> which provides
/// human readable output.</para>
/// </summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <param name="x">Reads linear acceleration for x axis in thousands of g.</param>
/// <param name="y">Reads linear acceleration for Y axis in thousands of g.</param>
/// <param name="z">Reads linear acceleration for Z axis in thousands of g.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LSM6DSO_ReadXLHuman(I2CMaster* driver, float_t* x, float_t* y, float_t* z);

/// <summary>Function for disabling MIPI I3CSM communication protocol.</summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LSM6DSO_DisableI3C(I2CMaster* driver);

/// <summary>Function for initialization of Sensor Hub feature.</summary>
/// <param name="driver">Selects the I2C driver to perform the transfer on.</param>
/// <returns>Returns true on success and false on failure.</returns>
bool LSM6DSO_InitSensorHub(I2CMaster* driver);

#endif // #ifndef LSM6DSO_H_