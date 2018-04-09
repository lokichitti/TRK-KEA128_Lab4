/*
 * SPI.c
 *
 *  Created on: Feb 23, 2016
 *      Author: Emilio
 */

#include "SPI.h"

void initSPI(){
	//C1[SPISWAI] = 0;
	CONFIG_PIN_AS_GPIO(PORT_E,0,OUTPUT); /* CLK*/
	CONFIG_PIN_AS_GPIO(PORT_E,1,OUTPUT); /* MOSI*/
	CONFIG_PIN_AS_GPIO(PORT_E,2,INPUT); /* MISO*/
	CONFIG_PIN_AS_GPIO(PORT_E,3,OUTPUT); /*PS*/
	OUTPUT_SET(PORT_E,3);
	
	SIM_SCGC |= SIM_SCGC_SPI0_MASK;
	SIM_PINSEL0 |= SIM_PINSEL_SPI0PS_MASK;
	
	SPI0_C1 |= SPI_C1_MSTR_MASK; // SPI0 master mode  
	                          
	SPI0_BR = 0b00000000;//1MHzSPI_BR_SPR(0x06);  //SPPR = 4, SPR = 3, bps div = (SPPR+1)*2^(SPR+1) = 80, baudrate= 24Mhz/80=300khz  
	SPI0_C1 |= SPI_C1_SSOE_MASK;                            
	SPI0_C1 &= (~SPI_C1_CPHA_MASK);  // clock polarity  
	SPI0_C1 &= (~SPI_C1_CPOL_MASK);  //clock phase  
	SPI0_C1 &= (~SPI_C1_LSBFE_MASK);  // LSB:most significant   
  
	SPI0_C1 &= (~SPI_C1_SPIE_MASK);                  //Disable RX interrrupt    
	SPI0_C1 &= (~SPI_C1_SPTIE_MASK);         //Disable the transmit interrupt   
	SPI0_C2 |= SPI_C2_MODFEN_MASK;   
		  
	SPI0_C1 |= SPI_C1_SPE_MASK;  // enable SPI module	
	
}

void Soft_SPI_Write(UINT8 u8Data){
	
	while ((SPI0_S & SPI_S_SPTEF_MASK) == 0);
	(void)SPI0_S;
	SPI0_D=u8Data;
	
	
}
UINT8 SPI_Receive_byte(void)
{
	(void)SPI0_D;
	SPI0_D=0xff;
	//SPI_Send_byte(0xff);
	while ((SPI0_S & SPI_S_SPRF_MASK) == 0);
	return SPI0_D;
}


void SPI_High_rate(void)
{
	SPI0_C1 &= (~SPI_C1_SPE_MASK);
  SPI0_BR = 0x01; //11 3 MHz 24MHz  -  01 4MHz 16MHz     		
  SPI0_C1 |= SPI_C1_SPE_MASK; 

}
