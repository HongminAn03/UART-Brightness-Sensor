/*
 *  @file spi_az.cpp
 *  
 *  Copyright (C) 2025 AZ ALL rights reserved.
 */

#include <arduino.h>
#include <cctype>
#include <SPI.h>
#include "spi_az.h"
#include "cmd.h"

static cmd_line_t   spi_saved_cmdline;
unsigned char spi_divider = 6;
// SPISettings(clock speed, bit order, data mode)
// clock speeds: Maximum spi speed
// Bit order: MSBFIRST (Most Significant Bit First) or LSBFIRST
// Data mode: SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3
//     SPI_MODE0 : CPOL = 0, CPHA = 0
//     SPI_MODE1 : CPOL = 0, CPHA = 1
//     SPI_MODE2 : CPOL = 1, CPHA = 0
//     SPI_MODE3 : CPOL = 1, CPHA = 1
sint8_t spi_bitorder = MSBFIRST;
sint8_t spi_datamode = SPI_MODE0;

void spi_command_help(void);
void spi_change_clock(void);
unsigned int spi_divider_to_clk(unsigned char div);

bool spi_write(unsigned char reg, unsigned char *data, unsigned int length)
{
    unsigned char txbuf[length + 2];

    txbuf[0] = reg;
    memcpy(&txbuf[1], data, length);

    SPI.beginTransaction(SPISettings(spi_divider_to_clk(spi_divider),
        spi_bitorder, spi_datamode));

    digitalWrite(PIN_SS, LOW);

    SPI.transfer(txbuf, length + 1);

    digitalWrite(PIN_SS, HIGH);

    SPI.endTransaction();

    return true;
}

unsigned char spi_read(unsigned char reg)
{
    unsigned char data;

    SPI.beginTransaction(SPISettings(spi_divider_to_clk(spi_divider),
        spi_bitorder, spi_datamode));

    digitalWrite(PIN_SS, LOW);

    data = SPI.transfer(reg);

    digitalWrite(PIN_SS, HIGH);

    SPI.endTransaction();

    return data; 
}

void spi_process_cmd_line(cmd_line_t *cmd)
{
    int i;
    char buf[256] = { 0, };
    unsigned char reg, data, value;

    reg = cmd->reg & 0xFF;
    data = cmd->data & 0xFF;

    switch (cmd->type) {
    case 's':
    case 'S':
        spi_change_clock();
        spi_command_help();
        break;

    case 'w':
    case 'W':
        value = 0;
        sprintf(buf, " [write] addr : 0x%02x, data : 0x%02x", reg, data);
        Serial.print(buf);
        spi_write(reg, &data, 1);
        sprintf(buf, " ->Okay\r\n");
        Serial.print(buf);
        spi_saved_cmdline = *cmd;
        break;

    case 'r':
    case 'R':
        value = 0;
        sprintf(buf, " [Read] addr : 0x%02x,", reg);
        Serial.print(buf);
        value = spi_read(reg);
        sprintf(buf, " -> data : 0x%02x\r\n", value);
        Serial.print(buf);
        spi_saved_cmdline = *cmd;
        break;

    case 'd':
    case 'D':
        sprintf(buf, " [Dump] from : 0x%02x, to : 0x%02x\r\n", reg, data);
        Serial.print(buf);
        for (i = reg; i <= data; i++) {
            sprintf(buf, " [Read] addr : 0x%02x,", i);
            Serial.print(buf);
            value = spi_read(i);
            sprintf(buf, " -> data : 0x%02x\r\n", value);
            Serial.print(buf);            
        }
        spi_saved_cmdline = *cmd;
        break;

    case 'h':
    case 'H':
        spi_command_help();
        break;

    default:
        Serial.print("Invalid cmd\r\n");
        break;
    }
}

void spi_command_help(void)
{
    char buf[256] = { 0, };
    unsigned int clk = spi_divider_to_clk(spi_divider);

    Serial.print("\r\n");
    Serial.print("=================================================\r\n");
    Serial.print("                    SPI Command Mode\r\n");
    Serial.print("=================================================\r\n");
    Serial.print(" s          : change spi speed\r\n");
    Serial.print(" w reg data : write data                  ex) w 01 AE\r\n");
    Serial.print(" r reg      : read data                   ex) r 01\r\n");
    Serial.print(" d reg1 reg2: read data from reg1 to reg2 ex) d 01 12\r\n");
    Serial.print(" Enter      : redo command(w/r/d)\r\n");
    Serial.print(" h          : print this menu\r\n");
    Serial.print(" q          : exit\r\n");
    Serial.print("-------------------------------------------------\r\n");
    if ((clk / 1000000) == 0) { // KHz
        sprintf(buf, " spi speed : %dKHz\r\n", clk / 1000);
    }
    else { // MHz
        if ((clk % 1000000) > 0) {
            sprintf(buf, " spi speed : %.02fMHz\r\n", (float)clk / (float)1000000);
        }
        else {
            sprintf(buf, " spi speed : %dMHz\r\n", clk / 1000000);
        }
    }
    Serial.print(buf);
    Serial.print(" reg(8bit)\r\n");
    Serial.print(" data(8bit)\r\n");
    Serial.print("=================================================\r\n");
}

void spi_change_clock_help(void)
{
    char buf[256] = { 0, };
    unsigned int sclk = ESP.getCpuFreqMHz(); // MHz
    unsigned int clk = spi_divider_to_clk(spi_divider);

    Serial.print("\r\n");
    Serial.print("=================================================\r\n");
    sprintf(buf, "                    Change SPI Clock (Sclk %dMHz)\r\n", sclk); Serial.print(buf);
    Serial.print("=================================================\r\n");
    for (int i = 1; i < 9; i++) {
        if ((sclk / (1 << i)) == 0) { // KHz
            sprintf(buf, " \'%d\' : %dKHz %s\r\n", i, (sclk * 1000) / (1 << i), (spi_divider == i) ? "*": "");
        }
        else { // MHz
            if ((sclk % (1 << i)) > 0) {
                sprintf(buf, " \'%d\' : %.02fMHz %s\r\n", i, (float)sclk / (float)(1 << i), (spi_divider == i) ? "*": "");
            }
            else {
                sprintf(buf, " \'%d\' : %dMHz %s\r\n", i, sclk / (1 << i), (spi_divider == i) ? "*": "");
            }
        }
        Serial.print(buf);
    }
    Serial.print(" 'h' : print this menu\r\n");
    Serial.print(" 'q' : exit\r\n");
    Serial.print("=================================================\r\n");
}

void spi_change_clock(void)
{    
    unsigned int div;

    spi_change_clock_help();
    while (1) {
        if (!Serial.available()) {
            continue;
        }
        int c = Serial.read();
        if ((c >= '1') && (c <= '8')) {
            div = c - '0';
        }
        else if ((c == 'h') || (c == 'H')) {
            spi_change_clock_help();
            continue;
        }
        else if ((c == 'q') || (c == 'Q')) {
            return;
        }
        else {
            continue;
        }

        if (div < 1) {
            continue;
        }

        spi_divider = div;
        spi_change_clock_help();
    }
}

unsigned int spi_divider_to_clk(unsigned char div)
{
    unsigned int sclk = ESP.getCpuFreqMHz(); // MHz

    return (sclk * 1000000) / (1 << div);
}

void test_spi_command(void)
{
    cmd_line_t cmd;
    spi_command_help();
    while (1) {
        Serial.print("SPI>");
        if (get_cmd_line(&cmd, spi_saved_cmdline) == true) {
            spi_process_cmd_line(&cmd);
        }
        else {
            break;
        }
    }
}

void spi_setup()
{
    pinMode(PIN_SS, OUTPUT);
    digitalWrite(PIN_SS, HIGH);

    SPI.begin();
    SPI.setBitOrder(spi_bitorder);
    SPI.setDataMode(spi_datamode);
}
