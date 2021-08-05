/*
 * pitot.h
 *
 *  Created on: Jul 14, 2021
 *  Author: samuel Brassard
 */

#ifndef INC_PITOT_H_
#define INC_PITOT_H_

#include "stm32f7xx_hal.h"

/*
 * DATASHEET DLHR Sensor (http://www.allsensors.com/datasheets/DS-0350_Rev_C.pdf)
 */
#define OS_DIG_MODE 0//0=GAGE 1=DIFF

#if (!OS_DIG_MODE)
#define OS_DIG 0.1
#define MAX_RANGE_MULTIPLIER 1.0
#endif

#if (OS_DIG_MODE)
#define OS_DIG 0.5
#define MAX_RANGE_MULTIPLIER 2.0
#endif

#define MAX_RANGE 2500.0 //range in Pa

#define SPEED_CALIBRATION_CONSTANT 1.3

#define ADDR_PITOT 0x29
#define START_SINGLE (uint8_t)0xAA // init and wait for 4 ms minimum before read
#define START_AVERAGE2 (uint8_t)0xAC// init and wait for 7 ms minimum before read
#define START_AVERAGE4 (uint8_t)0xAD// init and wait for 14 ms minimum before read
#define START_AVERAGE8 (uint8_t)0xAE// init and wait for 28 ms minimum before read
#define START_AVERAGE16 (uint8_t)0xAF// init and wait for 58 ms minimum before read


HAL_StatusTypeDef init_pitot(I2C_HandleTypeDef* hi2c, int operationMod );

int read_pitot_pressure_temperature(I2C_HandleTypeDef* hi2c , double* pressure, double* temperature);

int read_pitot_speed(I2C_HandleTypeDef* hi2c , double* speed);


#endif /* INC_PITOT_H_ */
