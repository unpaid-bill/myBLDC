#ifndef __DRV8323_H__
#define __DRV8323_H__

#include "stm32f103xb.h"
#include "spi.h"
#include "main.h"
#include "uart.h"
#include "gpio.h"

typedef struct
{
    uint16_t fault_status_1;
    uint16_t VGS_status_2;
    uint16_t driver_control;
    uint16_t gate_drive_HS;
    uint16_t gate_drive_ls;
    uint16_t OCP_control;
    uint16_t CSA_control;
    SPI_TypeDef *spi;
} drv8323_TypeDef;


/*******      Register Address Definitions       *******/
#define DRV_STATUS_1                0x0U << 11
#define DRV_STATUS_2                0x1U << 11
#define DRV_CTRL_1                  0x2U << 11
#define DRV_CTRL_2                  0x3U << 11

#define DRV_FAULT_STATUS_1          0x0U << 11
#define DRV_VGS_STATUS_2            0x1U << 11
#define DRV_DRIVER_CONTROL          0x2U << 11
#define DRV_GATE_DRIVE_HS           0x3U << 11
#define DRV_GATE_DRIVE_LS           0x4U << 11
#define DRV_OCP_CONTROL             0x5U << 11
#define DRV_CSA_CONTROL             0x6U << 11


/*******  Bit definitions for Fault Status 1 register  *******/
#define FS1_FAULT_Pos                  (10U)
#define FS1_FAULT_Msk                  (0x1U << FS1_FAULT_Pos)
#define FS1_FAULT                      FS1_FAULT_Msk
#define FS1_VDS_OCP_Pos                (9U)
#define FS1_VDS_OCP_Msk                (0x1U << FS1_VDS_OCP_Pos)
#define FS1_VDS_OCP                    FS1_VDS_OCP_Msk
#define FS1_GDF_Pos                    (8U)
#define FS1_GDF_Msk                    (0x1U << FS1_GDF_Pos)
#define FS1_GDF                        FS1_GDF_Msk
#define FS1_UVLO_Pos                   (7U)
#define FS1_UVLO_Msk                   (0x1U << FS1_UVLO_Pos)
#define FS1_UVLO                       FS1_UVLO_Msk
#define FS1_OTSD_Pos                   (6U)
#define FS1_OTSD_Msk                   (0x1U << FS1_OTSD_Pos)
#define FS1_OTSD                       FS1_OTSD_Msk
#define FS1_VDS_HA_Pos                 (5U)
#define FS1_VDS_HA_Msk                 (0x1U << FS1_VDS_HA_Pos)
#define FS1_VDS_HA                     FS1_VDS_HA_Msk
#define FS1_VDS_LA_Pos                 (4U)
#define FS1_VDS_LA_Msk                 (0x1U << FS1_VDA_LA_Pos)
#define FS1_VDS_LA                     FS1_VDA_LA_Msk
#define FS1_VDS_HB_Pos                 (3U)
#define FS1_VDS_HB_Msk                 (0x1U << FS1_VDS_HB_Pos)
#define FS1_VDS_HB                     FS1_VDS_HB_Msk
#define FS1_VDS_LB_Pos                 (2U)
#define FS1_VDS_LB_Msk                 (0x1U << FS1_VDS_LB_Pos)
#define FS1_VDS_LB                     FS1_VDS_LB_Msk
#define FS1_VDS_HC_Pos                 (1U)
#define FS1_VDS_HC_Msk                 (0x1U << FS1_VDS_HC_Pos)
#define FS1_VDS_HC                     FS1_VDS_HC_Msk
#define FS1_VDS_LC_Pos                 (0U)
#define FS1_VDS_LC_Msk                 (0x1U << FS1_VDS_LC_Pos)
#define FS1_VDS_LC                     FS1_VDS_LC_Msk

/*******  Bit definitions for VGS Status 2 register  *********/

/*******  Bit definitions for Driver Control register  *******/
#define DRV_CTRL_DIS_CPUV_Pos          (9U)
#define DRV_CTRL_DIS_CPUV_Msk          (0x1U << DRV_CTRL_DIS_CPUV_Pos)
#define DRV_CTRL_DIS_CPUV              DRV_CTRL_DIS_CPUV_Msk
#define DRV_CTRL_DIS_GDF_Pos           (8U)
#define DRV_CTRL_DIS_GDF_Msk           (0x1U << DRV_CTRL_DIS_GDF_Pos)
#define DRV_CTRL_DIS_GDF               DRV_CTRL_DIS_GDF_Msk
#define DRV_CTRL_OTW_REP_Pos           (7U)
#define DRV_CTRL_OTW_REP_Msk           (0x1U << DRV_CTRL_OTW_REP_Pos)
#define DRV_CTRL_OTW_REP               DRV_CTRL_OTW_REP_Msk

#define DRV_CTRL_PWM_MODE_Pos          (5U)
#define DRV_CTRL_PWM_MODE_Msk          (0x3U << DRV_CTRL_PWM_MODE_Pos)
#define DRV_CTRL_PWM_MODE              DRV_CTRL_PWM_MODE_Msk
#define DRV_CTRL_PWM_MODE_0            (0x1U << DRV_CTRL_PWM_MODE_Pos)
#define DRV_CTRL_PWM_MODE_1            (0x2U << DRV_CTRL_PWM_MODE_Pos)
#define DRV_CTRL_PWM_MODE_6x           (0x0U << DRV_CTRL_PWM_MODE_Pos)
#define DRV_CTRL_PWM_MODE_3x           (0x1U << DRV_CTRL_PWM_MODE_Pos)
#define DRV_CTRL_PWM_MODE_1x           (0x2U << DRV_CTRL_PWM_MODE_Pos)
#define DRV_CTRL_PWM_MODE_IND          (0x3U << DRV_CTRL_PWM_MODE_Pos)

#define DRV_CTRL_1PWM_COM_Pos          (4U)
#define DRV_CTRL_1PWM_COM_Msk          (0x1U << DRV_CTRL_1PWM_COM_Pos)
#define DRV_CTRL_1PWM_COM              DRV_CTRL_1PWM_COM_Msk
#define DRV_CTRL_1PWM_DIR_Pos          (3U)
#define DRV_CTRL_1PWM_DIR_Msk          (0x1U <<DRV_CTRL_1PWM_DIR_Pos)
#define DRV_CTRL_1PWM_DIR              DRV_CTRL_1PWM_DIR_Msk
#define DRV_CTRL_COAST_Pos             (2U)
#define DRV_CTRL_COAST_Msk             (0x1U << DRV_CTRL_COAST_Pos)
#define DRV_CTRL_COAST                 DRV_CTRL_COAST_Msk
#define DRV_CTRL_BRAKE_Pos             (1U)
#define DRV_CTRL_BRAKE_Msk             (0x1U << DRV_CTRL_BRAKE_Pos)
#define DRV_CTRL_BRAKE                 DRV_CTRL_BRAKE_Msk
#define DRV_CTRL_CLR_FLT_Pos           (0U)
#define DRV_CTRL_CLR_FLT_Msk           (0x1U << DRV_CTRL_CLR_FLT_Pos)
#define DRV_CTRL_CLR_FLT               DRV_CTRL_CLR_FLT_Msk

/*******  Bit definitions for Gate Drive HS register  ********/
#define GATE_DRV_HS_LOCK_Pos           (8U)
#define GATE_DRV_HS_LOCK_Msk           (0x7U << GATE_DRV_HS_LOCK_Pos)
#define GATE_DRV_HS_LOCK               GATE_DRV_HS_LOCK_Msk
#define GATE_DRV_HS_LOCK_0             (0x1U << GATE_DRV_HS_LOCK_Pos)
#define GATE_DRV_HS_LOCK_1             (0x2U << GATE_DRV_HS_LOCK_Pos)
#define GATE_DRV_HS_LOCK_2             (0x4U << GATE_DRV_HS_LOCK_Pos)

#define GATE_DRV_HS_IDRIVEP_HS_Pos     (4U)
#define GATE_DRV_HS_IDRIVEP_HS_Msk     (0xFU << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS         GATE_DRV_HS_IDRIVEP_HS_Msk
#define GATE_DRV_HS_IDRIVEP_HS_0       (0x1U << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_1       (0x2U << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_2       (0x4U << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_3       (0x8U << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_10mA    (0x0U << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_30mA    (0x1U << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_60mA    (0x2U << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_80mA    (0x3U << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_120mA   (0x4U << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_140mA   (0x5U << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_170mA   (0x6U << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_190mA   (0x7U << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_260mA   (0x8U << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_330mA   (0x9U << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_370mA   (0xAU << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_440mA   (0xBU << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_570mA   (0xCU << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_680mA   (0xDU << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_820mA   (0xEU << GATE_DRV_HS_IDRIVEP_HS_Pos)
#define GATE_DRV_HS_IDRIVEP_HS_1000mA  (0xFU << GATE_DRV_HS_IDRIVEP_HS_Pos)

#define GATE_DRV_HS_IDRIVEN_HS_Pos     (0U)
#define GATE_DRV_HS_IDRIVEN_HS_Msk     (0xFU << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS         GATE_DRV_HS_IDRIVEN_HS_Msk
#define GATE_DRV_HS_IDRIVEN_HS_0       (0x1U << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_1       (0x2U << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_2       (0x4U << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_3       (0x8U << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_20mA    (0x0U << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_60mA    (0x1U << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_120mA   (0x2U << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_160mA   (0x3U << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_240mA   (0x4U << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_280mA   (0x5U << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_340mA   (0x6U << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_380mA   (0x7U << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_520mA   (0x8U << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_660mA   (0x9U << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_740mA   (0xAU << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_880mA   (0xBU << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_1140mA  (0xCU << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_1360mA  (0xDU << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_1640mA  (0xEU << GATE_DRV_HS_IDRIVEN_HS_Pos)
#define GATE_DRV_HS_IDRIVEN_HS_2000mA  (0xFU << GATE_DRV_HS_IDRIVEN_HS_Pos)

/*******  Bit definitions for Gate Drive LS register  ********/
#define GATE_DRV_LS_CBC_Pos            (10U)
#define GATE_DRV_LS_CBC_Msk            (0x7U << GATE_DRV_LS_LOCK_Pos)
#define GATE_DRV_LS_CBC                GATE_DRV_LS_LOCK_Msk
#define GATE_DRV_LS_TDRIVE_Pos         (8U)
#define GATE_DRV_LS_TDRIVE_Msk         (0x7U << GATE_DRV_LS_LOCK_Pos)
#define GATE_DRV_LS_TDRIVE             GATE_DRV_LS_LOCK_Msk
#define GATE_DRV_LS_TDRIVE_0           (0x1U << GATE_DRV_LS_TDRIVE_Pos)
#define GATE_DRV_LS_TDRIVE_1           (0x2U << GATE_DRV_LS_TDRIVE_Pos)
#define GATE_DRV_LS_TDRIVE_500ns       (0x0U << GATE_DRV_LS_TDRIVE_Pos)
#define GATE_DRV_LS_TDRIVE_1000ns      (0x1U << GATE_DRV_LS_TDRIVE_Pos)
#define GATE_DRV_LS_TDRIVE_2000ns      (0x2U << GATE_DRV_LS_TDRIVE_Pos)
#define GATE_DRV_LS_TDRIVE_4000ns      (0x3U << GATE_DRV_LS_TDRIVE_Pos)

#define GATE_DRV_LS_IDRIVEP_LS_Pos     (4U)
#define GATE_DRV_LS_IDRIVEP_LS_Msk     (0xF << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS         GATE_DRV_LS_IDRIVEP_LS_Msk
#define GATE_DRV_LS_IDRIVEP_LS_0       (0x1U << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_1       (0x2U << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_2       (0x4U << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_3       (0x8U << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_10mA    (0x0U << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_30mA    (0x1U << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_60mA    (0x2U << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_80mA    (0x3U << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_120mA   (0x4U << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_140mA   (0x5U << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_170mA   (0x6U << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_190mA   (0x7U << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_260mA   (0x8U << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_330mA   (0x9U << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_370mA   (0xAU << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_440mA   (0xBU << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_570mA   (0xCU << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_680mA   (0xDU << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_820mA   (0xEU << GATE_DRV_LS_IDRIVEP_LS_Pos)
#define GATE_DRV_LS_IDRIVEP_LS_1000mA  (0xFU << GATE_DRV_LS_IDRIVEP_LS_Pos)

#define GATE_DRV_LS_IDRIVEN_LS_Pos     (0U)
#define GATE_DRV_LS_IDRIVEN_LS_Msk     (0xFU << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS         GATE_DRV_LS_IDRIVEN_LS_Msk
#define GATE_DRV_LS_IDRIVEN_LS_0       (0x1U << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_1       (0x2U << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_2       (0x4U << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_3       (0x8U << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_20mA    (0x0U << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_60mA    (0x1U << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_120mA   (0x2U << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_160mA   (0x3U << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_240mA   (0x4U << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_280mA   (0x5U << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_340mA   (0x6U << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_380mA   (0x7U << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_520mA   (0x8U << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_660mA   (0x9U << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_740mA   (0xAU << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_880mA   (0xBU << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_1140mA  (0xCU << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_1360mA  (0xDU << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_1640mA  (0xEU << GATE_DRV_LS_IDRIVEN_LS_Pos)
#define GATE_DRV_LS_IDRIVEN_LS_2000mA  (0xFU << GATE_DRV_LS_IDRIVEN_LS_Pos)

/*******  Bit definitions for OCP Control register  **********/

#define OCP_CTRL_TRETRY_Pos         (10U)
#define OCP_CTRL_TRETRY_Msk         (0x1U << OCP_CTRL_TRETRY_Pos)
#define OCP_CTRL_TRETRY             OCP_CTRL_TRETRY_Msk

#define OCP_CTRL_DEAD_TIME_Pos      (8U)
#define OCP_CTRL_DEAD_TIME_Msk      (0x3U << OCP_CTRL_DEAD_TIME_Pos)
#define OCP_CTRL_DEAD_TIME          OCP_CTRL_DEAD_TIME_Msk
#define OCP_CTRL_DEAD_TIME_0        (0x1U << OCP_CTRL_DEAD_TIME_Pos)
#define OCP_CTRL_DEAD_TIME_1        (0x2U << OCP_CTRL_DEAD_TIME_Pos)
#define OCP_CTRL_DEAD_TIME_50ns     (0x0U << OCP_CTRL_DEAD_TIME_Pos)
#define OCP_CTRL_DEAD_TIME_100ns    (0x1U << OCP_CTRL_DEAD_TIME_Pos)
#define OCP_CTRL_DEAD_TIME_200ns    (0x2U << OCP_CTRL_DEAD_TIME_Pos)
#define OCP_CTRL_DEAD_TIME_400ns    (0x3U << OCP_CTRL_DEAD_TIME_Pos)
/*******  Bit definitions for CSA Control register  **********/


/*******  Bit definitions for Status Register 1  *******/
#define SR1_FETLC_OC_Pos            (0U)
#define SR1_FETLC_OC_Msk            (0x1U << SR1_FETLC_OC_Pos)
#define SR1_FETLC_OC                SR1_FETLC_OC_Msk
#define SR1_FETHC_OC_Pos            (1U)
#define SR1_FETHC_OC_Msk            (0x1U << SR1_FETHC_OC_Pos)
#define SR1_FETHC_OC                SR1_FETHC_OC_Msk
#define SR1_FETLB_OC_Pos            (2U)
#define SR1_FETLB_OC_Msk            (0x1U << SR1_FETLB_OC_Pos)
#define SR1_FETLB_OC                SR1_FETLB_OC_Msk
#define SR1_FETHB_OC_Pos            (3U)
#define SR1_FETHB_OC_Msk            (0x1U << SR1_FETHB_OC_Pos)
#define SR1_FETHB_OC                SR1_FETHB_OC_Msk
#define SR1_FETLA_OC_Pos            (4U)
#define SR1_FETLA_OC_Msk            (0x1U << SR1_FETLA_OC_Pos)
#define SR1_FETLA_OC                SR1_FETLA_OC_Msk
#define SR1_FETHA_OC_Pos            (5U)
#define SR1_FETHA_OC_Msk            (0x1U << SR1_FETHA_OC_Pos)
#define SR1_FETHA_OC                SR1_FETHA_OC_Msk

#define SR1_OTW_Pos                 (6U)
#define SR1_OTW_Msk                 (0x1U << SR1_OTW_Pos)
#define SR1_OTW                     SR1_OTW_Msk
#define SR1_OTSD_Pos                (7U)
#define SR1_OTSD_Msk                (0x1U << SR1_OTSD_Pos)
#define SR1_OTSD                    SR1_OTSD_Msk
#define SR1_PVDD_Pos                (8U)
#define SR1_PVDD_Msk                (0x1U << SR1_PVDD_Pos)
#define SR1_PVDD                    SR1_PVDD_Msk
#define SR1_GVDD_Pos                (9U)
#define SR1_GVDD_Msk                (0x1U << SR1_GVDD_Pos)
#define SR1_GVDD                    SR1_GVDD_Msk
#define SR1_FAULT_Pos               (10U)
#define SR1_FAULT_Msk               (0x1U << SR1_FAULT_Pos)
#define SR1_FAULT                   SR1_FAULT_Msk

/*******  Bit definitions for Status Register 2  *******/
#define SR2_DEVICE_ID_Pos           (0U)
#define SR2_DEVICE_ID_Msk           (0xFU << SR2_DEVICE_ID_Pos)
#define SR2_DEVICE_ID               SR2_DEVICE_ID_Msk

#define SR2_GVDD_OV_Pos             (7U)
#define SR2_GVDD_OV_Msk             (0x1U << SR2_GVDD_OV_Pos)
#define SR2_GVDD_OV                 SR2_GVDD_OV_Msk

/*******  Bit definitions for Control Register 1  *******/
/* Gate current configuration */
#define CR1_GATE_CURRENT_Pos        (0U)
#define CR1_GATE_CURRENT_Msk        (0x3U << CR1_GATE_CURRENT_Pos)
#define CR1_GATE_CURRENT            CR1_GATE_CURRENT_Msk
#define CR1_GATE_CURRENT_0          (0x1U << CR1_GATE_CURRENT_Pos)
#define CR1_GATE_CURRENT_1          (0x2U << CR1_GATE_CURRENT_Pos)

#define CR1_GATE_CURRENT_1_7A       0x000U
#define CR1_GATE_CURRENT_0_7A       0x001U
#define CR1_GATE_CURRENT_0_25A      0x002U

#define CR1_GATE_RESET_Pos          (2U)
#define CR1_GATE_RESET_Msk          (0x1U << CR1_GATE_RESET_Pos)
#define CR1_GATE_RESET              CR1_GATE_RESET_Msk

/* PWM mode configuration */
#define CR1_PWM_MODE_Pos            (3U)
#define CR1_PWM_MODE_Msk            (0x1U << CR1_PWM_MODE_Pos)
#define CR1_PWM_MODE                CR1_PWM_MODE_Msk

#define CR1_PWM_MODE_6              0x000U
#define CR1_PWM_MODE_3              CR1_PWM_MODE

/* Over-current protection configuration */
#define CR1_OCP_MODE_Pos            (4U)
#define CR1_OCP_MODE_Msk            (0x3U << CR1_OCP_MODE_Pos)
#define CR1_OCP_MODE                CR1_OCP_MODE_Msk
#define CR1_OCP_MODE_0              (0x1U << CR1_OCP_MODE_Pos)
#define CR1_OCP_MODE_1              (0x2U << CR1_OCP_MODE_Pos)

#define CR1_OCP_MODE_LIMIT          0x000U
#define CR1_OCP_MODE_LATCH          0x010U
#define CR1_OCP_MODE_REPORT         0x020U
#define CR1_OCP_MODE_DISABLED       0x030U

/* Over-current adjust configuration */
#define CR1_OC_ADJ_SET_Pos          (6U)
#define CR1_OC_ADJ_SET_Msk          (0x1F << CR1_OC_ADJ_SET_Pos)
#define CR1_OC_ADJ_SET              CR1_OC_ADJ_SET_Msk
// #define CR1_OC_ADJ_SET_0            (0x1U << CR1_OC_ADJ_SET_Pos)
// #define CR1_OC_ADJ_SET_1            (0x2U << CR1_OC_ADJ_SET_Pos)
// #define CR1_OC_ADJ_SET_2            (0x3U << CR1_OC_ADJ_SET_Pos)
// #define CR1_OC_ADJ_SET_3            (0x4U << CR1_OC_ADJ_SET_Pos)
// #define CR1_OC_ADJ_SET_4            (0x5U << CR1_OC_ADJ_SET_Pos)

#define CR1_OC_ADJ_SET_0            0x000U
#define CR1_OC_ADJ_SET_1            0x040U
#define CR1_OC_ADJ_SET_2            0x080U
#define CR1_OC_ADJ_SET_3            0x0C0U
#define CR1_OC_ADJ_SET_4            0x100U
#define CR1_OC_ADJ_SET_5            0x140U
#define CR1_OC_ADJ_SET_6            0x180U
#define CR1_OC_ADJ_SET_7            0x1C0U
#define CR1_OC_ADJ_SET_8            0x200U
#define CR1_OC_ADJ_SET_9            0x240U
#define CR1_OC_ADJ_SET_10           0x280U
#define CR1_OC_ADJ_SET_11           0x2C0U
#define CR1_OC_ADJ_SET_12           0x300U
#define CR1_OC_ADJ_SET_13           0x340U
#define CR1_OC_ADJ_SET_14           0x380U
#define CR1_OC_ADJ_SET_15           0x3C0U
#define CR1_OC_ADJ_SET_16           0x400U
#define CR1_OC_ADJ_SET_17           0x440U
#define CR1_OC_ADJ_SET_18           0x480U
#define CR1_OC_ADJ_SET_19           0x4C0U
#define CR1_OC_ADJ_SET_20           0x500U
#define CR1_OC_ADJ_SET_21           0x540U
#define CR1_OC_ADJ_SET_22           0x580U
#define CR1_OC_ADJ_SET_23           0x5C0U
#define CR1_OC_ADJ_SET_24           0x600U
#define CR1_OC_ADJ_SET_25           0x640U
#define CR1_OC_ADJ_SET_26           0x680U
#define CR1_OC_ADJ_SET_27           0x6C0U
#define CR1_OC_ADJ_SET_28           0x700U
#define CR1_OC_ADJ_SET_29           0x740U
#define CR1_OC_ADJ_SET_30           0x780U
#define CR1_OC_ADJ_SET_31           0x7C0U

/*******  Bit definitions for Control Register 2  *******/
#define CR2_OCTW_MODE_Pos           (0U)
#define CR2_OCTW_MODE_Msk           (0x2U << CR2_OCTW_MODE_Pos)
#define CR2_OCTW_MODE               CR2_OCTW_MODE_Msk
#define CR2_OCTW_MODE_0             (0x1U << CR2_OCTW_MODE_Pos)
#define CR2_OCTW_MODE_1             (0x2U << CR2_OCTW_MODE_Pos)

#define CR2_OCTW_MODE_OT_OC         0x000U
#define CR2_OCTW_MODE_OT            0x001U
#define CR2_OCTW_MODE_OC            0x002U

#define CR2_GAIN_Pos                (2U)
#define CR2_GAIN_Msk                (0x2U << CR2_GAIN_Pos)
#define CR2_GAIN                    CR2_GAIN_Msk
#define CR2_GAIN_Msk_0              (0x1U << CR2_GAIN_Pos)
#define CR2_GAIN_Msk_1              (0x2U << CR2_GAIN_Pos)

#define CR2_GAIN_10                 0x000U
#define CR2_GAIN_20                 0x004U
#define CR2_GAIN_40                 0x008U
#define CR2_GAIN_80                 0x00CU

#define CR2_DC_CAL_CH1_Pos          (4U)
#define CR2_DC_CAL_CH1_Msk          (0x1U << CR2_DC_CAL_CH1_Pos)
#define CR2_DC_CAL_CH1              CR2_DC_CAL_CH1_Msk
#define CR2_DC_CAL_CH2_Pos          (5U)
#define CR2_DC_CAL_CH2_Msk          (0x1U << CR2_DC_CAL_CH2_Pos)
#define CR2_DC_CAL_CH2              CR2_DC_CAL_CH2_Msk

#define CR2_OC_TOFF_Pos             (6U)
#define CR2_OC_TOFF_Msk             (0x1U << CR2_OC_TOFF_Pos)
#define CR2_OC_TOFF                 CR2_OC_TOFF_Msk



void drv8323_Init(drv8323_TypeDef *drv, SPI_TypeDef *spi);
uint16_t drv_transceive(drv8323_TypeDef *drv, uint16_t tx_reg);
uint16_t drv_write(drv8323_TypeDef *drv, uint32_t drv_reg, uint16_t payload);
uint16_t drv_transceive(drv8323_TypeDef *drv, uint16_t tx_reg);
uint16_t drv_read(drv8323_TypeDef *drv, uint32_t drv_reg);
void drv_write_reg(drv8323_TypeDef *drv, uint16_t reg);
void drv_read_reg(drv8323_TypeDef *drv, uint16_t reg);
void drv_update(drv8323_TypeDef *drv);


#endif // endif __DRV8323_H__
