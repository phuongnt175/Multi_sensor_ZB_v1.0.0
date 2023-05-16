/*
 * Si7020.c
 *
 *  Created on: Mar 2, 2023
 *      Author: admin1
 */


// Transmission flags
#include "app/framework/include/af.h"
#include "Si7020.h"



/**
 * @func    initI2C
 * @brief   Initialize I2C communicate sensor
 * @param   None
 * @retval  None
 */
void initI2C(void)
{
  CMU_ClockEnable(cmuClock_I2C0, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Use default settings
  I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

  // Using PB0 (SDA) and PB1 (SCL)
  GPIO_PinModeSet(I2C_GPIO, I2C0_PIN_SDA, gpioModeWiredAndPullUpFilter, 1);
  GPIO_PinModeSet(I2C_GPIO, I2C0_PIN_SCL, gpioModeWiredAndPullUpFilter, 1);

  // Route I2C pins to GPIO
  GPIO->I2CROUTE[0].SDAROUTE = (GPIO->I2CROUTE[0].SDAROUTE & ~_GPIO_I2C_SDAROUTE_MASK)
								| (I2C_GPIO << _GPIO_I2C_SDAROUTE_PORT_SHIFT
								| (I2C0_PIN_SDA << _GPIO_I2C_SDAROUTE_PIN_SHIFT));
  GPIO->I2CROUTE[0].SCLROUTE = (GPIO->I2CROUTE[0].SCLROUTE & ~_GPIO_I2C_SCLROUTE_MASK)
								| (I2C_GPIO << _GPIO_I2C_SCLROUTE_PORT_SHIFT
								| (I2C0_PIN_SCL << _GPIO_I2C_SCLROUTE_PIN_SHIFT));
  GPIO->I2CROUTE[0].ROUTEEN = GPIO_I2C_ROUTEEN_SDAPEN | GPIO_I2C_ROUTEEN_SCLPEN;

  // Initialize the I2C
  I2C_Init(I2C0, &i2cInit);

  // Enable automatic STOP on NACK
  I2C0->CTRL = I2C_CTRL_AUTOSN;

}

/**
 * @func    Si7020_Init
 * @brief   Initialize Si7020
 * @param   None
 * @retval  None
 */
void Si7020_Init (void){
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;
	uint8_t                    i2c_read_data[3];	// detect ID: SI7020_DEVICE_ID  and checksum byte
	uint8_t                    i2c_write_data[2];	// command Electronic ID 2nd Byte

	seq.addr  = SI7020_ADDR << 1 ;  			//(Address Si7020: 0x40 << 1 for bit R/W)
	seq.flags = I2C_FLAG_WRITE_READ;

	/* Select command to issue */
	i2c_write_data[0] = SI7020_READ_ID_1;
	i2c_write_data[1] = SI7020_READ_ID_2;

	seq.buf[0].data   = i2c_write_data;
	seq.buf[0].len    = 2;

	/* Select location/length of data to be read */
	seq.buf[1].data = i2c_read_data;
	seq.buf[1].len  = 3;

	// Sending data
	ret = I2C_TransferInit(I2C0, &seq);
	while (ret == i2cTransferInProgress)
	{
		ret = I2C_Transfer(I2C0);
	}
	emberAfCorePrintln (" Detect ret: %d",ret);

	if ((ret != i2cTransferDone) | (i2c_read_data[0] != SI7020_DEVICE_ID)) {
		emberAfCorePrintln (" Detect Si7020 failure!--Device Id:%d ",i2c_read_data[0]);
	}else {
	emberAfCorePrintln (" Detect Si7020 success! ");
	}
}


/**
 * @func    Si7020_Measure
 * @brief   Measure
 * @param   *buffer, command
 * @retval  Retval
 */
boolean Si7020_Measure (uint32_t *buffer, uint8_t command, uint8_t Length_Data){
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef retVal;
	uint8_t                    i2c_read_data[Length_Data]; //i2c_read_data[0]: MSB		i2c_read_data[1]: LSB
	uint8_t                    i2c_write_data[1];

	seq.addr  = SI7020_ADDR << 1; //(Address Si7020: 0x40 << 1 for bit R/W)
	seq.flags = I2C_FLAG_WRITE_READ;

	/* Select command to issue */
	i2c_write_data[0] = command;			// command
	seq.buf[0].data   = i2c_write_data;
	seq.buf[0].len    = 1;

	/* Select location/length of data to be read */
	seq.buf[1].data = i2c_read_data;
	seq.buf[1].len  = 2;

	// Sending data
	retVal = I2C_TransferInit(I2C0, &seq);
	while (retVal == i2cTransferInProgress)
	{
		retVal = I2C_Transfer(I2C0);
	}

	*buffer = ((uint32_t) i2c_read_data[0] << 8) + (i2c_read_data[1]);   // // RH_CODE (MSB << 8) + LSB
	return true;
}

/**
 * @func    Si7020_MeasureTempAndHumi
 * @brief   Measure Humi
 * @param   *humiData
 * @retval  Humi
 */
uint32_t Si7020_MeasureHumi (void){
	uint32_t humiData;
	boolean retVal = Si7020_Measure (&humiData, SI7020_READ_RH, 2);
	if (retVal)
		humiData = ( (((humiData) * 12500) >> 16) - 600 )/100; // Humi = ((Humi * 2500)/65536) - 6
	return humiData;
}

/**
 * @func    Si7020_MeasureTempAndHumi
 * @brief   Measure Temp
 * @param   *tempData
 * @retval  Temp
 */
uint32_t Si7020_MeasureTemp (void){
	uint32_t tempData;
	boolean retVal = Si7020_Measure (&tempData, SI7020_READ_TEMP, 2);
	if (retVal)
		tempData = ( (((tempData) * 17572) >> 16) - 4685 )/100; // Temp = ((Temp * 17572)/65536 - 46.85
	return tempData;
}
