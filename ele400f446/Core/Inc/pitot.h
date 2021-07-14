/*
 * pitot.h
 *
 *  Created on: Jul 14, 2021
 *      Author: rockets
 */

#ifndef INC_PITOT_H_
#define INC_PITOT_H_

#include "../Src/main.c"

#define ADDR_PITOT 41  // pt a shifter d'un bit vers la droite
#define START_SINGLE 0xAA
#define START_AVERAGE2 0xAC
#define START_AVERAGE4 0xAD
#define START_AVERAGE8 0xAE
#define START_AVERAGE16 0xAF

HAL_StatusTypeDef init_pitot(void);

int read_pitot(uint32_t* pression, uint32_t* temperature);


#endif /* INC_PITOT_H_ */
