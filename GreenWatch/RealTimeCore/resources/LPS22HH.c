#include "LPS22HH.h"


bool LPS22HH_RegWrite(I2CMaster* driver, uint8_t addr, uint8_t value)
{
    const uint8_t cmd[] = { addr, value };
    return (I2CMaster_WriteSync(driver, LPS22HH_ADDRESS, cmd, sizeof(cmd)) == ERROR_NONE);
}

bool LPS22HH_RegRead(I2CMaster* driver, uint8_t addr, uint8_t* value)
{
    uint8_t v;
    int32_t status = I2CMaster_WriteThenReadSync(
        driver, LPS22HH_ADDRESS, &addr, sizeof(addr), &v, sizeof(v));
    if (value) {
        *value = v;
    }
    return (status == ERROR_NONE);
}

bool LPS22HH_Reset(I2CMaster* driver)
{
    if (!driver) {
        return false;
    }

    if (!LPS22HH_RegWrite(driver, LPS22HH_REG_CTRL_REG2, 0x04)) {
        return false;
    }

    uint8_t status;
    while (true) {
        if (LPS22HH_RegRead(driver, LPS22HH_REG_CTRL_REG2, &status)
            && ((status & 0x04) == 0)) {
            break;
        }
    }

    return true;
}

bool LPS22HH_CheckWhoAmI(I2CMaster* driver)
{
    if (!driver) {
        return false;
    }

    uint8_t ident;
    return (LPS22HH_RegRead(driver, LPS22HH_REG_WHO_AM_I, &ident)
        && (ident == LPS22HH_WHO_AM_I));
}

bool LPS22HH_Config(I2CMaster* driver, unsigned odr, bool en_lpfp, bool lpfp_cfg, bool bdu, bool sim)
{
    if (!driver) {
        return false;
    }

    LPS22HH_ctrl_reg1_t ctrl_reg1;

    if ((odr >> 3) != 0) {
        return false;
    }
    ctrl_reg1.odr = odr;

    ctrl_reg1.en_lpfp = en_lpfp;
    ctrl_reg1.lpfp_cfg = lpfp_cfg;
    ctrl_reg1.bdu = bdu;
    ctrl_reg1.sim = sim;

    return LPS22HH_RegWrite(driver, LPS22HH_REG_CTRL_REG1, ctrl_reg1.mask);
}


bool LPS22HH_Status(I2CMaster* driver, bool* t_or, bool* p_or, bool* t_da, bool* p_da)
{
    if (!driver) {
        return false;
    }

    LPS22HH_status_t status;
    if (!LPS22HH_RegRead(driver, LPS22HH_REG_STATUS, &status.mask)) {
        return false;
    }

    if (t_or) {
        *t_or = status.t_or;
    }
    if (p_or) {
        *p_or = status.p_or;
    }
    if (t_da) {
        *t_da = status.t_da;
    }
    if (p_da) {
        *p_da = status.p_da;
    }
    
    return true;
}

bool LPS22HH_ReadTemp(I2CMaster* driver, int16_t* temp)
{
    if (!driver) {
        return false;
    }

    int16_t t = 0;
    if (!LPS22HH_RegRead(driver, LPS22HH_REG_TEMP_OUT_L, &((uint8_t*)&t)[0])
        || !LPS22HH_RegRead(driver, LPS22HH_REG_TEMP_OUT_H, &((uint8_t*)&t)[1])) {
        return false;
    }

    if (temp) {
        *temp = t;
    }
    return true;
}

bool LPS22HH_ReadTempCelsius(I2CMaster* driver, float_t* temp)
{
    int16_t th;
    if (!LPS22HH_ReadTemp(driver, &th)) {
        return false;
    }

    // Divide by sensitivity of 100 LSB/C
    if (temp) *temp = (float_t)th / 100.0f;
    return true;
}

bool LPS22HH_ReadPressure(I2CMaster* driver, int32_t* pressure)
{
    if (!driver) {
        return false;
    }

    int32_t p = 0;
    if (!LPS22HH_RegRead(driver, LPS22HH_REG_PRESS_OUT_XL, &((uint8_t*)&p)[0])
        || !LPS22HH_RegRead(driver, LPS22HH_REG_PRESS_OUT_L, &((uint8_t*)&p)[1])
        || !LPS22HH_RegRead(driver, LPS22HH_REG_PRESS_OUT_H, &((uint8_t*)&p)[2])) {
        return false;
    }

    if (pressure) {
        *pressure = p;
    }
    return true;
}

bool LPS22HH_ReadPressureHuman(I2CMaster* driver, float_t* pressure)
{
    int32_t ps;
    if (!LPS22HH_ReadPressure(driver, &ps)) {
        return false;
    }
    
    // Divide by sensitivity of 4096 LSB/hPa
    if (pressure) *pressure = (float_t)ps / 4096.0f;
    return true;

}
