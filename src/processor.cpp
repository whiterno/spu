#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../lib/stack/stack_hcpp/stack.h"
#include "../include/commands.h"
#include "../include/processor.h"

static int runProgramm();
static int readFile(FILE* fp, int* cmds);
static int push(Stack* stk, int* cmds, int* RAM, int* registers, int ip);
static int pop(Stack* stk, int* cmds, int* RAM, int* registers, int ip);


int main(){
    runProgramm();

    return 0;
}

static int runProgramm(){
    Stack* stk = stackCtor(INIT(stk) 0);
    FILE* fp = fopen("./exe_cmds.txt", "r");
    int ip = 0;

    int cmds[MAX_CMDS_SIZE] = {};
    int registers[REGISTERS_AMOUNT] = {};
    int RAM[RAM_SIZE] = {};

    readFile(fp, cmds);

    while (1){
        int cmd = cmds[ip++];
        int cmd_masked = cmd & 31;
        if (cmd == HLT || cmd == END_OF_CMDS){
            cmd_masked = HLT;
        }

        switch(cmd_masked){
            case(HLT):{
                return 0;
            }
            case(PUSH):{
                ip = push(stk, cmds, RAM, registers, ip);
                continue;
            }
            case(POP):{
                ip = pop(stk, cmds, RAM, registers, ip);
                continue;
            }
            case(ADD):{
                int value1 = 0;
                int value2 = 0;

                stackPop(stk, &value1);
                stackPop(stk, &value2);
                stackPush(stk, value1 + value2);
                continue;
            }
            case(SUB):{
                int value1 = 0;
                int value2 = 0;

                stackPop(stk, &value1);
                stackPop(stk, &value2);
                stackPush(stk, value2 - value1);
                continue;
            }
            case(MULT):{
                int value1 = 0;
                int value2 = 0;

                stackPop(stk, &value1);
                stackPop(stk, &value2);
                stackPush(stk, value1 * value2);
                continue;
            }
            case(DIV):{
                int value1 = 0;
                int value2 = 0;

                stackPop(stk, &value1);
                stackPop(stk, &value2);
                stackPush(stk, value2 / value1);
                continue;
            }
            case(IN):{
                int value = 0;

                scanf("%d", &value);
                stackPush(stk, value);
                continue;
            }
            case(OUT):{
                int value = 0;

                stackPop(stk, &value);
                printf("%d\n", value);
                continue;
            }
            case(JMP):{
                ip = cmds[ip];
                continue;
            }
            case(JA):{
                int value1 = 0;
                int value2 = 0;

                stackPop(stk, &value2);
                stackPop(stk, &value1);

                if (value1 > value2){
                    ip = cmds[ip];
                }
                else{
                    ip++;
                }
                continue;
            }
            case(JAE):{
                int value1 = 0;
                int value2 = 0;

                stackPop(stk, &value2);
                stackPop(stk, &value1);

                if (value1 >= value2){
                    ip = cmds[ip];
                }
                else{
                    ip++;
                }
                continue;
            }
            case(JB):{
                int value1 = 0;
                int value2 = 0;

                stackPop(stk, &value2);
                stackPop(stk, &value1);

                if (value1 < value2){
                    ip = cmds[ip];
                }
                else{
                    ip++;
                }
                continue;
            }
            case(JBE):{
                int value1 = 0;
                int value2 = 0;

                stackPop(stk, &value2);
                stackPop(stk, &value1);

                if (value1 <= value2){
                    ip = cmds[ip];
                }
                else{
                    ip++;
                }
                continue;
            }
            case(JE):{
                int value1 = 0;
                int value2 = 0;

                stackPop(stk, &value2);
                stackPop(stk, &value1);

                if (value1 == value2){
                    ip = cmds[ip];
                }
                else{
                    ip++;
                }
                continue;
            }
            case(JNE):{
                int value1 = 0;
                int value2 = 0;

                stackPop(stk, &value2);
                stackPop(stk, &value1);

                if (value1 != value2){
                    ip = cmds[ip];
                }
                else{
                    ip++;
                }
                continue;
            }
            case(DRAW):{
                for (int row = 0; row < VIDEO_MEM_ROWS; row++){
                    for (int col = 0; col < VIDEO_MEM_COLS; col++){
                        printf(" %c", RAM[col + row * VIDEO_MEM_ROWS]);
                    }
                    printf("\n");
                }
            }
        }
    }
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

static int push(Stack* stk, int* cmds, int* RAM, int* registers, int ip){
    assert(stk);
    assert(cmds);
    assert(RAM);
    assert(registers);

    int cmd = cmds[ip - 1];

    if ((cmd & IMMED) && (cmd & REG)){
        int value_i = cmds[ip++];
        int value_r = registers[cmds[ip++]];

        if(cmd & MEM){
            stackPush(stk, RAM[value_i + value_r]);

            return ip;
        }
        stackPush(stk, value_i + value_r);

        return ip;
    }
    if (cmd & IMMED){
        int value_i = cmds[ip++];

        if(cmd & MEM){
            stackPush(stk, RAM[value_i]);

            return ip;
        }
        stackPush(stk, value_i);

        return ip;
    }
    if (cmd & REG){
        int value_r = registers[cmds[ip++]];

        if(cmd & MEM){
            stackPush(stk, RAM[value_r]);

            return ip;
        }
        stackPush(stk, value_r);

        return ip;
    }

    return ip;
}

static int pop(Stack* stk, int* cmds, int* RAM, int* registers, int ip){
    assert(stk);
    assert(cmds);
    assert(RAM);
    assert(registers);

    int cmd = cmds[ip - 1];

    if ((cmd & IMMED) && (cmd & REG)){
        int value_i = cmds[ip++];
        int value_r = registers[cmds[ip++]];
        int value_ram = 0;

        stackPop(stk, &value_ram);
        RAM[value_i + value_r] = value_ram;

        return ip;
    }
    if (cmd & IMMED){
        int value_i = cmds[ip++];
        int value_ram = 0;

        stackPop(stk, &value_ram);
        RAM[value_i] = value_ram;

        return ip;
    }
    if ((cmd & REG) && ((cmd & MEM) == 0)){
        int value = 0;

        stackPop(stk, &value);
        registers[cmds[ip++]] = value;

        return ip;
    }
    if ((cmd & REG)){
        int value_ram = 0;

        stackPop(stk, &value_ram);
        RAM[registers[cmds[ip++]]] = value_ram;

        return ip;
    }

    return ip;

}

