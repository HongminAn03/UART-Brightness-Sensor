/*
 *  @file gpio.h
 *  
 *  Copyright (C) 2025 AZ ALL rights reserved.
 */

#ifndef _GPIO_H_
#define _GPIO_H_

const int PIN_TX  		= 1;	// TX 	GPIO1 	UART-TX		IO1
const int PIN_RX  		= 3;	// RX 	GPIO3 	UART-RX		IO3
//const int PIN_ESWIRE 	= 16;	// D0	GPIO16	ESWIRE 		IO16
const int PIN_SCL 		= 5;	// D1	GPIO5	I2C-SCL		IO5
const int PIN_SDA 		= 4;	// D2	GPIO4	I2C-SDA		IO4
const int PIN_ASWIRE 	= 0;	// D3	GPIO0	ASWIRE 		IO0
const int PIN_ESWIRE 	= 2;	// D4	GPIO2	ESWIRE 		IO2 		(LED)
const int PIN_SCK 		= 14;	// D5 	GPIO14 	SPI-SCK		IO14
const int PIN_MISO 		= 12;	// D6 	GPIO12 	SPI-MISO	IO12
const int PIN_MOSI 		= 13;	// D7 	GPIO13 	SPI-MOSI	IO13
const int PIN_SS 		= 15;	// D8 	GPIO15 	SPI-CS(SS)	IO15

#endif	// _GPIO_H_
