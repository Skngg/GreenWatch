#include "LSM6DSO.h"


bool LSM6DSO_RegWrite(I2CMaster* driver, uint8_t addr, uint8_t value)
{
    const uint8_t cmd[] = { addr, value };
    return (I2CMaster_WriteSync(driver, LSM6DSO_ADDRESS, cmd, sizeof(cmd)) == ERROR_NONE);
}

bool LSM6DSO_RegRead(I2CMaster* driver, uint8_t addr, uint8_t* value)
{
    uint8_t v;
    int32_t status = I2CMaster_WriteThenReadSync(
        driver, LSM6DSO_ADDRESS, &addr, sizeof(addr), &v, sizeof(v));
    if (value) {
        *value = v;
    }
    return (status == ERROR_NONE);
}

bool LSM6DSO_Reset(I2CMaster* driver)
{
    if (!driver) {
        return false;
    }

    if (!LSM6DSO_RegWrite(driver, LSM6DSO_REG_CTRL3_C, 0x01)) {
        return false;
    }

    uint8_t status;
    while (true) {
        if (LSM6DSO_RegRead(driver, LSM6DSO_REG_CTRL3_C, &status)
            && ((status & 0x01) == 0)) {
            break;
        }
    }

    return true;
}

bool LSM6DSO_CheckWhoAmI(I2CMaster* driver)
{
    if (!driver) {
        return false;
    }

    uint8_t ident;
    return (LSM6DSO_RegRead(driver, LSM6DSO_REG_WHO_AM_I, &ident)
        && (ident == LSM6DSO_WHO_AM_I));
}

bool LSM6DSO_ConfigXL(I2CMaster* driver, unsigned odr, unsigned fs, bool lpf2_xl_en)
{
    if (!driver) {
        return false;
    }

    LSM6DSO_ctrl1_xl_t ctrl1_xl;

    if ((odr >> 4) != 0) {
        return false;
    }
    ctrl1_xl.odr_xl = odr;

    switch (fs) {
    case  2:
        ctrl1_xl.fs_xl = 0;
        break;
    case 16:
        ctrl1_xl.fs_xl = 1;
        break;
    case  4:
        ctrl1_xl.fs_xl = 2;
        break;
    case  8:
        ctrl1_xl.fs_xl = 3;
        break;
    default:
        return false;
    }

    ctrl1_xl.lpf2_xl_en = lpf2_xl_en;

    return LSM6DSO_RegWrite(driver, LSM6DSO_REG_CTRL1_XL, ctrl1_xl.mask);
}


bool LSM6DSO_ConfigG(I2CMaster* driver, unsigned odr, unsigned fs)
{
    if (!driver) {
        return false;
    }

    LSM6DSO_ctrl2_g_t ctrl2_g;

    if ((odr >> 4) != 0) {
        return false;
    }
    ctrl2_g.odr_g = odr;

    switch (fs) {
    case  125:
        ctrl2_g.fs_125 = true;
        ctrl2_g.fs_g = 0;
        break;
    case  250:
        ctrl2_g.fs_125 = false;
        ctrl2_g.fs_g = 0;
        break;
    case  500:
        ctrl2_g.fs_125 = false;
        ctrl2_g.fs_g = 1;
        break;
    case 1000:
        ctrl2_g.fs_125 = false;
        ctrl2_g.fs_g = 2;
        break;
    case 2000:
        ctrl2_g.fs_125 = false;
        ctrl2_g.fs_g = 3;
        break;
    default:
        return false;
    }

    return LSM6DSO_RegWrite(driver, LSM6DSO_REG_CTRL2_G, ctrl2_g.mask);
}

bool LSM6DSO_Status(I2CMaster* driver, bool* tda, bool* gda, bool* xlda)
{
    if (!driver) {
        return false;
    }

    LSM6DSO_status_t status;
    if (!LSM6DSO_RegRead(driver, LSM6DSO_REG_STATUS_REG, &status.mask)) {
        return false;
    }

    if (tda) {
        *tda = status.tda;
    }
    if (gda) {
        *gda = status.gda;
    }
    if (xlda) {
        *xlda = status.xlda;
    }
    return true;
}

bool LSM6DSO_ReadTemp(I2CMaster* driver, int16_t* temp)
{
    if (!driver) {
        return false;
    }

    int16_t t = 0;
    if (!LSM6DSO_RegRead(driver, LSM6DSO_REG_OUT_TEMP_L, &((uint8_t*)&t)[0])
        || !LSM6DSO_RegRead(driver, LSM6DSO_REG_OUT_TEMP_H, &((uint8_t*)&t)[1])) {
        return false;
    }

    if (temp) {
        *temp = t;
    }
    return true;
}

bool LSM6DSO_ReadTempCelsius(I2CMaster* driver, float_t* t)
{
    int16_t th;
    if (!LSM6DSO_ReadTemp(driver, &th)) {
        return false;
    }

    if (t) *t = (((float_t)th / 256.0f) + 25.0f);
    return true;
}

bool LSM6DSO_ReadG(I2CMaster* driver, int16_t* x, int16_t* y, int16_t* z)
{
    if (!driver) {
        return false;
    }

    int16_t g_x = 0;
    int16_t g_y = 0;
    int16_t g_z = 0;

    if (!LSM6DSO_RegRead(driver, LSM6DSO_REG_OUTX_L_G, &((uint8_t*)&g_x)[0])
        || !LSM6DSO_RegRead(driver, LSM6DSO_REG_OUTX_H_G, &((uint8_t*)&g_x)[1])
        || !LSM6DSO_RegRead(driver, LSM6DSO_REG_OUTY_L_G, &((uint8_t*)&g_y)[0])
        || !LSM6DSO_RegRead(driver, LSM6DSO_REG_OUTY_H_G, &((uint8_t*)&g_y)[1])
        || !LSM6DSO_RegRead(driver, LSM6DSO_REG_OUTZ_L_G, &((uint8_t*)&g_z)[0])
        || !LSM6DSO_RegRead(driver, LSM6DSO_REG_OUTZ_H_G, &((uint8_t*)&g_z)[1])) {
        return false;
    }

    if (x) {
        *x = g_x;
    }
    if (y) {
        *y = g_y;
    }
    if (z) {
        *z = g_z;
    }
    return true;
}

bool LSM6DSO_ReadGHuman(I2CMaster* driver, float_t* x, float_t* y, float_t* z)
{
    LSM6DSO_ctrl2_g_t ctrl2_g;
    if (!LSM6DSO_RegRead(driver, LSM6DSO_REG_CTRL2_G, &ctrl2_g.mask)) {
        return false;
    }

    uint8_t fs_g = 0;
    float_t scale = 4.375f;     // 125 dps 
    if (ctrl2_g.fs_125) {
        switch (ctrl2_g.fs_g) {
        case 0:                 // 250 dps
            scale = 8.75f;
            break;
        case 1:                 // 500 dps
            scale = 17.5f;
            break;
        case 2:                 // 1000 dps
            scale = 35.0f;
            break;
        case 3:                 // 2000 dps
            scale = 70.0f;
            break;
        }
    }

    int16_t xh, yh, zh;
    if (!LSM6DSO_ReadXL(driver, &xh, &yh, &zh)) {
        return false;
    }

    if (x) *x = (float_t)xh * scale;
    if (y) *y = (float_t)yh * scale;
    if (z) *z = (float_t)zh * scale;
    return true;
}

bool LSM6DSO_ReadXL(I2CMaster* driver, int16_t* x, int16_t* y, int16_t* z)
{
    if (!driver) {
        return false;
    }

    int16_t xl_x = 0;
    int16_t xl_y = 0;
    int16_t xl_z = 0;

    if (!LSM6DSO_RegRead(driver, LSM6DSO_REG_OUTX_L_XL, &((uint8_t*)&xl_x)[0])
        || !LSM6DSO_RegRead(driver, LSM6DSO_REG_OUTX_H_XL, &((uint8_t*)&xl_x)[1])
        || !LSM6DSO_RegRead(driver, LSM6DSO_REG_OUTY_L_XL, &((uint8_t*)&xl_y)[0])
        || !LSM6DSO_RegRead(driver, LSM6DSO_REG_OUTY_H_XL, &((uint8_t*)&xl_y)[1])
        || !LSM6DSO_RegRead(driver, LSM6DSO_REG_OUTZ_L_XL, &((uint8_t*)&xl_z)[0])
        || !LSM6DSO_RegRead(driver, LSM6DSO_REG_OUTZ_H_XL, &((uint8_t*)&xl_z)[1])) {
        return false;
    }

    if (x) {
        *x = xl_x;
    }
    if (y) {
        *y = xl_y;
    }
    if (z) {
        *z = xl_z;
    }
    return true;
}

bool LSM6DSO_ReadXLHuman(I2CMaster* driver, float_t* x, float_t* y, float_t* z)
{
    LSM6DSO_ctrl1_xl_t ctrl1_xl;
    uint8_t ctrl8_xl;
    if (!LSM6DSO_RegRead(driver, LSM6DSO_REG_CTRL1_XL, &ctrl1_xl.mask)
        || !LSM6DSO_RegRead(driver, LSM6DSO_REG_CTRL8_XL, &ctrl8_xl)) {
        return false;
    }

    float_t scale = 0;
    switch (ctrl1_xl.fs_xl) {
    case 0:                     // 2 g
        scale = 0.061f;
        break;
    case 1:                     // 16 g / 2 g
        if ((ctrl8_xl & 0x01))  // XL_FS_MODE bit from CTRL8_XL reg    
            scale = 0.061f;
        else
            scale = 0.488f;
        break;
    case 2:                     // 4 g
        scale = 0.122f;
        break;
    case 3:                     // 8 g
        scale = 0.244f;
        break;
    }

    int16_t xh, yh, zh;
    if (!LSM6DSO_ReadXL(driver, &xh, &yh, &zh)) {
        return false;
    }

    if (x) *x = (float_t)xh * scale;
    if (y) *y = (float_t)yh * scale;
    if (z) *z = (float_t)zh * scale;
    return true;
}

bool LSM6DSO_DisableI3C(I2CMaster* driver) {
    if (!driver) {
        return false;
    }
    
    uint8_t ctrl9_xl;
    if (!LSM6DSO_RegRead(driver, LSM6DSO_REG_CTRL9_XL, &ctrl9_xl)) {
        return false;
    }

    return LSM6DSO_RegWrite(driver, LSM6DSO_REG_CTRL9_XL, (ctrl9_xl & 0xFD));
}

bool LSM6DSO_InitSensorHub(I2CMaster* driver) {
    if (!LSM6DSO_DisableI3C(driver)) {
        return false;
    }
     
    uint8_t func_cfg_access;
    if(!LSM6DSO_RegRead(driver, LSM6DSO_REG_FUNC_CFG_ACCESS, &func_cfg_access)) {
        return false;
    }

    // Set SHUB_REG_ACCESS bit to 1
    if(!LSM6DSO_RegWrite(driver, LSM6DSO_REG_FUNC_CFG_ACCESS, (func_cfg_access | 0x40))) {
        return false;
    }

    // Enable internal pull-up for LSM6DSO's Master I2C
    // MASTER_CONFIG register is 0 by default so for init we disregard its contents
    if (!LSM6DSO_RegWrite(driver, LSM6DSO_SHUB_REG_MASTER_CONFIG, 0x08)) {
        return false;
    }

    // Set SHUB_REG_ACCESS bit to 0
    if (!LSM6DSO_RegWrite(driver, LSM6DSO_REG_FUNC_CFG_ACCESS, (func_cfg_access & 0xBF))) {
        return false;
    }

    return true;
}