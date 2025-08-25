/*
 *  @file utils.cpp
 *  
 *  Copyright (C) 2025 AZ ALL rights reserved.
 */

#include <arduino.h>
#include "utils.h"

void mdelay(unsigned int msec)
{
    delay(msec);
}

void udelay(unsigned int usec)
{
#if 1
    delayMicroseconds(usec);
#else   // 1.009us
    for (int i = 0; i < 120; i++) {
        __asm__ __volatile__ ("nop\n\t");
    } // 10(2.2us), 5(1.174us), 4(1.01us), 3(789us)
#endif
}