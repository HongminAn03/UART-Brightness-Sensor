/*
 *  @file file.cpp
 *  
 *  Copyright (C) 2025 AZ ALL rights reserved.
 */

#include <FS.h>
#include "file.h"
#include "i2c.h"
#include "spi_az.h"
#include "aeswire.h"

#define STR_CONFIG_FILE					"/config.txt"

#define I2C_DEV_ID                      "I2CDEVID"
#define I2C_SPEED                       "I2CSPEED"
#define SPI_DIVIDER                     "SPIDIVIER"
#define SPI_BITORDER                    "SPIBITORDER"
#define SPI_DATAMODE                    "SPIDATAMODE"
#define ASWIRE_INIT                     "ASWIREINIT"
#define ESWIRE_INIT                     "ESWIREINIT"
#define ASIWRE_RESET                    "ASWIRERESET"
#define ESIWRE_RESET                    "ESWIRERESET"
#define AESWIRE_CLK_DELAY               "AESWIRECLKDEALY"
#define AESWIRE_INIT_DELAY              "AESWIREINITDEALY"

#define ASWIRE_REG                      "ASWIREREG"
#define ASWIRE_DATA                     "ASWIREDATA"
#define ESWIRE_REG                      "ESWIREREG"
#define ESWIRE_DATA                     "ESWIREDATA"


bool file_load(void)
{
    char *c_ptr, *end_ptr;
    char buf[256] = { 0, };
    int value;

    if (!SPIFFS.exists(STR_CONFIG_FILE)) {
        Serial.println("Config file does not exist");
        return false;
    }

    File file = SPIFFS.open(STR_CONFIG_FILE, "r");
    if (!file) {
        Serial.println("Failed to open config file");
        return false;
    }

    while(1) {
        String line = file.readStringUntil('\n');
        if (line.length() == 0) {
            break;
        }

        sprintf(buf, "%s", line.c_str());

        c_ptr = strtok(buf, ", \r\n");
        if (!strncmp(c_ptr, I2C_DEV_ID, strlen(I2C_DEV_ID))) {
            c_ptr = strtok(NULL, ", \r\n");
            sscanf(c_ptr, "%d", &value);
            i2c_dev_id = value & 0xFF;
        }
        else if (!strncmp(c_ptr, I2C_SPEED, strlen(I2C_SPEED))) {
            c_ptr = strtok(NULL, ", \r\n");
            sscanf(c_ptr, "%d", &value);
            i2c_speed = (eI2CSpeed)value;
        }        
        else if (!strncmp(c_ptr, SPI_DIVIDER, strlen(SPI_DIVIDER))) {
            c_ptr = strtok(NULL, ", \r\n");
            sscanf(c_ptr, "%d", &value);
            spi_divider = value & 0xFF;
        }
        else if (!strncmp(c_ptr, SPI_BITORDER, strlen(SPI_BITORDER))) {
            c_ptr = strtok(NULL, ", \r\n");
            sscanf(c_ptr, "%d", &value);
            spi_bitorder = value & 0xFF;
        }
        else if (!strncmp(c_ptr, SPI_DATAMODE, strlen(SPI_DATAMODE))) {
            c_ptr = strtok(NULL, ", \r\n");
            sscanf(c_ptr, "%d", &value);
            spi_datamode = value & 0xFF;
        }        
        else if (!strncmp(c_ptr, ASWIRE_INIT, strlen(ASWIRE_INIT))) {
            c_ptr = strtok(NULL, ", \r\n");
            sscanf(c_ptr, "%d", &value);
            aswire_init = value & 0xFF;
        }
        else if (!strncmp(c_ptr, ESWIRE_INIT, strlen(ESWIRE_INIT))) {
            c_ptr = strtok(NULL, ", \r\n");
            sscanf(c_ptr, "%d", &value);
            eswire_init = value & 0xFF;
        }
        else if (!strncmp(c_ptr, ASIWRE_RESET, strlen(ASIWRE_RESET))) {
            c_ptr = strtok(NULL, ", \r\n");
            sscanf(c_ptr, "%d", &value);
            aswire_reset = value & 0xFF;
        }
        else if (!strncmp(c_ptr, ESIWRE_RESET, strlen(ESIWRE_RESET))) {
            c_ptr = strtok(NULL, ", \r\n");
            sscanf(c_ptr, "%d", &value);
            eswire_reset = value & 0xFF;
        }
        else if (!strncmp(c_ptr, AESWIRE_CLK_DELAY, strlen(AESWIRE_CLK_DELAY))) {
            c_ptr = strtok(NULL, ", \r\n");
            sscanf(c_ptr, "%d", &value);
            aeswire_clk_width = value;
        }
        else if (!strncmp(c_ptr, AESWIRE_INIT_DELAY, strlen(AESWIRE_INIT_DELAY))) {
            c_ptr = strtok(NULL, ", \r\n");
            sscanf(c_ptr, "%d", &value);
            aeswire_init_delay = value;
        }
        else if (!strncmp(c_ptr, ASWIRE_REG, strlen(ASWIRE_REG))) {
            c_ptr = strtok(NULL, ", \r\n");
            sscanf(c_ptr, "%d", &value);
            aswire_reg = value;
        }
        else if (!strncmp(c_ptr, ASWIRE_DATA, strlen(ASWIRE_DATA))) {
            c_ptr = strtok(NULL, ", \r\n");
            sscanf(c_ptr, "%d", &value);
            aswire_data = value;
        }
        else if (!strncmp(c_ptr, ESWIRE_REG, strlen(ESWIRE_REG))) {
            c_ptr = strtok(NULL, ", \r\n");
            sscanf(c_ptr, "%d", &value);
            eswire_reg = value;
        }
        else if (!strncmp(c_ptr, ESWIRE_DATA, strlen(ESWIRE_DATA))) {
            c_ptr = strtok(NULL, ", \r\n");
            sscanf(c_ptr, "%d", &value);
            eswire_data = value;
        }
    }

    file.close();

    Serial.println("Load config file : Okay");

    return true;
}

bool file_save(void)
{
    char buf[256] = { 0, };

    File file = SPIFFS.open(STR_CONFIG_FILE, "w");
    if (!file) {
        Serial.println("Failed to open config file for waiting");
        return false;
    }

    sprintf(buf, "%s, %d", I2C_DEV_ID, i2c_dev_id);
    file.println(buf);
    sprintf(buf, "%s, %d", I2C_SPEED, i2c_speed);
    file.println(buf);

    sprintf(buf, "%s, %d", SPI_DIVIDER, spi_divider);
    file.println(buf);
    sprintf(buf, "%s, %d", SPI_BITORDER, spi_bitorder);
    file.println(buf);
    sprintf(buf, "%s, %d", SPI_DATAMODE, spi_datamode);
    file.println(buf);

    sprintf(buf, "%s, %d", ASWIRE_INIT, aswire_init);
    file.println(buf);
    sprintf(buf, "%s, %d", ESWIRE_INIT, eswire_init);
    file.println(buf);
    sprintf(buf, "%s, %d", ASIWRE_RESET, aswire_reset);
    file.println(buf);
    sprintf(buf, "%s, %d", ESIWRE_RESET, eswire_reset);
    file.println(buf);
    sprintf(buf, "%s, %d", AESWIRE_CLK_DELAY, aeswire_clk_width);
    file.println(buf);
    sprintf(buf, "%s, %d", AESWIRE_INIT_DELAY, aeswire_init_delay);
    file.println(buf);
    sprintf(buf, "%s, %d", ASWIRE_REG, aswire_reg);
    file.println(buf);
    sprintf(buf, "%s, %d", ASWIRE_DATA, aswire_data);
    file.println(buf);
    sprintf(buf, "%s, %d", ESWIRE_REG, eswire_reg);
    file.println(buf);
    sprintf(buf, "%s, %d", ESWIRE_DATA, eswire_data);
    file.println(buf);

    file.close();
//    SPIFFS.end();

    Serial.println("Save config file : Okay");

    return true;
}

void file_setup(void)
{
    SPIFFS.begin();
}