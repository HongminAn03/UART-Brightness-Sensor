/*
 *  @file spi_az.h
 *  
 *  Copyright (C) 2025 AZ ALL rights reserved.
 */

#ifndef _SPI_AZ_H_
#define _SPI_AZ_H_

#include "gpio.h"

void test_spi_command(void);
void spi_setup();

extern unsigned char spi_divider;
extern sint8_t spi_bitorder;
extern sint8_t spi_datamode;

#endif  // _SPI_AZ_H_
