#include "drv8323.h"


void drv8323_Init(drv8323_TypeDef *drv, SPI_TypeDef *spi){
    drv->spi = spi;
}

uint16_t drv_transceive(drv8323_TypeDef *drv, uint16_t tx_reg){
    //tx_reg = 0b1000100000000000;

    SPI_Transfer(drv->spi, tx_reg);
    // printf("transmitting\t");  print_reg(tx_reg, 16);
    uint16_t rx_reg = SPI_Transcieve(drv->spi, 0b1000000000000000);

    return rx_reg;
}

uint16_t drv_write(drv8323_TypeDef *drv, uint32_t drv_reg, uint16_t payload){
    //tx_reg = 0b1000100000000000;
    payload &= 0b0000011111111111;
    uint16_t tx_reg = 0;
    tx_reg |= (drv_reg & 0b0111100000000000);
    tx_reg |= payload;
    SPI_Transfer(drv->spi, tx_reg);
    uint16_t rx_reg = SPI_Transcieve(drv->spi, 0b1000000000000000);

    return rx_reg;
}

uint16_t drv_read(drv8323_TypeDef *drv, uint32_t drv_reg){
    //tx_reg = 0b1000100000000000;
    uint16_t tx_reg = 0;
    tx_reg |= drv_reg;
    tx_reg |= (1<<15);
    // tx_reg |= payload;

    // printf("reading\t");  print_reg(tx_reg, 16);

    SPI_Transfer(drv->spi, tx_reg);
    uint16_t rx_reg = SPI_Transcieve(drv->spi, 0b1000000000000000);

    return rx_reg;
}


void drv_write_reg(drv8323_TypeDef *drv, uint16_t reg, uint16_t data){

    data &= 0x7FF; // cut to lower 10 bits
    uint16_t tx_reg = 0;
    tx_reg |= (reg & 0x7800);
    tx_reg |= data;
    // SPI_Transfer(drv->spi, tx_reg);
    // uint16_t rx_reg = SPI_Transcieve(drv->spi, 0b1000000000000000);

    return;
}

uint16_t drv_read_reg(drv8323_TypeDef *drv, uint16_t reg){
    uint16_t tx_reg = 0;
    tx_reg |= (reg & 0x7800);
    tx_reg |= (1 << 15);
    // SPI_Transfer(drv->spi, tx_reg);
    // uint16_t rx_reg = SPI_Transcieve(drv->spi, 0b1000000000000000);
    return 0;
}

void drv_update(drv8323_TypeDef *drv){

    // SPI_Transfer(drv->spi, 0b1000000000000000);
    drv->CSA_control    = SPI_Transcieve(drv->spi, DRV_CSA_CONTROL);
    drv->driver_control = SPI_Transcieve(drv->spi, DRV_DRIVER_CONTROL);
    drv->fault_status_1 = SPI_Transcieve(drv->spi, DRV_FAULT_STATUS_1);
    drv->gate_drive_HS  = SPI_Transcieve(drv->spi, DRV_GATE_DRIVE_HS);
    drv->gate_drive_LS  = SPI_Transcieve(drv->spi, DRV_GATE_DRIVE_LS);
    drv->OCP_control    = SPI_Transcieve(drv->spi, DRV_OCP_CONTROL);
    drv->VGS_status_2   = SPI_Transcieve(drv->spi, DRV_VGS_STATUS_2);

    
}