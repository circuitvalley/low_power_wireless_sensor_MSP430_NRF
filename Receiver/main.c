#include <msp430.h> 
#include <stdint.h>
#include "ds18b20.h"
#include "eusci_b_spi.h"
#include "eusci_a_uart.h"
#include "NRF24L01P.h"

/*
 * main.c
 */
#define RAM_START 	0x1F00 //up to 0x1FFF
#define FRAM_START 	0xFE80



#define DATA_PORT P3OUT
#define D5 5 //BIT5



#define OE 			BIT3 	//port2
#define OE_SET		P2OUT|=OE
#define OE_CLEAR	P2OUT&=~OE

//__delay_cycles(24);

static void initClock()
{
   	PM5CTL0=0;
    CSCTL0 =0xA500;
   // FRCTL0 = 0xA500 | NWAITS0; //keine warte weil 8Mhz clk
   CSCTL1 = DCOFSEL_3;  // on reset set to 8MHz
   CSCTL2 = SELA__DCOCLK |  SELS__DCOCLK | SELM__DCOCLK;
   CSCTL3 &= ~(DIVA0 | DIVA1 | DIVA2 | DIVS0 | DIVS1 | DIVS2 | DIVM0| DIVM1 | DIVM2  ); // MCLK and SMCLK to be 8Mhz
   CSCTL3 |= DIVA1|DIVA0;  // ACLK = CLK/8 ==  1Mhz
   // CSCTL4 = HFXTDRIVE0|HFXTDRIVE1 | HFFREQ1 |LFXTOFF  ;  //drive HFXT to be max power ( kein crytal bei stromsucher V1.0 deshab nicht benötigt)
  // _delay_cycles(5000);
  // CSCTL0 =0x0000;
   SFRIFG1 &=~( OFIFG);
}



int main(void) {
	volatile unsigned int i=0;unsigned char TXData;
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer for a while


	 initClock();
P2SEL1 |= BIT0;  //UART TX
P3DIR |=BIT4;
P3SEL1 |=BIT4;
P3SEL0 |=BIT4;
	  initOneWire();
	   RF24L01_IO_set();

	  EUSCI_B_SPI_initMasterParam param = {0};
	     param.selectClockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK;
	     param.clockSourceFrequency = 24000000;
	     param.desiredSpiClock = 8000000;
	     param.msbFirst = EUSCI_B_SPI_MSB_FIRST;
	     param.clockPhase = EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
	     param.clockPolarity = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
	     param.spiMode = EUSCI_B_SPI_3PIN;
	     EUSCI_B_SPI_initMaster(EUSCI_B0_BASE, &param);

	     EUSCI_A_UART_initParam uart_param = {0};
	     uart_param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
	     uart_param.clockPrescalar = 4;
	     uart_param.firstModReg = 5;
	     uart_param.secondModReg = 0x55;
	     uart_param.parity = EUSCI_A_UART_NO_PARITY;
	     uart_param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
	     uart_param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
	     uart_param.uartMode = EUSCI_A_UART_MODE;
	     uart_param.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

	     //Enable SPI module
	     EUSCI_B_SPI_enable(EUSCI_B0_BASE);

	     if(STATUS_FAIL != EUSCI_A_UART_init(EUSCI_A0_BASE, &uart_param))
	     {
	    	  EUSCI_A_UART_enable(EUSCI_A0_BASE);
	     }



	     //Wait for slave to initialize
	     __delay_cycles(100);

	     TXData = 0x1;                             // Holds TX data

	     //USCI_B0 TX buffer ready?

	     //Transmit Data to slave
//	     EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, TXData);
//
//	     __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
//	     __no_operation();                       // Remain in LPM0


	     init_NRF24L01() ;
//	     nRF24L01_TxPacket(TxBuf);	// send the buffer packet
	 		                      SPI_RW_Reg(WRITE_REG+STATUS,0XFF);
	 		          	    TxBuf[1] = 0xff;	//then clear it
	 		          	    TxBuf[2] = 0xff;	//then clear it
	 		          	    tf=0; //send flag is off

	 		                SetRX_Mode();
	 		       	     __delay_cycles(1000);

	 		    	     __delay_cycles(1000);





	  findDS18B20(&tsensor);

	while(1)
    {

		if(!(P2IN&(0x1<<1)))
			{
			 uint8_t string_length=0;

				if(nRF24L01_RxPacket(RxBuf))   //then wait for a packet from the peer
				{
					SetRX_Mode();
				}
				while( RxBuf[string_length]!='\n' && (string_length++)<32 );
				string_length++;

				if(string_length<32)EUSCI_A_UART_transmitBuffer(EUSCI_A0_BASE,RxBuf,string_length);
			}

//readTemp(&tsensor);

    }


}
