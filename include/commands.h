#ifndef COMMANDS_H
#define COMMANDS_H

enum Commands{
    HLT = -1,
    PUSH,
    ADD,
    SUB,
    MULT,
    DIV,
    IN,
    OUT
};

const int MAX_CMDS_SIZE = 512;
const int MAX_CMD_SIZE = 32;

#endif
