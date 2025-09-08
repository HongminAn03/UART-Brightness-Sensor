/*
 *  @file AZ1000.ino
 *  
 *  Copyright (C) 2025 AZ ALL rights reserved.
 */

#include <FS.h>

#include "gpio.h"
#include "utils.h"
#include "serial.h"
#include "i2c.h"
#include "spi_az.h"
#include "aeswire.h"
#include "file.h"

void setup()
{
    //pinMode(PIN_LED, OUTPUT);
    //digitalWrite(PIN_LED, !digitalRead(PIN_LED));

    file_setup();
    file_load();

    serial_setup();
    i2c_setup();
    spi_setup();
    aeswire_setup();

	Serial.println("Device Setup OK\n");
}

void loop()
{
    while(1) {
        test_ic();
        mdelay(1000);
    }
}

void serial_setup()
{
    Serial.begin(115200);
}

static void test_help(void)
{
    Serial.print("\r\n");
    Serial.print("=================================================\r\n");
    Serial.print("                    IC Test\r\n");
    Serial.print("=================================================\r\n");
    Serial.print(" 's' : spi test\r\n");
    Serial.print(" 'i' : i2c test\r\n");
    Serial.print(" 'w' : aswire/eswire test\r\n");
    Serial.print(" '-' : load config\r\n");    
    Serial.print(" '=' : save config\r\n");
    Serial.print(" 'h' : help\r\n");
    Serial.print("=================================================\r\n");
    Serial.print("CMD >");
}

void test_ic(void)
{
    test_help();
    while (1) {
        if (!Serial.available()) {
            continue;
        }

        int c = Serial.read();
        Serial.write(c);
        switch (c) {
        case 's':
        case 'S':
            test_spi_command();
            test_help();
            break;

        case 'i':
        case 'I':
            test_i2c_command();
            test_help();
            break;

        case 'w':
        case 'W':
            test_aeswire_command();
            test_help();
            break;

        case '-':
            file_load();
            test_help();
            break;

        case '=':
            file_save();
            test_help();
            break;

        case 'h':
        case 'H':
            test_help();
            break;

        default:
            Serial.print("Invalid Key\r\n");
            break;
        }
    }

}
