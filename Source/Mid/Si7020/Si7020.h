 /* File name: Si7020.h
 *
 * Description:
 *
 *
 * Last Changed By:  $Author: $
 * Revision:         $Revision: $
 * Last Changed:     $Date: $April 17, 2023
 *
 * Code sample:
 ******************************************************************************/

#ifndef SOURCE_MID_SI7020_SI7020_H_
#define SOURCE_MID_SI7020_SI7020_H_

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_i2c.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "bsp.h"

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/


/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

#define I2C_GPIO							 gpioPortB
#define I2C0_PIN_SDA			    		 	 1
#define I2C0_PIN_SCL			    			 0
#define SCAN_TEMP_AND_HUMI						10000

/* I2C device address for Si7020 */
#define SI7020_ADDR            0x40

/* Device ID value for Si7020 */
#define SI7020_DEVICE_ID       0x14

/* Si7020 Read Temperature Command */
#define SI7020_READ_TEMP       0xE0

/* Si7020 Read RH Command */
#define SI7020_READ_RH         0xE5

/* SI7020 ID */
#define SI7020_READ_ID_1       0xFC
#define SI7020_READ_ID_2       0xC9

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/

void i2cInit(void);
void si7020Init (void);
boolean Si7020Measure (uint32_t *buffer, uint8_t command, uint8_t Length_Data);
uint32_t Si7020MeasureHumi (void);
uint32_t Si7020MeasureTemp (void);
/******************************************************************************/

#endif /* SOURCE_MID_SI7020_SI7020_H_ */
