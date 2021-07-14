/*
 * pitot.c
 *
 *  Created on: Jul 14, 2021
 *      Author: rockets
 */
#include "pitot.h"


HAL_StatusTypeDef  init_pitot(){

	uint8_t* commande = START_AVERAGE16;//ou un autre mode changer si nessesaire

	HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(&hi2c2,ADDR_PITOT,commande,1,HAL_MAX_DELAY);

	return ret;
}

int read_pitot(uint32_t* pression, uint32_t* temperature){

	uint8_t buf[7];
	HAL_StatusTypeDef ret;
	uint8_t status1;
	uint8_t status2;
	int success = 0;

	ret = HAL_I2C_Master_Receive(&hi2c2,ADDR_PITOT,buf,1,HAL_MAX_DELAY);
	if(ret != HAL_OK){

		//code d'erreur

	}else{
		status1 = buf[0];
		ret = HAL_I2C_Master_Receive(&hi2c2,ADDR_PITOT,buf,7,HAL_MAX_DELAY);
		if(ret != HAL_OK){

				//code d'erreur

			}else{
				status2 = buf[0];
				*pression = (uint32_t)(buf[1]<<16|buf[2]<<8|buf[3]);
				*temperature = (uint32_t)(buf[4]<<16|buf[5]<<8|buf[6]);
				
				
				success=1;
			}
	}

	return success;

}

