/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "myip2.h"
#include "xil_io.h"
#include "sleep.h"
#include "lcd.h"

void reloj(void);
void presentahora(void);
void prueba (void);

int seg,min,hora;

int main(){

	init_platform();
    Init_Lcd();
    ClrLcd();
    PosCaracHLcd(0x06);
    SacaTextoLcd((uint8_t *)"MyS");
	PosCaracLLcd(0x01);
    SacaTextoLcd((uint8_t *)"Pavelek Israel");
    sleep(2);
    ClrLcd();
    while(1){
    	presentahora();
    	reloj();
    	sleep(1);
    	//prueba();
    }

    cleanup_platform();
    return 0;
}


void reloj(void){

	seg++;
	if(seg==60){
		seg=0;
		min++;
		if(min==60){
			min=0;
			hora++;
			if(hora==24)hora=0;
		}
	}

}

void presentahora(void){

	ClrLcd();
	PosCaracHLcd(6);
    SacaTextoLcd((uint8_t *)"Hora");
    PosCaracLLcd(0);
	DatoAsciiLcd(hora/10);
	DatoAsciiLcd(hora%10);
	DatoLcd(':');
	DatoAsciiLcd(min/10);
	DatoAsciiLcd(min%10);
	DatoLcd(':');
	DatoAsciiLcd(seg/10);
	DatoAsciiLcd(seg%10);
}


void prueba (void){
	for(int valor=0;valor<255;valor++){
		MYIP2_mWriteReg(XPAR_MYIP2_0_S_AXI_BASEADDR, MYIP2_S_AXI_SLV_REG0_OFFSET, LCD_ADDRESS);
		MYIP2_mWriteReg(XPAR_MYIP2_0_S_AXI_BASEADDR, MYIP2_S_AXI_SLV_REG1_OFFSET, valor);
		MYIP2_mWriteReg(XPAR_MYIP2_0_S_AXI_BASEADDR, MYIP2_S_AXI_SLV_REG2_OFFSET, ENABLE);
		usleep(10000);
		MYIP2_mWriteReg(XPAR_MYIP2_0_S_AXI_BASEADDR, MYIP2_S_AXI_SLV_REG2_OFFSET, !ENABLE);
		usleep(10000);
	}
}
