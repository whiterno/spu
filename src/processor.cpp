#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "../lib/stack/stack_hcpp/stack.h"
#include "../include/commands.h"
#include "../include/processor.h"

static int runProgramm();
static int readFile(FILE* fp, int* cmds);
static int push(Processor* prc);
static int pop(Processor* prc);

int main(){
    runProgramm();

    return 0;
}

static int runProgramm(){
    Processor prc = {.stk = stackCtor(INIT(stk) 0), .ret_adr = stackCtor(INIT(ret_adr) 0), .cmds = {}, .registers = {}, .RAM = {}, .ip = 0};
    FILE* fp = fopen("./exe_cmds.txt", "r");

    readFile(fp, prc.cmds);

    #define DEF_CMD_(name, num, ...)                \
    case(name):{                                    \
        __VA_ARGS__                                 \
    }

    while (1){
        int cmd = prc.cmds[prc.ip++];
        int cmd_masked = cmd & 31;
        if (cmd == HLT || cmd == END_OF_CMDS){
            cmd_masked = HLT;
        }

        switch(cmd_masked){
            #include "../include/cmds_gen.h"
        }
    }

    #undef DEF_CMD_
}

static int readFile(FILE* fp, int* cmds){
    assert(fp);
    assert(cmds);

    int cmd = 0;
    int ip = 0;

    while (1){
        fscanf(fp, "%d", &cmd);
        cmds[ip++] = cmd;

        if (cmds[ip - 1] == END_OF_CMDS){
            break;
        }
    }

    return 0;
}

static int push(Processor* prc){
    assert(prc);

    int cmd = prc->cmds[prc->ip - 1];

    if ((cmd & IMMED) && (cmd & REG)){
        int value_i = prc->cmds[prc->ip++];
        int value_r = prc->registers[prc->cmds[prc->ip++]];

        if(cmd & MEM){
            stackPush(prc->stk, prc->RAM[value_i + value_r]);

            return prc->ip;
        }
        stackPush(prc->stk, value_i + value_r);

        return prc->ip;
    }
    if (cmd & IMMED){
        int value_i = prc->cmds[prc->ip++];

        if(cmd & MEM){
            stackPush(prc->stk, prc->RAM[value_i]);

            return prc->ip;
        }
        stackPush(prc->stk, value_i);

        return prc->ip;
    }
    if (cmd & REG){
        int value_r = prc->registers[prc->cmds[prc->ip++]];

        if(cmd & MEM){
            stackPush(prc->stk, prc->RAM[value_r]);

            return prc->ip;
        }
        stackPush(prc->stk, value_r);

        return prc->ip;
    }

    return prc->ip;
}

static int pop(Processor* prc){
    assert(prc);

    int cmd = prc->cmds[prc->ip - 1];

    if ((cmd & IMMED) && (cmd & REG)){
        int value_i = prc->cmds[prc->ip++];
        int value_r = prc->registers[prc->cmds[prc->ip++]];
        int value_ram = 0;

        stackPop(prc->stk, &value_ram);
        prc->RAM[value_i + value_r] = value_ram;

        return prc->ip;
    }
    if (cmd & IMMED){
        int value_i = prc->cmds[prc->ip++];
        int value_ram = 0;

        stackPop(prc->stk, &value_ram);
        prc->RAM[value_i] = value_ram;

        return prc->ip;
    }
    if ((cmd & REG) && ((cmd & MEM) == 0)){
        int value = 0;

        stackPop(prc->stk, &value);
        prc->registers[prc->cmds[prc->ip++]] = value;

        return prc->ip;
    }
    if ((cmd & REG)){
        int value_ram = 0;

        stackPop(prc->stk, &value_ram);
        prc->RAM[prc->registers[prc->cmds[prc->ip++]]] = value_ram;

        return prc->ip;
    }

    return prc->ip;

}

