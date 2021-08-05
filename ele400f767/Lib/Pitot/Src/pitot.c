/*
 * pitot.c
 *
 *  Created on: Jul 14, 2021
 *      Author: rockets
 */
#include "../Inc/pitot.h"
#include "math.h"


HAL_StatusTypeDef  init_pitot(I2C_HandleTypeDef* hi2c, int operationMod){

	uint8_t commande[1];
	commande[0] = operationMod;//choose your average

	HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(hi2c,(uint16_t)ADDR_PITOT<<1,commande,1,HAL_MAX_DELAY);

	return ret;
}

int read_pitot_pressure_temperature(I2C_HandleTypeDef* hi2c, double* pressure, double* temperature){

	uint8_t buf[7];
	HAL_StatusTypeDef ret;
	uint8_t status1;
	uint8_t status2;
	int success = 0;

	ret = HAL_I2C_Master_Receive(hi2c,(uint16_t)(ADDR_PITOT<<1)+1,buf,1,HAL_MAX_DELAY);
	if(ret != HAL_OK){

		//err code

	}else{
		status1 = buf[0];
		ret = HAL_I2C_Master_Receive(hi2c,(uint16_t)(ADDR_PITOT<<1)+1,buf,7,HAL_MAX_DELAY);
		if(ret != HAL_OK){

			//err code

		}else{
			status2 = buf[0];
			uint32_t P_out = (uint32_t)(buf[1]<<16|buf[2]<<8|buf[3]);
			uint32_t T_out = (uint32_t)(buf[4]<<16|buf[5]<<8|buf[6]);

			uint32_t nbBits = pow(2,24);

			*pressure = 1.25 * ((P_out - (OS_DIG * nbBits))/nbBits) * (double)MAX_RANGE_MULTIPLIER * (double)MAX_RANGE;
			*temperature = ((T_out * 125.0)/nbBits)-40.0;
			success=1;
		}
	}
	return success;

}

int read_pitot_speed(I2C_HandleTypeDef* hi2c , double* speed){

	double pressure;

	int success = read_pitot_pressure_temperature(hi2c,&pressure,NULL);

	if(success){
		double speed_square = (double)SPEED_CALIBRATION_CONSTANT*(pressure/1000.0);
		*speed = sqrt(speed_square);
	}
	return success;
}

