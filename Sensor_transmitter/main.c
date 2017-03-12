#include <msp430.h> 
#include <stdint.h>
#include "ds18b20.h"
#include "eusci_b_spi.h"
#include "NRF24L01P.h"
/*
 * File:   main.c
 * Author:  Gaurav
 * website: www.circuitvalley.com
 * Created on March 14, 2017, 9:42 PM
 *	This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *	Email: gauravsingh@circuitvalley.com
************************************************************************/



// measured Power consuption
// ~1.7 mA for 600ms during Temp Sampling,
// ~1.0 mA for 140ms during NRF SPI and Setup
// ~0.8 mA for 40ms
// ~2.0 mA average for 20ms time around actual RF transmission though peak goes around max 9ma for short period of time ~8ms,
static void initClock()
{
	PM5CTL0=0;
	CSCTL0 =0xA500;
	// FRCTL0 = 0xA500 | NWAITS0; //keine warte weil 8Mhz clk
	CSCTL1 = DCOFSEL_3;  // on reset set to 8MHz
	CSCTL2 = SELA__VLOCLK |  SELS__DCOCLK | SELM__DCOCLK;
	CSCTL3 &= ~(DIVA0 | DIVA1 | DIVA2 | DIVS0 | DIVS1 | DIVS2 | DIVM0| DIVM1 | DIVM2  ); // MCLK and SMCLK to be 8Mhz
	//CSCTL3 |= DIVA1|DIVA0;  // ACLK = CLK/8 ==  1Mhz
	// CSCTL4 = HFXTDRIVE0|HFXTDRIVE1 | HFFREQ1 |LFXTOFF  ;  //drive HFXT to be max power ( kein crytal bei stromsucher V1.0 deshab nicht benötigt)
	// _delay_cycles(5000);
	// CSCTL0 =0x0000;
	SFRIFG1 &=~( OFIFG);
}


void initSamplingTIMERB1()
{
	TB1CTL = 0;
	TB1CCR0 =60000; //VLO is minimum 5 khz , VLO /5 is minimum 1khz , at 1khz 60000 for 60 seconds

	TB1EX0 = TBIDEX_4; // div by 5
	TB1CTL = TASSEL__ACLK | ID_0 | MC_1  |TBIE|TBCLR;
	TB1R=58000;
}



#pragma vector = TIMER1_B1_VECTOR		//timer B1 over flor make it trig ever 60 second , take sample and tramit over rf
__interrupt void samplingTIMERB1_ISR(void)
{
	if(TB1CTL&TBIFG)
	{	uint16_t integer_temp;
		uint8_t sign,fraction_temp;
		ENABLE_SENSOR_POWER();
		readTemp(&tsensor);
		DISABLE_SENSOR_POWER();
		integer_temp=tsensor.temp>>4;
		fraction_temp=tsensor.temp&0xf;
		sign='+';
		if(integer_temp&0x800)
		{
			integer_temp=(~integer_temp)&0xFF ; //number is "extended" 2's complement
			sign='-';
			fraction_temp= (~fraction_temp)&0xf;


		}
		fraction_temp=(fraction_temp*625)/1000;
		TxBuf[1]=(integer_temp/100)+0x30;
		if(TxBuf[1]=='0')
				{
					TxBuf[0]=' ';
					TxBuf[1]=sign;
				}else
				{
					TxBuf[0]=sign;
				}
				TxBuf[2]= ((integer_temp/10)%10) +0x30 ;
				if(TxBuf[2]=='0' && TxBuf[1]==sign)
				{
						TxBuf[1]=' ';
						TxBuf[2]=sign;
				}

				TxBuf[3]= (integer_temp%10)+0x30;

		TxBuf[4]='.';
		TxBuf[5]=fraction_temp+0x30;
		TxBuf[6]='\r';
		TxBuf[7]='\n';

		nRF24L01_TxPacket(TxBuf);	// send the buffer packet
		SPI_RW_Reg(WRITE_REG+STATUS,0XFF);
	}
	TB1CTL &= ~TBIFG;
}


int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer for a while
	initClock();
	initSamplingTIMERB1();
	initOneWire();
	findDS18B20(&tsensor);
	initNRF24L01pIO();
	initNRF24L01p() ;
	__enable_interrupt();
	_BIS_SR(LPM3_bits + GIE);
	__no_operation();
}
