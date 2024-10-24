#ifndef PROCESSOR_H
#define PROCESSOR_H

const int REGISTERS_AMOUNT = 7;
const int RAM_SIZE = 900;
const int VIDEO_MEM_COLS = 30;
const int VIDEO_MEM_ROWS = 30;

struct Processor{
    Stack* stk;
    Stack* ret_adr;
    int cmds[MAX_CMDS_SIZE];
    int RAM[RAM_SIZE];
    int registers[REGISTERS_AMOUNT];
    int ip;
};

#endif
