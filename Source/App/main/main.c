 /* File name: main.c
 *
 * Description:
 *
 *
 * Last Changed By:  $Author: $
 * Revision:         $Revision: $
 * Last Changed:     $Date: $May 16, 2023
 *
 * Code sample:
 ******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "app/framework/include/af.h"
#include "Source/Mid/Kalman_filter/kalman_filter.h"
#include "Source/Mid/LDR/ldr.h"
#include "Source/Mid/Si7020/Si7020.h"
#include "em_chip.h"
#include "main.h"

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/
#define PERIOD_SCAN_SENSORLIGHT									5000 	//	ms
#define PERIOD_SCAN_SENSORTEMHUMI								10000	//	ms

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
uint32_t KalmanHumi=0;
uint32_t KalmanTemp=0;

/* Event **************************************************************/
EmberEventControl readValueSensorLightControl;
EmberEventControl ReadValueTempHumiControl;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
/*
 * * @brief Main Init
 */
void emberAfMainInitCallback(void)
{
	emberAfCorePrintln("Main Init");
	LDRInit();
	initI2C();
	Si7020_Init();
	KalmanFilterInit(2, 2, 0.001); // Initialize Kalman filter
	emberEventControlSetDelayMS(readValueSensorLightControl, 1000);
	emberEventControlSetDelayMS(ReadValueTempHumiControl,1000);
}

/**
 * @func   LightSensor_AdcPollingReadHandler
 * @brief  Read value from ADC
 * @param  None
 * @retval None
 */
void LightSensor_AdcPollingReadHandler(void)
{
	uint32_t lux;
	emberEventControlSetInactive(readValueSensorLightControl);
	lux = LightSensor_AdcPollingRead();
	emberAfCorePrintln("Light:   %d lux         ",lux);
	emberEventControlSetDelayMS(readValueSensorLightControl,PERIOD_SCAN_SENSORLIGHT);
}

/**
 * @func    ReadValueTempHumiHandler
 * @brief   Event Sensor Handler v1
 * @param   None
 * @retval  None
 */
void ReadValueTempHumiHandler(void)
{
	emberEventControlSetInactive(ReadValueTempHumiControl);
	KalmanHumi = Si7020_MeasureHumi();
	KalmanTemp = Si7020_MeasureTemp();
	emberAfCorePrintln("Humi:    %d RH       Temp:     %d oC        ", KalmanHumi,KalmanTemp);
	emberEventControlSetDelayMS(ReadValueTempHumiControl,PERIOD_SCAN_SENSORTEMHUMI);
}

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/

