/*
 *  @file cmd.h
 *  
 *  Copyright (C) 2025 AZ ALL rights reserved.
 */

#ifndef _CMD_H_
#define _CMD_H_

typedef struct {
    char type;
    unsigned int reg;
    unsigned int data;
} cmd_line_t;

static bool get_cmd_line(cmd_line_t *cmd, cmd_line_t saved_cmdline)
{
    int c, index = 0;
    char buf[128];

    while (1) {
        if (!Serial.available()) {
            continue;
        }
        c = Serial.read();
        if (c == '\r') {
            if (index == 0) {
                *cmd = saved_cmdline;
            }
            else {
                unsigned int reg, data;
                sscanf(buf, "%c %x %x", &cmd->type, &reg, &data);
                cmd->reg  = reg;
                cmd->data = data;
            }
            break;
        }
        else if (c == 0x08) {
            if (index > 0) {
                index--;
            }
            Serial.write(c);
        }
        else if (isprint(c)) {
            buf[index++] = c;
            buf[index] = '\0';
            Serial.write(c);
            if (c == 'h' || c == 'H') {
                cmd->type = 'h';
                break;
            }
            if (c == 's' || c == 'S') {
                cmd->type = 's';
                break;
            }
            else if (c == 'q' || c == 'Q') {
                return (false);
            }
        }
        else {}
    }
    Serial.print("\r\n");

    return (true);
};
#endif  // _CMD_H_
