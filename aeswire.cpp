/*
 *  @file aeswire.cpp
 *  
 *  Copyright (C) 2025 AZ ALL rights reserved.
 */

#include <Arduino.h>
#include <cctype>
#include "aeswire.h"
#include "cmd.h"
#include "utils.h"

#define GPIO_OUT_HIGH                   (0x60000304)
#define GPIO_OUT_LOW                    (0x60000308)

static cmd_line_t   aeswire_saved_cmdline;
unsigned char aswire_init = 0;
unsigned char eswire_init = 0;
unsigned char aswire_reset = 0;
unsigned char eswire_reset = 0;
unsigned int aeswire_clk_width = 10;
unsigned int aeswire_init_delay = 10;

unsigned char aswire_reg = 0;
unsigned char aswire_data = 0;
unsigned char eswire_reg = 0;
unsigned char eswire_data = 0;

void aeswire_command_help(void);

void aswire_send(unsigned char e)
{
    unsigned char i = 0;

    os_intr_lock();
    while (i < e) {
        WRITE_PERI_REG(GPIO_OUT_LOW, (1 << PIN_ASWIRE));
        udelay(aeswire_clk_width);
        WRITE_PERI_REG(GPIO_OUT_HIGH, (1 << PIN_ASWIRE));
        udelay(aeswire_clk_width);
        i++;
    }
    os_intr_unlock();
}

void eswire_send(unsigned char e)
{
    unsigned char i = 0;

    os_intr_lock();
    while (i < e) {
        WRITE_PERI_REG(GPIO_OUT_LOW, (1 << PIN_ESWIRE));
        udelay(aeswire_clk_width);
        WRITE_PERI_REG(GPIO_OUT_HIGH, (1 << PIN_ESWIRE));
        udelay(aeswire_clk_width);
        i++;
    }
    os_intr_unlock();
}

void aswire_write(unsigned char reg, unsigned char data)
{
    if (aswire_init) {
        WRITE_PERI_REG(GPIO_OUT_HIGH, (1 << PIN_ASWIRE));
    }
    else {
        WRITE_PERI_REG(GPIO_OUT_LOW, (1 << PIN_ASWIRE));
    }

    udelay(aeswire_init_delay);

    if (eswire_init) {
        WRITE_PERI_REG(GPIO_OUT_HIGH, (1 << PIN_ESWIRE));
    }
    else {
        WRITE_PERI_REG(GPIO_OUT_LOW, (1 << PIN_ESWIRE));
    }
    udelay(400);

    if (reg != 0) {
        aswire_send(reg);
        udelay(250);
    }
    if (data != 0) {
        aswire_send(data);
        udelay(250);
    }

    if (aswire_reset) {
        WRITE_PERI_REG(GPIO_OUT_HIGH, (1 << PIN_ASWIRE));
    }
    else {
        WRITE_PERI_REG(GPIO_OUT_LOW, (1 << PIN_ASWIRE));
    }

    udelay(aeswire_init_delay);

    if (eswire_reset) {
        WRITE_PERI_REG(GPIO_OUT_HIGH, (1 << PIN_ESWIRE));
    }
    else {
        WRITE_PERI_REG(GPIO_OUT_LOW, (1 << PIN_ESWIRE));
    }
}

void aeswire_write(unsigned char areg, unsigned char adata, unsigned char ereg, unsigned char edata)
{
    if (aswire_init) {
        WRITE_PERI_REG(GPIO_OUT_HIGH, (1 << PIN_ASWIRE));
    }
    else {
        WRITE_PERI_REG(GPIO_OUT_LOW, (1 << PIN_ASWIRE));
    }

    udelay(aeswire_init_delay);

    if (eswire_init) {
        WRITE_PERI_REG(GPIO_OUT_HIGH, (1 << PIN_ESWIRE));
    }
    else {
        WRITE_PERI_REG(GPIO_OUT_LOW, (1 << PIN_ESWIRE));
    }
    udelay(400);

    if (areg != 0) {
        aswire_send(areg);
        udelay(250);
    }
    if (adata != 0) {
        aswire_send(adata);
        udelay(250);
    }

    mdelay(10);

    if (ereg != 0) {
        eswire_send(ereg);
        udelay(250);
    }
    if (edata != 0) {
        eswire_send(edata);
        udelay(250);
    }

    if (aswire_reset) {
        WRITE_PERI_REG(GPIO_OUT_HIGH, (1 << PIN_ASWIRE));
    }
    else {
        WRITE_PERI_REG(GPIO_OUT_LOW, (1 << PIN_ASWIRE));
    }
    
    udelay(aeswire_init_delay);

    if (eswire_reset) {
        WRITE_PERI_REG(GPIO_OUT_HIGH, (1 << PIN_ESWIRE));
    }
    else {
        WRITE_PERI_REG(GPIO_OUT_LOW, (1 << PIN_ESWIRE));
    }
}

void aeswire_process_cmd_line(cmd_line_t *cmd)
{
    int i;
    unsigned char value;
    char buf[256] = { 0, };
    unsigned char reg, data;

    reg = cmd->reg & 0xFF;
    data = cmd->data & 0xFF;

    switch (cmd->type) {
    case 'i':
    case 'I':
        aswire_init = reg;
        eswire_init = data;
        sprintf(buf, " [init] aswire init : %d, eswire init : %d\r\n", aswire_init, eswire_init);
        Serial.print(buf);
        aeswire_command_help();
        break;

    case 'r':
    case 'R':
        aswire_reset = reg;
        eswire_reset = data;
        sprintf(buf, " [reset] aswire reset : %d, eswire reset : %d\r\n", aswire_reset, eswire_reset);
        Serial.print(buf);
        aeswire_command_help();
        break;

    case 'b':
    case 'B':
        aeswire_init_delay = cmd->reg;
        sprintf(buf, " [delay] delay between aswire and eswire(us) : %x\r\n", aeswire_init_delay);
        Serial.print(buf);
        aeswire_command_help();
        break;

    case 'c':
    case 'C':
        aeswire_clk_width = cmd->reg;
        sprintf(buf, " [delay] eswire clock width(us) : %x\r\n", aeswire_clk_width);
        Serial.print(buf);
        aeswire_command_help();
        break;

    case 'a':
    case 'A':
        sprintf(buf, " [aswire set] addr : 0x%02x, data : 0x%02x\r\n", reg, data);
        Serial.print(buf);
        aswire_reg = reg & 0xFF;
        aswire_data = data & 0xFF;
        aeswire_command_help();
        break;

    case 'e':
    case 'E':
        sprintf(buf, " [eswire set] addr : 0x%02x, data : 0x%02x\r\n", reg, data);
        Serial.print(buf);
        eswire_reg = reg & 0xFF;
        eswire_data = data & 0xFF;
        aeswire_command_help();
        break;

    case 'w':
    case 'W':
        sprintf(buf, " [write] aswire : 0x%02x, 0x%02x  eswire : 0x%02x, 0x%02x\r\n", aswire_reg, aswire_data, eswire_reg, eswire_data);
        Serial.print(buf);
        aeswire_write(aswire_reg, aswire_data, eswire_reg, eswire_data);
        aeswire_saved_cmdline = *cmd;
        break;

    case 'h':
    case 'H':
        aeswire_command_help();
        break;

    default:
        Serial.print("Invalid cmd\r\n");
        break;
    }
}

void aeswire_command_help(void)
{
    char buf[256] = { 0, };

    Serial.print("\r\n");
    Serial.print("=================================================\r\n");
    Serial.print("                    a/esire Command Mode\r\n");
    Serial.print("=================================================\r\n");
    Serial.print(" i ainit einit : initial(ainit: aswire, einit : eswire) ex) i 0 1\r\n");
    Serial.print(" r arst erst   : reset(arst : aswire, erst : eswire)    ex) r 1 0\r\n");
    Serial.print(" b delay       : delay between aswire and eswire(ms)    ex) b 0A\r\n");
    Serial.print(" c width       : eswire clock width(udelay)             ex) c 0A\r\n");
    Serial.print(" a reg data    : set aswire write reg & data            ex) a 01 AE\r\n");
    Serial.print(" e reg data    : set eswire write reg & data            ex) e 01 AE\r\n");
    Serial.print(" w             : write aswire and eswire                ex) w\r\n");
    Serial.print(" Enter         : redo command(w)\r\n");
    Serial.print(" h             : print this menu\r\n");
    Serial.print(" q             : exit\r\n");
    Serial.print("-------------------------------------------------\r\n");
    Serial.print("       aswire  eswire\r\n");
    sprintf(buf, " init       %d      %d\r\n", aswire_init, eswire_init); Serial.print(buf);
    sprintf(buf, " reset      %d      %d\r\n", aswire_reset, eswire_reset); Serial.print(buf);
    sprintf(buf, " delay between aswire and eswire(us) : %x(%dus)\r\n", aeswire_init_delay, aeswire_init_delay); Serial.print(buf);
    sprintf(buf, " eswire clock width(us) : %x(%dus)\r\n", aeswire_clk_width, aeswire_clk_width); Serial.print(buf);
    sprintf(buf, " aswire addr : 0x%02x, data : 0x%02x\r\n", aswire_reg, aswire_data); Serial.print(buf);
    sprintf(buf, " eswire addr : 0x%02x, data : 0x%02x\r\n", eswire_reg, eswire_data); Serial.print(buf);
    Serial.print(" reg(8bit)\r\n");
    Serial.print(" data(8bit)\r\n");
    Serial.print("=================================================\r\n");
}

void test_aeswire_command(void)
{
    cmd_line_t cmd;
    aeswire_command_help();
    while (1) {
        Serial.print("AESWIRE>");
        if (get_cmd_line(&cmd, aeswire_saved_cmdline) == true) {
            aeswire_process_cmd_line(&cmd);
        }
        else {
            break;
        }
    }
}

void aeswire_setup()
{
    pinMode(PIN_ASWIRE, OUTPUT);
    pinMode(PIN_ESWIRE, OUTPUT);

    digitalWrite(PIN_ASWIRE, LOW);
    digitalWrite(PIN_ESWIRE, LOW);
}
