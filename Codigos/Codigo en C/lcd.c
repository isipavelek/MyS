/*
 * lcd.c
 *
 *  Created on: 12 dic. 2023
 *      Author: ipave
 */

#include <stdint.h>
#include "lcd.h"
#include "sleep.h"

#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "myip2.h"
#include "xil_io.h"
#include "lcd.h"


static void DelayLcd(uint32_t demora);
static void ControlLcd(uint8_t valor);
static void Envia8bitsLcd (uint8_t valor,_Bool tipo);
static void Envia4bitsLcd (uint8_t valor,_Bool tipo);
static void LCD_Write_Byte(uint8_t valor);


static const uint8_t LCD_INIT_CMD[]={
		_4BIT_MODE,DISPLAY_CONTROL,RETURN_HOME,ENTRY_MODE+AUTOINCREMENT,DISPLAY_CONTROL+DISPLAY_ON,CLR_LCD
};

_Bool Init_Lcd(void){

  // if(I2C_HW_init()==LCD_ERROR)return LCD_ERROR;
   DelayLcd(DELAY20ms);
   Envia4bitsLcd(COMANDO_INI1,CONTROL);
   DelayLcd(DELAY10ms);
   Envia4bitsLcd(COMANDO_INI1,CONTROL);
   DelayLcd(DELAY1ms);
   Envia4bitsLcd(COMANDO_INI1,CONTROL);
   Envia4bitsLcd(COMANDO_INI2,CONTROL);
   for(uint8_t i=0;i<sizeof(LCD_INIT_CMD);i++)ControlLcd(LCD_INIT_CMD[i]);
   DelayLcd(DELAY2ms);
   return LCD_OK;

}

/**
  * @brief  DelayLcd demora bloqueante para el uso del LCD
  * @param  valor de la demora a realizar en ms
  * @return void
  * @author Ing. Pavelek Israel
  * @version 1.0
  * @date 11/12/2023
*/


static void DelayLcd(uint32_t demora){
	  usleep(demora*1000);
}

/**
  * @brief  ControlLcd envia una palabra de CONTROL al lcd
  * @param  uint8_t valor de control a enviar
  * @return void
  * @author Ing. Pavelek Israel
  * @version 1.0
  * @date 11/12/2023
*/


static void ControlLcd(uint8_t valor){
	Envia8bitsLcd(valor,CONTROL);
}

/**
  * @brief  DatoLcd envia una palabra de DATOS al lcd
  * @param  uint8_t valor de datos a enviar
  * @return void
  * @author Ing. Pavelek Israel
  * @version 1.0
  * @date 11/12/2023
*/


void DatoLcd (uint8_t dato){
	Envia8bitsLcd(dato,DATOS);
}
/**
  * @brief  DatoAsciiLcd envia una palabra de DATOS al lcd
  * @param  uint8_t valor de datos a enviar previo a su conversion en ASCII
  * @return void
  * @author Ing. Pavelek Israel
  * @version 1.0
  * @date 11/12/2023
*/


void DatoAsciiLcd (uint8_t dato){
	Envia8bitsLcd(dato+'0',DATOS);
}

/**
  * @brief  DatoAsciiLcd envia una palabra de DATOS al lcd
  * @param  uint8_t valor de datos a enviar previo a su conversion en BCD
  * @return void
  * @author Ing. Pavelek Israel
  * @version 1.0
  * @date 11/12/2023
*/

void DatoBCD (uint8_t dato){

	  DatoAsciiLcd((((dato)&0xf0)>>4));
	  DatoAsciiLcd(((dato)&0x0f));

}

/**
  * @brief  DatoAsciiLcd Funci�n que parte el envio de 1 byte en nible alto y bajo para el trabajo en 4 bits
  * @param  uint8_t valor a enviar
  * @param uint8_t tipo CONTROL o DATOS
  * @return void
  * @author Ing. Pavelek Israel
  * @version 1.0
  * @date 11/12/2023
*/

void Envia8bitsLcd (uint8_t valor,_Bool tipo){
	Envia4bitsLcd(valor&HIGH_NIBBLE,tipo); 		//me con quedo y envio los 4 bits m�s significaticos.
	Envia4bitsLcd(valor<<LOW_NIBBLE,tipo);   	//me quedo y envio los 4 bits menos significativos.
}

/**
  * @brief  DatoAsciiLcd Funci�n que parte el envio de 4 bits
  * @param  uint8_t valor a enviar
  * @param uint8_t tipo CONTROL o DATOS
  * @return void
  * @author Ing. Pavelek Israel
  * @version 1.0
  * @date 11/12/2023
*/

static void Envia4bitsLcd (uint8_t valor,_Bool tipo){

	LCD_Write_Byte(valor+tipo+EN+BL);
	DelayLcd(DelayTime);
	LCD_Write_Byte(valor+tipo+BL);
	DelayLcd(DelayTime);
}

/**
  * @brief  SacaTextoLcd Funcion que envia un texto al LCD
  * @param  uint8_t * puntero a un string a enviar
  * @return void
  * @author Ing. Pavelek Israel
  * @version 1.0
  * @date 11/12/2023
*/


void SacaTextoLcd (uint8_t *texto){
	while(*texto)DatoLcd(*texto++);
}

/**
  * @brief  ClrLcd Funcion que borra el LCD
  * @param  void
  * @return void
  * @author Ing. Pavelek Israel
  * @version 1.0
  * @date 11/12/2023
*/

void ClrLcd(void){
   ControlLcd(CLR_LCD);				//	Display Clear.
   DelayLcd(DELAY2ms);

}


/**
  * @brief  PosCaracHLcd Funci�n que posiciona el cursor del LCD en la linea superior.
  * @param  uint8_t un valor de 0 a la longitud de la pantalla
  * @return void
  * @author Ing. Pavelek Israel
  * @version 1.0
  * @date 11/12/2023
*/


void PosCaracHLcd(uint8_t posH){
	ControlLcd(posH | LINEA1);
}

/**
  * @brief  PosCaracLLcd Funci�n que posiciona el cursor del LCD en la linea inferior.
  * @param  uint8_t un valor de 0 a la longitud de la pantalla
  * @return void
  * @author Ing. Pavelek Israel
  * @version 1.0
  * @date 11/12/2023
*/


void PosCaracLLcd(uint8_t posL){
	ControlLcd(posL | LINEA2);
}

/**
  * @brief  CursorOffLcd Funcion que apaga el cursor del LCD
  * @param  void
  * @return void
  * @author Ing. Pavelek Israel
  * @version 1.0
  * @date 11/12/2023
*/

void CursorOffLcd(void){
	ControlLcd(DISPLAY_CONTROL+DISPLAY_ON);
}

/**
  * @brief  CursorOnLcd Funcion que prende el cursor del LCD
  * @param  void
  * @return void
  * @author Ing. Pavelek Israel
  * @version 1.0
  * @date 11/12/2023
*/

void CursorOnLcd(void){
	ControlLcd(DISPLAY_CONTROL+CURSOR_ON+DISPLAY_ON+CURSOR_BLINK);
}


void LCD_Write_Byte(uint8_t valor){
	MYIP2_mWriteReg(XPAR_MYIP2_0_S_AXI_BASEADDR, MYIP2_S_AXI_SLV_REG0_OFFSET, LCD_ADDRESS);
	MYIP2_mWriteReg(XPAR_MYIP2_0_S_AXI_BASEADDR, MYIP2_S_AXI_SLV_REG1_OFFSET, valor);
	//DelayLcd(10);
	MYIP2_mWriteReg(XPAR_MYIP2_0_S_AXI_BASEADDR, MYIP2_S_AXI_SLV_REG2_OFFSET, ENABLE);
	DelayLcd(10);
	MYIP2_mWriteReg(XPAR_MYIP2_0_S_AXI_BASEADDR, MYIP2_S_AXI_SLV_REG2_OFFSET, !ENABLE);
	//DelayLcd(10);

}
