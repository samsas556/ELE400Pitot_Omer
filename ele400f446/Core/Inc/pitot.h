/*
 * pitot.h
 *
 *  Created on: Jul 14, 2021
 *      Author: rockets
 */

#ifndef INC_PITOT_H_
#define INC_PITOT_H_

#include "stm32f4xx_hal.h"

#define ADDR_PITOT 0x29 // non shift vers la droite
#define START_SINGLE (uint8_t)0xAA
#define START_AVERAGE2 (uint8_t)0xAC
#define START_AVERAGE4 (uint8_t)0xAD
#define START_AVERAGE8 (uint8_t)0xAE
#define START_AVERAGE16 (uint8_t)0xAF


HAL_StatusTypeDef init_pitot(I2C_HandleTypeDef* hi2c);



int read_pitot(I2C_HandleTypeDef* hi2c , double* pression, double* temperature);


#endif /* INC_PITOT_H_ */
