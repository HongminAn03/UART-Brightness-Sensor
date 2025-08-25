/*
 *  @file aeswire.h
 *  
 *  Copyright (C) 2025 AZ ALL rights reserved.
 */

#ifndef _AESWIRE_H_
#define _AESWIRE_H_

#include "gpio.h"

void test_aeswire_command(void);
void aeswire_setup();

extern unsigned char aswire_init;
extern unsigned char eswire_init;
extern unsigned char aswire_reset;
extern unsigned char eswire_reset;
extern unsigned int aeswire_clk_width;
extern unsigned int aeswire_init_delay;
extern unsigned char aswire_reg;
extern unsigned char aswire_data;
extern unsigned char eswire_reg;
extern unsigned char eswire_data;

#endif  // _AESWIRE_H_
