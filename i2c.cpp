/*
 *  @file i2c.cpp
 *  
 *  Copyright (C) 2025 AZ ALL rights reserved.
 */

#include <arduino.h>
#include <cctype>
#include <Wire.h>
#include "i2c.h"
#include "cmd.h"
#include "utils.h"

static cmd_line_t i2c_saved_cmdline;
int i2c_dev_id = 0x44;
eI2CSpeed i2c_speed = e50KHz;

void i2c_command_help(void);
void i2c_change_clock(void);
void i2c_set_clock(eI2CSpeed clk);

bool i2c_write(unsigned char reg, unsigned char data)
{
    byte ret;
    char buf[256] = { 0, };

    Wire.beginTransmission(i2c_dev_id);
    Wire.write(reg);
    Wire.write(data);
    ret = Wire.endTransmission(i2c_dev_id);
    if (ret != 0) {
        sprintf(buf, "i2c_write [Reg:%02x, Data:0x%02x] Fail\r\n", reg, data);
        Serial.print(buf);
        return false;
    }

    return true;
}

unsigned char i2c_read(unsigned char reg)
{
    byte ret;
    unsigned char count = 0xFF;
    char buf[256] = { 0, };

    Wire.beginTransmission(i2c_dev_id);
    Wire.write(reg);
    ret = Wire.endTransmission(i2c_dev_id);
    if (ret != 0) {
        sprintf(buf, "i2c_read [Reg:%02x] Fail\r\n", reg);
        Serial.print(buf);
        return 0;
    }

    udelay(5);

    Wire.requestFrom(i2c_dev_id, 1);
    while(count--) {
        if (!Wire.available()) {
            udelay(10);
            continue;
        }

        return Wire.read();
    }

    sprintf(buf, "i2c_read [Reg:%02x] Fail\r\n", reg);   
    Serial.print(buf);
    return 0; 
}

unsigned char i2c_scan_devid(void)
{
    byte err;
    unsigned char devid = 0;
    char buf[256] = { 0, };

    for (unsigned char id = 1; id < 127; id++) {
        Wire.beginTransmission(id);
        err = Wire.endTransmission();

        if (err == 0) {
            if (id < 16) {
                id = 0;
            }
            sprintf(buf, "I2C device found at devid 0x%02x\r\n", id);
            Serial.print(buf);
            devid = id;
            break;
        }
        else if (err == 4) {
            if (id < 16) {
                id = 0;
            }
            sprintf(buf, "Unknown error at devid 0x%02x\r\n", id);
            Serial.print(buf);
        }
    }

    if (devid == 0) {
        Serial.println("No I2C devices found\r\n");
    }
    else {
        Serial.println("Done.\r\n");
    }

    return devid;
}

void i2c_process_cmd_line(cmd_line_t *cmd)
{
    int i;
    unsigned char value;
    char buf[256] = { 0, };
    unsigned char reg, data;

    reg = cmd->reg & 0xFF;
    data = cmd->data & 0xFF;

    switch (cmd->type) {
    case 's':
    case 'S':
        i2c_change_clock();
        i2c_command_help();
        break;

    case 'c':
    case 'C':
        i2c_dev_id = reg;
        sprintf(buf, " [Change] device id : 0x%02x\r\n", i2c_dev_id);
        Serial.print(buf);
        break;

    case 'w':
    case 'W':
        value = 0;
        sprintf(buf, " [write] addr : 0x%02x, data : 0x%02x", reg, data);
        Serial.print(buf);
        if (i2c_write(reg, data)) {
            Serial.print(" ->Okay\r\n");
        }
        else {
            Serial.print(" ->Fail\r\n");
        }

        i2c_saved_cmdline = *cmd;
        break;

    case 'r':
    case 'R':
        value = 0;
        sprintf(buf, " [Read] addr : 0x%02x,", reg);
        Serial.print(buf);
        value = i2c_read(reg);
        sprintf(buf, " -> data : 0x%02x\r\n", value);
        Serial.print(buf);

        i2c_saved_cmdline = *cmd;
        break;

    case 'd':
    case 'D':
        sprintf(buf, " [Dump] from : 0x%02x, to : 0x%02x\r\n", reg, data);
        Serial.print(buf);
        for (i = reg; i <= data; i++) {
            sprintf(buf, " [Read] addr : 0x%02x,", i);
            Serial.print(buf);
            value = i2c_read(i);
            sprintf(buf, " -> data : 0x%02x\r\n", value);
            Serial.print(buf);            
        }

        i2c_saved_cmdline = *cmd;
        break;

    case 'f':
    case 'F':
        i2c_dev_id = i2c_scan_devid();
        break;

    case 'h':
    case 'H':
        i2c_command_help();
        break;

    default:
        Serial.print("Invalid cmd\r\n");
        break;
    }
}

void i2c_command_help(void)
{
    char buf[256] = { 0, };

    Serial.print("\r\n");
    Serial.print("=================================================\r\n");
    Serial.print("                    I2C Command Mode\r\n");
    Serial.print("=================================================\r\n");
    Serial.print(" s          : change i2c speed\r\n");
    sprintf(buf, " c devid    : change device id            ex) c %02x\r\n", i2c_dev_id); Serial.print(buf);
    Serial.print(" w reg data : write data                  ex) w 01 AE\r\n");
    Serial.print(" r reg      : read data                   ex) r 01\r\n");
    Serial.print(" d reg1 reg2: read data from reg1 to reg2 ex) d 01 12\r\n");
    Serial.print(" Enter      : redo command(w/r/d)\r\n");
    Serial.print(" f          : scanning i2c device id\r\n");
    Serial.print(" h          : print this menu\r\n");
    Serial.print(" q          : exit\r\n");
    Serial.print("-------------------------------------------------\r\n");
    if (i2c_speed == e50KHz) {
        Serial.print(" i2c speed : 50KHz\r\n");
    }
    else if (i2c_speed == e1MHz) {
        Serial.print(" i2c speed : 1MHz\r\n");
    }
    else {
        sprintf(buf, " i2c speed : %dKHz\r\n", i2c_speed * 100); Serial.print(buf);
    }
    sprintf(buf, " device id : 0x%02x\r\n", i2c_dev_id); Serial.print(buf);
    Serial.print(" reg(8bit)\r\n");
    Serial.print(" data(8bit)\r\n");
    Serial.print("=================================================\r\n");
}

void i2c_change_clock_help(void)
{
    char buf[256] = { 0, };

    Serial.print("\r\n");
    Serial.print("=================================================\r\n");
    Serial.print("                    Change I2C Clock  \r\n");
    Serial.print("=================================================\r\n");
    sprintf(buf, " '0' :  50KHz %s\r\n", (i2c_speed ==  e50KHz) ? "*": ""); Serial.print(buf);
    sprintf(buf, " '1' : 100KHz %s\r\n", (i2c_speed == e100KHz) ? "*": ""); Serial.print(buf);
    sprintf(buf, " '2' : 200KHz %s\r\n", (i2c_speed == e200KHz) ? "*": ""); Serial.print(buf);
    sprintf(buf, " '3' : 300KHz %s\r\n", (i2c_speed == e300KHz) ? "*": ""); Serial.print(buf);
    sprintf(buf, " '4' : 400KHz %s\r\n", (i2c_speed == e400KHz) ? "*": ""); Serial.print(buf);
    sprintf(buf, " '5' : 500KHz %s\r\n", (i2c_speed == e500KHz) ? "*": ""); Serial.print(buf);
    sprintf(buf, " '6' : 600KHz %s\r\n", (i2c_speed == e600KHz) ? "*": ""); Serial.print(buf);
    sprintf(buf, " '7' : 700KHz %s\r\n", (i2c_speed == e700KHz) ? "*": ""); Serial.print(buf);
    sprintf(buf, " '8' : 800KHz %s\r\n", (i2c_speed == e800KHz) ? "*": ""); Serial.print(buf);
    sprintf(buf, " '9' : 900KHz %s\r\n", (i2c_speed == e900KHz) ? "*": ""); Serial.print(buf);
    sprintf(buf, " 'a' :   1MHz %s\r\n", (i2c_speed ==   e1MHz) ? "*": ""); Serial.print(buf);
    Serial.print(" 'h' : print this menu\r\n");
    Serial.print(" 'q' : exit\r\n");
    Serial.print("=================================================\r\n");
}

void i2c_change_clock(void)
{
    eI2CSpeed clk;

    i2c_change_clock_help();
    while (1) {
        if (!Serial.available()) {
            continue;
        }
        int c = Serial.read();
        if ((c >= '0') && (c <= '9')) {
            clk = (eI2CSpeed)(c - '0');
        }
        else if ((c == 'a') || (c == 'A')) {
            clk = e1MHz;
        }
        else if ((c == 'h') || (c == 'H')) {
            i2c_change_clock_help();
            continue;
        }
        else if ((c == 'q') || (c == 'Q')) {
            return;
        }
        else {
            continue;
        }

        if (clk < 0 || clk > e1MHz) {
            continue;
        }

        i2c_speed = clk;
        i2c_set_clock(i2c_speed);
        i2c_change_clock_help();
    }
}

void i2c_set_clock(eI2CSpeed clk)
{
    char buf[256] = { 0, };

    Wire.setClock((clk == e50KHz) ? 50000 : (clk * 100000));
    if (clk == e50KHz) {
        Serial.print("set 50KHz Okay\r\n");
    }
    else if (clk == e1MHz) {
        Serial.print("set 1MHz Okay\r\n");
    }
    else {
        sprintf(buf, "set %dKHz Okay\r\n", clk * 100); Serial.print(buf);
    }
}

void test_i2c_command(void)
{
    cmd_line_t cmd;
    i2c_set_clock(i2c_speed);
    i2c_command_help();
    while (1) {
        Serial.print("I2C>");
        if (get_cmd_line(&cmd, i2c_saved_cmdline) == true) {
            i2c_process_cmd_line(&cmd);
        }
        else {
            break;
        }
    }
}

void i2c_setup()
{
    Wire.begin(PIN_SDA, PIN_SCL);
}
