#ifndef __DRV8303_H__
#define __DRV8303_H__

#include "stm32f103xb.h"
#include "spi.h"

typedef struct
{
    uint16_t SR1;
    uint16_t SR2;
    uint16_t CR1;
    uint16_t CR2;
    SPI_TypeDef *spi;
} drv8303_TypeDef;


/*******      Register Address Definitions       *******/
#define DRV_STATUS_1                0x0U << 11
#define DRV_STATUS_2                0x1U << 11
#define DRV_CTRL_1                  0x2U << 11
#define DRV_CTRL_2                  0x3U << 11

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



void drv8303_Init(drv8303_TypeDef *drv, SPI_TypeDef *spi);
uint16_t drv_transceive(drv8303_TypeDef *drv, uint16_t tx_reg);
uint16_t drv_write(drv8303_TypeDef *drv, uint32_t drv_reg, uint16_t payload);
uint16_t drv_transceive(drv8303_TypeDef *drv, uint16_t tx_reg);
uint16_t drv_read(drv8303_TypeDef *drv, uint32_t drv_reg);
void drv_write_reg(drv8303_TypeDef *drv, uint16_t reg);
void drv_read_reg(drv8303_TypeDef *drv, uint16_t reg);
void drv_update(drv8303_TypeDef *drv);


#endif // endif __DRV8303_H__
