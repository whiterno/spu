#ifndef COMMANDS_H
#define COMMANDS_H

#define DEF_CMD_(name, num, ...) name = num,
enum Commands{
    END_OF_CMDS = -2,
    #include "cmds_gen.h"
};

enum Bits{
    IMMED = 128,
    REG   = 64,
    MEM   = 32
};
#undef DEF_CMD_

const int MAX_CMDS_SIZE = 512;
const int MAX_CMD_SIZE = 32;

#endif
