/*
 *  @file i2c.h
 *  
 *  Copyright (C) 2025 AZ ALL rights reserved.
 */

#ifndef _I2C_H_
#define _I2C_H_

#include "gpio.h"

// Select I2C Speed
typedef enum {
  e50KHz = 0,
  e100KHz,
  e200KHz,
  e300KHz,
  e400KHz,
  e500KHz,
  e600KHz,
  e700KHz,
  e800KHz,
  e900KHz,
  e1MHz
} eI2CSpeed;

void test_i2c_command(void);
void i2c_setup();

extern int i2c_dev_id;
extern eI2CSpeed i2c_speed;

#endif  // _I2C_H_
