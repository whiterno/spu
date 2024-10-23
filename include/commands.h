#ifndef COMMANDS_H
#define COMMANDS_H

enum Commands{
    END_OF_CMDS = -2,
    HLT = -1,
    PUSH,
    POP,
    ADD,
    SUB,
    MULT,
    DIV,
    IN,
    OUT,
    JMP,
    JA,
    JAE,
    JB,
    JBE,
    JE,
    JNE,
    DRAW
};

enum Bits{
    IMMED = 128,
    REG   = 64,
    MEM   = 32
};

const int MAX_CMDS_SIZE = 512;
const int MAX_CMD_SIZE = 32;

#endif
