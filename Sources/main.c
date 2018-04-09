/******************************************************************************								
*    LAB4 -Keyboard Interrupt (KBI) Module                                    *								
*                                                                             *								
*       This lab demonstrates how to use the KBI module to read button        *								
*    presses without polling. It uses interrupts to detect button presses on  *								
*    SW1 and SW2. Each button toggles an LED when pressed.                    *								
*       Press buttons SW0 or SW1 to toggle its corresponding LED              *								
******************************************************************************/		


/**********************************************************************************************
* External objects
**********************************************************************************************/
#include "derivative.h" /* include peripheral declarations */
#include "CLK.h"
#include "GPIO.h"
#include "KBI.h"
/**********************************************************************************************
* Global variables
**********************************************************************************************/


/**********************************************************************************************
* Constants and macros
**********************************************************************************************/
#define LED0_TOGGLE		OUTPUT_TOGGLE(PORT_C,16)
#define LED1_TOGGLE		OUTPUT_TOGGLE(PORT_C,17)
#define LED2_TOGGLE		OUTPUT_TOGGLE(PORT_C,18)
#define LED3_TOGGLE		OUTPUT_TOGGLE(PORT_C,19)

#define LED0_OFF		OUTPUT_CLEAR(PORT_C,16);
#define LED1_OFF		OUTPUT_CLEAR(PORT_C,17);
#define LED2_OFF		OUTPUT_CLEAR(PORT_C,18);
#define LED3_OFF		OUTPUT_CLEAR(PORT_C,19);

#define LED0_ON			OUTPUT_SET(PORT_C,16);
#define LED1_ON			OUTPUT_SET(PORT_C,17);
#define LED2_ON			OUTPUT_SET(PORT_C,18);
#define LED3_ON			OUTPUT_SET(PORT_C,19);


// MAX7219 SPI LED Driver
#define MAX7219_TEST 0x0f // in real code put into a .h file
#define MAX7219_BRIGHTNESS 0x0a // in real code put into a .h file
#define MAX7219_SCAN_LIMIT 0x0b // in real code put into a .h file
#define MAX7219_DECODE_MODE 0x09 // in real code put into a .h file
#define MAX7219_SHUTDOWN 0x0C // in real code put into a .h file


/**********************************************************************************************
* Local types
**********************************************************************************************/
const unsigned short CharData[][8] ={
{0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
{0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000000, 0b00000100},
{0b00001010, 0b00001010, 0b00001010, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
{0b00000000, 0b00001010, 0b00011111, 0b00001010, 0b00011111, 0b00001010, 0b00011111, 0b00001010},
{0b00000111, 0b00001100, 0b00010100, 0b00001100, 0b00000110, 0b00000101, 0b00000110, 0b00011100},
{0b00011001, 0b00011010, 0b00000010, 0b00000100, 0b00000100, 0b00001000, 0b00001011, 0b00010011},
{0b00000110, 0b00001010, 0b00010010, 0b00010100, 0b00001001, 0b00010110, 0b00010110, 0b00001001},
{0b00000100, 0b00000100, 0b00000100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
{0b00000010, 0b00000100, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00000100, 0b00000010},
{0b00001000, 0b00000100, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000100, 0b00001000},
{0b00010101, 0b00001110, 0b00011111, 0b00001110, 0b00010101, 0b00000000, 0b00000000, 0b00000000},
{0b00000000, 0b00000000, 0b00000100, 0b00000100, 0b00011111, 0b00000100, 0b00000100, 0b00000000},
{0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000110, 0b00000100, 0b00001000},
{0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001110, 0b00000000, 0b00000000, 0b00000000},
{0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000100},
{0b00000001, 0b00000010, 0b00000010, 0b00000100, 0b00000100, 0b00001000, 0b00001000, 0b00010000},
{0b00001110, 0b00010001, 0b00010011, 0b00010001, 0b00010101, 0b00010001, 0b00011001, 0b00001110},
{0b00000100, 0b00001100, 0b00010100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00011111},
{0b00001110, 0b00010001, 0b00010001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00011111},
{0b00001110, 0b00010001, 0b00000001, 0b00001110, 0b00000001, 0b00000001, 0b00010001, 0b00001110},
{0b00010000, 0b00010000, 0b00010100, 0b00010100, 0b00011111, 0b00000100, 0b00000100, 0b00000100},
{0b00011111, 0b00010000, 0b00010000, 0b00011110, 0b00000001, 0b00000001, 0b00000001, 0b00011110},
{0b00000111, 0b00001000, 0b00010000, 0b00011110, 0b00010001, 0b00010001, 0b00010001, 0b00001110},
{0b00011111, 0b00000001, 0b00000001, 0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000},
{0b00001110, 0b00010001, 0b00010001, 0b00001110, 0b00010001, 0b00010001, 0b00010001, 0b00001110},
{0b00001110, 0b00010001, 0b00010001, 0b00001111, 0b00000001, 0b00000001, 0b00000001, 0b00000001},
{0b00000000, 0b00000100, 0b00000100, 0b00000000, 0b00000000, 0b00000100, 0b00000100, 0b00000000},
{0b00000000, 0b00000100, 0b00000100, 0b00000000, 0b00000000, 0b00000100, 0b00000100, 0b00001000},
{0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00001000, 0b00000100, 0b00000010, 0b00000001},
{0b00000000, 0b00000000, 0b00000000, 0b00011110, 0b00000000, 0b00011110, 0b00000000, 0b00000000},
{0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000010, 0b00000100, 0b00001000, 0b00010000},
{0b00001110, 0b00010001, 0b00010001, 0b00000010, 0b00000100, 0b00000100, 0b00000000, 0b00000100},
{0b00001110, 0b00010001, 0b00010001, 0b00010101, 0b00010101, 0b00010001, 0b00010001, 0b00011110},
{0b00001110, 0b00010001, 0b00010001, 0b00010001, 0b00011111, 0b00010001, 0b00010001, 0b00010001},
{0b00011110, 0b00010001, 0b00010001, 0b00011110, 0b00010001, 0b00010001, 0b00010001, 0b00011110},
{0b00000111, 0b00001000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00001000, 0b00000111},
{0b00011100, 0b00010010, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010010, 0b00011100},
{0b00011111, 0b00010000, 0b00010000, 0b00011110, 0b00010000, 0b00010000, 0b00010000, 0b00011111},
{0b00011111, 0b00010000, 0b00010000, 0b00011110, 0b00010000, 0b00010000, 0b00010000, 0b00010000},
{0b00001110, 0b00010001, 0b00010000, 0b00010000, 0b00010111, 0b00010001, 0b00010001, 0b00001110},
{0b00010001, 0b00010001, 0b00010001, 0b00011111, 0b00010001, 0b00010001, 0b00010001, 0b00010001},
{0b00011111, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00011111},
{0b00011111, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00010100, 0b00001000},
{0b00010001, 0b00010010, 0b00010100, 0b00011000, 0b00010100, 0b00010010, 0b00010001, 0b00010001},
{0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00011111},
{0b00010001, 0b00011011, 0b00011111, 0b00010101, 0b00010001, 0b00010001, 0b00010001, 0b00010001},
{0b00010001, 0b00011001, 0b00011001, 0b00010101, 0b00010101, 0b00010011, 0b00010011, 0b00010001},
{0b00001110, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00001110},
{0b00011110, 0b00010001, 0b00010001, 0b00011110, 0b00010000, 0b00010000, 0b00010000, 0b00010000},
{0b00001110, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010101, 0b00010011, 0b00001111},
{0b00011110, 0b00010001, 0b00010001, 0b00011110, 0b00010100, 0b00010010, 0b00010001, 0b00010001},
{0b00001110, 0b00010001, 0b00010000, 0b00001000, 0b00000110, 0b00000001, 0b00010001, 0b00001110},
{0b00011111, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100},
{0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00001110},
{0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00001010, 0b00000100},
{0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010101, 0b00010101, 0b00001010},
{0b00010001, 0b00010001, 0b00001010, 0b00000100, 0b00000100, 0b00001010, 0b00010001, 0b00010001},
{0b00010001, 0b00010001, 0b00001010, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100},
{0b00011111, 0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00010000, 0b00011111},
{0b00001110, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001110},
{0b00010000, 0b00001000, 0b00001000, 0b00000100, 0b00000100, 0b00000010, 0b00000010, 0b00000001},
{0b00001110, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00001110},
{0b00000100, 0b00001010, 0b00010001, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
{0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011111},
{0b00001000, 0b00000100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
{0b00000000, 0b00000000, 0b00000000, 0b00001110, 0b00010010, 0b00010010, 0b00010010, 0b00001111},
{0b00000000, 0b00010000, 0b00010000, 0b00010000, 0b00011100, 0b00010010, 0b00010010, 0b00011100},
{0b00000000, 0b00000000, 0b00000000, 0b00001110, 0b00010000, 0b00010000, 0b00010000, 0b00001110},
{0b00000000, 0b00000001, 0b00000001, 0b00000001, 0b00000111, 0b00001001, 0b00001001, 0b00000111},
{0b00000000, 0b00000000, 0b00000000, 0b00011100, 0b00010010, 0b00011110, 0b00010000, 0b00001110},
{0b00000000, 0b00000011, 0b00000100, 0b00000100, 0b00000110, 0b00000100, 0b00000100, 0b00000100},
{0b00000000, 0b00001110, 0b00001010, 0b00001010, 0b00001110, 0b00000010, 0b00000010, 0b00001100},
{0b00000000, 0b00010000, 0b00010000, 0b00010000, 0b00011100, 0b00010010, 0b00010010, 0b00010010},
{0b00000000, 0b00000000, 0b00000100, 0b00000000, 0b00000100, 0b00000100, 0b00000100, 0b00000100},
{0b00000000, 0b00000010, 0b00000000, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00001100},
{0b00000000, 0b00010000, 0b00010000, 0b00010100, 0b00011000, 0b00011000, 0b00010100, 0b00010000},
{0b00000000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00001100},
{0b00000000, 0b00000000, 0b00000000, 0b00001010, 0b00010101, 0b00010001, 0b00010001, 0b00010001},
{0b00000000, 0b00000000, 0b00000000, 0b00010100, 0b00011010, 0b00010010, 0b00010010, 0b00010010},
{0b00000000, 0b00000000, 0b00000000, 0b00001100, 0b00010010, 0b00010010, 0b00010010, 0b00001100},
{0b00000000, 0b00011100, 0b00010010, 0b00010010, 0b00011100, 0b00010000, 0b00010000, 0b00010000},
{0b00000000, 0b00001110, 0b00010010, 0b00010010, 0b00001110, 0b00000010, 0b00000010, 0b00000001},
{0b00000000, 0b00000000, 0b00000000, 0b00001010, 0b00001100, 0b00001000, 0b00001000, 0b00001000},
{0b00000000, 0b00000000, 0b00001110, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00011110},
{0b00000000, 0b00010000, 0b00010000, 0b00011100, 0b00010000, 0b00010000, 0b00010000, 0b00001100},
{0b00000000, 0b00000000, 0b00000000, 0b00010010, 0b00010010, 0b00010010, 0b00010010, 0b00001100},
{0b00000000, 0b00000000, 0b00000000, 0b00010001, 0b00010001, 0b00010001, 0b00001010, 0b00000100},
{0b00000000, 0b00000000, 0b00000000, 0b00010001, 0b00010001, 0b00010001, 0b00010101, 0b00001010},
{0b00000000, 0b00000000, 0b00000000, 0b00010001, 0b00001010, 0b00000100, 0b00001010, 0b00010001},
{0b00000000, 0b00000000, 0b00010001, 0b00001010, 0b00000100, 0b00001000, 0b00001000, 0b00010000},
{0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00000010, 0b00000100, 0b00001000, 0b00011111},
{0b00000010, 0b00000100, 0b00000100, 0b00000100, 0b00001000, 0b00000100, 0b00000100, 0b00000010},
{0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100},
{0b00001000, 0b00000100, 0b00000100, 0b00000100, 0b00000010, 0b00000100, 0b00000100, 0b00001000},
{0b00000000, 0b00000000, 0b00000000, 0b00001010, 0b00011110, 0b00010100, 0b00000000, 0b00000000}
};

/**********************************************************************************************
* Local function prototypes
*********************************************************************************************/
void KBI_Interrupt(UINT8 ch);
void Enable_Interrupt(UINT8 vector_number);

/**********************************************************************************************
* Local variables
**********************************************************************************************/


/**********************************************************************************************
* Local functions
**********************************************************************************************/


/**********************************************************************************************
* Global functions
**********************************************************************************************/
/***********************************************************************************************
*
* @brief    GPIO_Init - Initialize the LEDs as outputs
* @param    none
* @return   none
*
************************************************************************************************/
void GPIO_Init()
{
	CONFIG_PIN_AS_GPIO(PORT_C,16,OUTPUT); /* Configure LED 0 (PTC0) as an output */
	CONFIG_PIN_AS_GPIO(PORT_C,17,OUTPUT); /* Configure LED 1 (PTC1) as an output */
	CONFIG_PIN_AS_GPIO(PORT_C,18,OUTPUT); /* Configure LED 2 (PTC2) as an output */
	CONFIG_PIN_AS_GPIO(PORT_C,19,OUTPUT); /* Configure LED 3 (PTC3) as an output */
	
	CONFIG_PIN_AS_GPIO(PORT_D,24,INPUT); /* Configure SW0 (PTD0) as an input */	
	CONFIG_PIN_AS_GPIO(PORT_D,25,INPUT); /* Configure SW1 (PTD1) as an input */
	ENABLE_INPUT(PORT_D,24);			 /* Enable input SW1*/	
	ENABLE_INPUT(PORT_D,25);			/*  Enable input SW2*/
	
	
	LED0_OFF;							/* Turn off LED0 */
	LED1_OFF;							/* Turn off LED1 */
	LED2_OFF;							/* Turn off LED2 */
	LED3_OFF;							/* Turn off LED3 */

}

// max7219 configuration
void max7219_init1()
{
OUTPUT_CLEAR(PORT_E,3);       // SELECT MAX
 Soft_SPI_Write(0x09);      //  Disable BCD mode for digit decoding
 OUTPUT_SET(PORT_E,3);       // DESELECT MAX
 
 OUTPUT_CLEAR(PORT_E,3); 
Soft_SPI_Write(0x0A);     //  Max segment luminosity intensity
 Soft_SPI_Write(0x0F);
 OUTPUT_SET(PORT_E,3);
 
 OUTPUT_CLEAR(PORT_E,3); 
 Soft_SPI_Write(0x0B);
 Soft_SPI_Write(0x07);      // Display refresh
 OUTPUT_SET(PORT_E,3);
 OUTPUT_CLEAR(PORT_E,3); 
 Soft_SPI_Write(0x0C);
 Soft_SPI_Write(0x01);      // Turn on the display
 OUTPUT_SET(PORT_E,3);
 
 OUTPUT_CLEAR(PORT_E,3); 
 Soft_SPI_Write(0x00);
 Soft_SPI_Write(0xFF);      // No test
 OUTPUT_SET(PORT_E,3);
}
 
void max7219_init2() {
	OUTPUT_CLEAR(PORT_E,3);        // SELECT MAX
  Soft_SPI_Write(0x09);      // BCD mode for digit decoding
  Soft_SPI_Write(0xFF);      // using Code-B
  Soft_SPI_Write(0x00);    // Bypass the 1st Chip
  Soft_SPI_Write(0x00);
  OUTPUT_SET(PORT_E,3);       // DESELECT MAX
 
  OUTPUT_CLEAR(PORT_E,3);        // SELECT MAX
  Soft_SPI_Write(0x0A);
  Soft_SPI_Write(0x0F);      // Segment luminosity intensity
  Soft_SPI_Write(0x00);    // Bypass the 1st Chip
  Soft_SPI_Write(0x00);
  OUTPUT_SET(PORT_E,3);       // DESELECT MAX
 
  OUTPUT_CLEAR(PORT_E,3);        // SELECT MAX
 Soft_SPI_Write(0x0B);
  Soft_SPI_Write(0x07);      // Display refresh
  Soft_SPI_Write(0x00);    // Bypass the 1st Chip
  Soft_SPI_Write(0x00);
  OUTPUT_SET(PORT_E,3);       // DESELECT MAX
 
  OUTPUT_CLEAR(PORT_E,3);        // SELECT MAX
  Soft_SPI_Write(0x0C);
  Soft_SPI_Write(0x01);      // Turn on the display
  Soft_SPI_Write(0x00);    // Bypass the 1st Chip
  Soft_SPI_Write(0x00);
  OUTPUT_SET(PORT_E,3);       // DESELECT MAX
 
  OUTPUT_CLEAR(PORT_E,3);       // SELECT MAX
  Soft_SPI_Write(0x00);
  Soft_SPI_Write(0xFF);      // No test
  Soft_SPI_Write(0x00);    // Bypass the 1st Chip
  Soft_SPI_Write(0x00);
  OUTPUT_SET(PORT_E,3);
  
  }
//Send data function
void Write_Byte(unsigned short myRow, unsigned short myValue)
{
	OUTPUT_CLEAR(PORT_E,3);        // select max7219.
 Soft_SPI_Write(myRow);  // send myRow value to max7219 (digit place).
 Soft_SPI_Write(myValue);
 
 OUTPUT_SET(PORT_E,3);       // deselect max7219.
 }
 
 void Write_Byte1(unsigned short myRow, unsigned short myValue)
{
	 OUTPUT_CLEAR(PORT_E,3); 
      // select ma7219.
 Soft_SPI_Write(myRow); 
 Soft_SPI_Write(myValue);
  Soft_SPI_Write(0x00);    // Bypass the 1st Chip
  Soft_SPI_Write(0x00);
 
  OUTPUT_SET(PORT_E,3);   
 }
 
//Clear matrix function
void Clear_Matrix(void)
{
 unsigned short x;
 
 for(x=1;x<9;x++)
 {
  Write_Byte(x,0xFF);
  Write_Byte1(x+8,0xFF);
  }
}
/***********************************************************************************************
*
* @brief    main() - Program entry function
* @param    none
* @return   none
*
************************************************************************************************/  

void delay(int k)
{
	int i=0, j=0;
	for(i=0;i<32767;i++)
		for(j=0;j<k;j++){}
			
}
//void maxTransferCMD(unsigned char address, unsigned char value) {  
//unsigned char i;
//
//OUTPUT_CLEAR(PORT_E,2);  
//  SPI_Send_byte(address);      // Send address.
//  SPI_Send_byte(value);        // Send the value.
//  SPI_Send_byte(address);      // Send address. 
//  SPI_Send_byte(value);        // Send the value.
//  OUTPUT_SET(PORT_E,2); // Finish transfer.
//}
//
//void maxTransferDATA(unsigned char address, unsigned char value, unsigned char v2) {  
//	unsigned char i;
//
//OUTPUT_CLEAR(PORT_E,2);   
//
//SPI_Send_byte(address);      // Send address.
//SPI_Send_byte(value);        // Send the value.
//SPI_Send_byte(address);      // Send address. 
//SPI_Send_byte(v2);           // Send the value.
//
//  OUTPUT_SET(PORT_E,2);  // Finish transfer.
//}
//void maxTransfer(uint8_t address, uint8_t value) {
//
//  // Ensure LOAD/CS is LOW
//	OUTPUT_CLEAR(PORT_E,2);
//
//  // Send the register address
//  SPI_Send_byte(address);
//
//  // Send the value
//  SPI_Send_byte(value);
//
//  // Tell chip to load in data
//  OUTPUT_SET(PORT_E,2);
//}
//void loop() {  
//
//	unsigned char row=0;
//int i=0,ud=1; // Need signed numbers.
//
//   for(;;) {      
//
//      i += ud; 
//
//      if (i>255) {ud=-1;i=255;}
//
//      if (i<0)   {ud=1 ;i=0;}
//
//      if (row++>8) row=1;  
//     
//      maxTransferDATA(row, 0, 0);             
//      maxTransferCMD(MAX7219_BRIGHTNESS,i>>4);
//
//      delay(100); 
//   }
//}

unsigned int DisplayBuffer[]={0,0,0,0,0,0,0,0};
unsigned int DisplayBuffer1[]={0,0,0,0,0,0,0,0};
unsigned int speed=10;
short i, l, k, ShiftAmount, scroll, temp,temp1, shift_step=1, StringLength;
char message[]="MIPS LED MATRIX 2014" ;
char index,index1;
int main(void)
{
	int counter = 0;
	
	Clk_Init();						/* Configure clocks to run at 20 Mhz*/
	GPIO_Init();					/* Configure button pins as inputs and LED pins as outputs */
	initSPI();
	max7219_init1();
	 max7219_init2();
	 Clear_Matrix();
	 StringLength = strlen(message) ;
	//KBI_Init();						/* Initialize KBI module */
	//KBI_SetCallback(KBI_Interrupt);	/* Set the KBI callback function to be called on each button press */
	//Enable_Interrupt(INT_KBI0); 	/* Enable KBI0 Interrupts */
	LED0_ON
	LED2_ON
	LED3_ON
	LED1_ON
	
	while(1)
	 {
//	 for (k=0; k<StringLength; k++)
//	 {
//	  for (scroll=0; scroll<(8/shift_step); scroll++)
//	   {
//	         for (ShiftAmount=0; ShiftAmount<8; ShiftAmount++)
//	         {
//	          index = message[k];
//	          index1=message[k-1];
//	          temp =CharData[index-32][ShiftAmount];
//	          temp1 =CharData[index-32][ShiftAmount];
//	          
//	          DisplayBuffer[ShiftAmount] = (DisplayBuffer[ShiftAmount] << shift_step)| (temp >> ((8-shift_step)-scroll*shift_step));
//	          DisplayBuffer1[ShiftAmount] = (DisplayBuffer1[ShiftAmount] << shift_step)| (temp1 >> ((8-shift_step)-scroll*shift_step));
//	         }
//	 
//	 
//	        for(l=0; l<5;l++)
//	        {
//	                 for (i=0; i<8; i++)
//	                 {
//	                     Write_Byte(i+1,DisplayBuffer[i]);  //Scrolling k character on firt LED Matrix
//	                     if(k>0)
//	                     Write_Byte1(i+1,DisplayBuffer1[i]);//Scrolling k-1 character on second LED Matrix
//	 
//	                 }
//	        }
//	 
//	 
//	   }
//	   }
	 
	 }
	 
	}

/***********************************************************************************************
*
* @brief    KBI_Interrupt(UINT8 ch). Callback function called by the KBI module on each interrupt
* @param    KBI channel
* @return   none
*
************************************************************************************************/  
void KBI_Interrupt(UINT8 ch)
{
		switch(ch)
		{
		case 0:
			LED0_TOGGLE;
		break;
		case 1:
			LED1_TOGGLE;
		break;
			
		}
}

/***********************************************************************************************
*
* @brief    Enable_Interrupt(UINT8 vector_number). Enable interrupts from desired module.
* @param    Module interrupt number from the interrupts vector table
* @return   none
*
************************************************************************************************/  
void Enable_Interrupt(UINT8 vector_number)
{
	
	vector_number= vector_number-16;

	
	/* Set the ICPR and ISER registers accordingly */
	NVIC_ICPR |= 1 << (vector_number%32);
	NVIC_ISER |= 1 << (vector_number%32);

}
