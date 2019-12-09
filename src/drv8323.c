#include "drv8323.h"


void drv8323_Init(drv8323_TypeDef *drv, SPI_TypeDef *spi){
    drv->spi = spi;
}

void drv8323_enable_all_fault_reporting(drv8323_TypeDef *drv){
    drv8323_update(drv);

    // enable all faults to assert on nFAULT pin
    drv->driver_control &= ~(DRV_CTRL_DIS_CPUV);    // enables charge pump undervoltage lockout
    drv->driver_control &= ~(DRV_CTRL_DIS_GDF);     // enables gate drive fault reporting
    drv->driver_control |=  (DRV_CTRL_OTW_REP);     // enables thermal warning reporting
    drv->OCP_control    &= ~(OCP_CTRL_OCP_MODE);    // over-current causes latched fault
    drv8323_write_reg(drv, DRV_DRIVER_CONTROL, drv->driver_control);
    drv8323_write_reg(drv, DRV_OCP_CONTROL, drv->OCP_control);
}

void drv8323_write_reg(drv8323_TypeDef *drv, uint16_t reg, uint16_t data){

    data &= 0x7FF; // cut to lower 10 bits
    uint16_t tx_reg = 0;
    tx_reg |= (reg & 0x7800);
    tx_reg |= data;
    SPI_Transcieve(drv->spi, tx_reg);
    // uint16_t rx_reg = SPI_Transcieve(drv->spi, 0b1000000000000000);

    return;
}

uint16_t drv8323_read_reg(drv8323_TypeDef *drv, uint16_t reg){
    uint16_t tx_reg = 0;
    tx_reg |= (reg & 0x7800);
    tx_reg |= (1 << 15);
    return SPI_Transcieve(drv->spi, tx_reg);
    // uint16_t rx_reg = SPI_Transcieve(drv->spi, 0b1000000000000000);
}

drv_fault_TypeDef drv8323_update(drv8323_TypeDef *drv){

    // SPI_Transfer(drv->spi, 0b1000000000000000);
    drv->fault_status_1 = drv8323_read_reg(drv, DRV_FAULT_STATUS_1);
    drv->fault_status_2 = drv8323_read_reg(drv, DRV_FAULT_STATUS_2);
    drv->driver_control = drv8323_read_reg(drv, DRV_DRIVER_CONTROL);
    drv->gate_drive_HS  = drv8323_read_reg(drv, DRV_GATE_DRIVE_HS);
    drv->gate_drive_LS  = drv8323_read_reg(drv, DRV_GATE_DRIVE_LS);
    drv->OCP_control    = drv8323_read_reg(drv, DRV_OCP_CONTROL);
    drv->CSA_control    = drv8323_read_reg(drv, DRV_CSA_CONTROL);

    return drv8323_check_faults(drv);
}

drv_fault_TypeDef drv8323_check_faults(drv8323_TypeDef *drv){
    uint16_t fs1 = drv->fault_status_1;
    uint16_t vgs2 = drv->fault_status_2;

    // don't need to worry about performance of an if/else ladder
    // as if an error has occured, the motor shouldn't be running
    return (fs1 || vgs2); // implement properly!
}

void     drv8323_3pwm_mode(drv8323_TypeDef *drv){
    /*
        sets register to be 3pwm mode
    */
}