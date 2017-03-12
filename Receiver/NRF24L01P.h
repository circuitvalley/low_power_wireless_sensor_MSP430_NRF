/*
 * NRF24L01P.h
 *
 *  Created on: Feb 26, 2017
 *      Author: Gaurav
 */

#ifndef NRF24L01P_H_
#define NRF24L01P_H_

//==========================NRF24L01============================================
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  32  	// 32 TX payload
#define RX_PLOAD_WIDTH  32  	// 32 uints TX payload
//****************************************************************//
// SPI(nRF24L01) commands
#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP1            0xFF  // Define No Operation, might be used to read status register

//***************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address
//=============================RF24l01 init config=====================================
extern char  TX_ADDRESS[TX_ADR_WIDTH];	//TX address
extern char  RX_ADDRESS[RX_ADR_WIDTH];	//RX address
extern char  sta;
extern char  tf, RxBuf[32],TxBuf[32];


#define PORT_MOSI	 PORT1 //bit6
#define PIN_MOSI		BIT6
#define PORT_MISO	PORT1 //bit 7
#define PIN_MISO	BIT7
#define PORT_CLK	PORT2 //bit 2
#define PIN_CLK		BIT2
#define SET_MOSI_PSEL P1SEL1 |=PIN_MOSI ; P1SEL0&=~PIN_MOSI;
#define SET_MISO_PSEL P1SEL1 |=PIN_MISO ; P1SEL0&=~PIN_MISO;
#define SET_CLK_PSEL  P2SEL1 |=PIN_CLK ; P2SEL0&=~PIN_CLK;

#define PORT_CS_DIR	P2DIR
#define PIN_CS		BIT6
#define PORT_CS_OUT P2OUT
#define PORT_CS_DIR P2DIR
#define SET_CS_DIR_OUT	PORT_CS_DIR|=PIN_CS
#define setSPICS(b) 	PORT_CS_OUT=(b)?(PORT_CS_OUT|PIN_CS):(PORT_CS_OUT& (~PIN_CS))

#define PIN_CE			BIT5
#define CE_PORT_OUT		PJOUT
#define CE_PORT_DIR		PJDIR

#define SET_CE_DIR_OUT	CE_PORT_DIR|=PIN_CE
#define NRF_ENABLE(b) CE_PORT_OUT=(b)?(CE_PORT_OUT|PIN_CE):(CE_PORT_OUT& (~PIN_CE))
#define SET_SPI_PSEL() SET_MOSI_PSEL;  SET_MISO_PSEL;  SET_CLK_PSEL;

char nRF24L01_RxPacket(char* rx_buf);
char SPI_RW_Reg(char reg, char value);
void nRF24L01_TxPacket(char * tx_buf);
void SetRX_Mode(void);
void init_NRF24L01(void);




#endif /* NRF24L01P_H_ */
