/*
 * pitot.c
 *
 *  Created on: Jul 14, 2021
 *      Author: rockets
 */
#include "../Inc/pitot.h"
#include "math.h"


HAL_StatusTypeDef  init_pitot(I2C_HandleTypeDef* hi2c){

	uint8_t commande[1];
	commande[0] = START_AVERAGE8;//ou un autre mode changer si nessesaire

	HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(hi2c,(uint16_t)ADDR_PITOT<<1,commande,1,HAL_MAX_DELAY);

	return ret;
}

int read_pitot(I2C_HandleTypeDef* hi2c, double* pression, double* temperature){

	uint8_t buf[7];
	HAL_StatusTypeDef ret;
	uint8_t status1;
	uint8_t status2;
	int success = 0;

	/*uint8_t commande[1];
	commande[0] = START_AVERAGE16;//ou un autre mode changer si nessesaire
	ret = HAL_I2C_Master_Transmit(hi2c,(uint16_t)ADDR_PITOT<<1,commande,1,HAL_MAX_DELAY);
	if(ret != HAL_OK){

		//code d'erreur

	}else{*/
		ret = HAL_I2C_Master_Receive(hi2c,(uint16_t)(ADDR_PITOT<<1)+1,buf,1,HAL_MAX_DELAY);
		if(ret != HAL_OK){

			//code d'erreur

		}else{
			status1 = buf[0];
			ret = HAL_I2C_Master_Receive(hi2c,(uint16_t)(ADDR_PITOT<<1)+1,buf,7,HAL_MAX_DELAY);
			if(ret != HAL_OK){

				//code d'erreur

			}else{
				status2 = buf[0];
				uint32_t pressionbrut = (uint32_t)(buf[1]<<16|buf[2]<<8|buf[3]);
				uint32_t temperaturebrut = (uint32_t)(buf[4]<<16|buf[5]<<8|buf[6]);

				uint32_t nbBits = pow(2,24);

				double divisionPression = ((pressionbrut - (0.1 * nbBits))/nbBits);
				double divisionTemp = ((temperaturebrut * 125.0)/nbBits);

				*pression = 1.25 * divisionPression * 10.0;
				*temperature = divisionTemp-40.0;
				success=1;
			}
		}
	//}

	return success;

}

