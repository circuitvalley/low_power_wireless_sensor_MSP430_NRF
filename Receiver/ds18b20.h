/* ************************************************************************** */
/** Descriptive File Name

  @Company
 CircuitValley

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */

#ifndef _DS1820_H
#define _DS1820_H

#include <msp430.h>
#include <stdint.h>
/* check configuration of driver */

#define ENABLE_SENSOR_POWER() P2DIR|=BIT3; P2OUT|=BIT3;
#define PIN_ONE_WIRE 		BIT4
#define PORT_ONE_WIRE		P2OUT
#define PORT_ONE_WIRE_IN	P2IN
#define PORT_ONE_DIR		P2DIR
#define setOneWire(b) 		PORT_ONE_WIRE=(b)?(PORT_ONE_WIRE|PIN_ONE_WIRE):PORT_ONE_WIRE&(~PIN_ONE_WIRE)
#define setOneWireDir(b) 	PORT_ONE_DIR=(b)?(PORT_ONE_DIR|PIN_ONE_WIRE):PORT_ONE_DIR& (~PIN_ONE_WIRE)
#define getOneWire  		(((PORT_ONE_WIRE_IN)&PIN_ONE_WIRE)&&0x1)
#define setOneWireREN		P2REN|=PIN_ONE_WIRE
#define TEMP_RES              0x100 /* temperature resolution => 1/256°C = 0.0039°C */

#define LOW 0
#define HIGH 1

#define fake_OC_setOneWire(b) setOneWireDir(!b);
/* -------------------------------------------------------------------------- */
/*                         DS1820 Timing Parameters                           */
/* ------------------------------------------------------------------------- */
#define system_clock 24 //in MHz
#define us_to_ncyles(a)  a*24

#define DS1820_RST_PULSE       us_to_ncyles(480) //480us   /* master reset pulse time in [us] */
#define DS1820_MSTR_BITSTART   us_to_ncyles(2)  //2us     /* delay time for bit start by master */
#define DS1820_PRESENCE_WAIT   us_to_ncyles(40)     //40    /* delay after master reset pulse in [us] */
#define DS1820_PRESENCE_FIN    us_to_ncyles(480) //480   /* dealy after reading of presence pulse [us] */
#define DS1820_BITREAD_DLY     us_to_ncyles(5)     //5     /* bit read delay */
#define DS1820_BITWRITE_DLY    us_to_ncyles(80) //100   /* bit write delay */

#define DS1820_DELAY_750MS 	   us_to_ncyles(750000)

/* -------------------------------------------------------------------------- */
/*                            DS1820 Registers                                */
/* -------------------------------------------------------------------------- */

#define DS1820_REG_TEMPLSB    0
#define DS1820_REG_TEMPMSB    1
#define DS1820_REG_CNTREMAIN  6
#define DS1820_REG_CNTPERSEC  7
#define DS1820_SCRPADMEM_LEN  9     /* length of scratchpad memory */

#define DS1820_ADDR_LEN       8


/* -------------------------------------------------------------------------- */
/*                            DS1820 Commands                                 */
/* -------------------------------------------------------------------------- */

#define DS1820_CMD_SEARCHROM     0xF0
#define DS1820_CMD_READROM       0x33
#define DS1820_CMD_MATCHROM      0x55
#define DS1820_CMD_SKIPROM       0xCC
#define DS1820_CMD_ALARMSEARCH   0xEC
#define DS1820_CMD_CONVERTTEMP   0x44
#define DS1820_CMD_WRITESCRPAD   0x4E
#define DS1820_CMD_READSCRPAD    0xBE
#define DS1820_CMD_COPYSCRPAD    0x48
#define DS1820_CMD_RECALLEE      0xB8


#define DS1820_FAMILY_CODE_DS18B20      0x28
#define DS1820_FAMILY_CODE_DS18S20      0x10

typedef struct DS18B20
{
    uint16_t temp;
    uint8_t ROM[8];
}DS_sensor_t;
extern DS_sensor_t tsensor;

void findDS18B20(DS_sensor_t *sensor);

void readTemp(DS_sensor_t *sensor);
void initOneWire();

/* -------------------------------------------------------------------------- */
/*                            static variables                                */
/* -------------------------------------------------------------------------- */


#endif /* _DS1820_H */

