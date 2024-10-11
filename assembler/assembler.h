#ifndef ASSEMBLER_H
#define ASSEMBLER_H

const int MAX_CMDS_SIZE = 512;
const int MAX_CMD_SIZE = 32;

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

#endif
