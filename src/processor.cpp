#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../lib/stack/stack_hcpp/stack.h"
#include "../include/commands.h"
#include "../include/processor.h"

int runProgramm();
int readFile(FILE* fp, int* cmds);


int main(){
    runProgramm();

    return 0;
}

int runProgramm(){
    Stack* stk = stackCtor(0);
    FILE* fp = fopen("./exe_cmds.txt", "r");
    int cmd = 0;
    int ip = 0;
    int cmds[MAX_CMDS_SIZE] = {};
    int registers[MAX_REGISTERS_AMOUNT] = {};

    readFile(fp, cmds);

    while (1){
        cmd = cmds[ip++];

        switch(cmd){
            case(HLT):{
                return 0;
            }
            case(PUSH):{
                int value = cmds[ip++];

                stackPush(stk, value);
                continue;
            }
            case(PUSHR):{
                stackPush(stk, registers[cmds[ip++]]);
                continue;
            }
            case(POP):{
                int value = 0;

                stackPop(stk, &value);
                registers[cmds[ip++]] = value;
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
                continue;
            }
        }
    }
}

int readFile(FILE* fp, int* cmds){
    assert(fp);
    assert(cmds);

    int cmd = 0;
    int ip = 0;

    while (1){
        fscanf(fp, "%d", &cmd);
        cmds[ip++] = cmd;

        if (cmds[ip - 1] == HLT){
            break;
        }
    }

    return 0;
}
