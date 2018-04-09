/*
 * SPI.h
 *
 *  Created on: Feb 23, 2016
 *      Author: Emilio
 */

#ifndef SPI_H_
#define SPI_H_

#include "GPIO.h"
#include "typedefs.h"

void initSPI();
void SPI_Send_byte(UINT8 u8Data);
UINT8 SPI_Receive_byte(void);
void SPI_High_rate(void);


#endif /* SPI_H_ */
