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
    uint16_t fs2 = drv->fault_status_2;

    // don't need to worry about performance of an if/else ladder
    // as if an error has occured, the motor shouldn't be running
    drv_fault_TypeDef return_fault = (fs1 | fs2);
    if(fs1 & FS1_VDS_OCP){
        printf("VDS Over Current Protection Error!\n");
    }
    if(fs1 & FS1_GDF){
        printf("Gate Drive Fault error\n");
    }
    if(fs1 & FS1_UVLO){
        printf("UVLO error\n");
    }
    if(fs1 & FS1_OTSD){
        printf("Overtemp shutdown error\n");
    }
    if(fs1 & FS1_VDS_HA){
        printf("VDS HA error\n");
    }
    if(fs1 & FS1_VDS_LA){
        printf("VDS LA error\n");
    }
    if(fs1 & FS1_VDS_HB){
        printf("VDS HB error\n");
    }
    if(fs1 & FS1_VDS_LB){
        printf("VDS LB error\n");
    }
    if(fs1 & FS1_VDS_HC){
        printf("VDS HC error\n");
    }
    if(fs1 & FS1_VDS_LC){
        printf("VDS LC error\n");
    }
    if(fs2 & FS2_SA_OC){
        printf("overcurrent on phase A\n");
    }
    if(fs2 & FS2_SB_OC){
        printf("overcurrent on phase B\n");
    }
    if(fs2 & FS2_SC_OC){
        printf("overcurrent on phase C\n");
    }
    if(fs2 & FS2_OTW){
        printf("overtemp warning\n");
    }
    if(fs2 & FS2_CPUV){
        printf("Charge pump undervoltage fault condition\n");
    }
    if(fs2 & FS2_VGS_HA){
        printf("VGS HA error\n");
    }
    if(fs2 & FS2_VGS_LA){
        printf("VGS LA error\n");
    }
    if(fs2 & FS2_VGS_HB){
        printf("VGS HB error\n");
    }
    if(fs2 & FS2_VGS_LB){
        printf("VGS LB error\n");
    }
    if(fs2 & FS2_VGS_HC){
        printf("VGS HC error\n");
    }
    if(fs2 & FS2_VGS_LC){
        printf("VDS LC error\n");
    }

    return return_fault; // implement properly!
}

void     drv8323_3pwm_mode(drv8323_TypeDef *drv){
    /*
        sets register to be 3pwm mode
    */
}