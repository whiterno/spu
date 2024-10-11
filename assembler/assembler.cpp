#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "assembler.h"

static void runProgramm(int cmds[]);
static void initCmds(int cmds[]);
static void initExeFile(int cmds[]);

int main(){
    int cmds[MAX_CMDS_SIZE] = {};

    runProgramm(cmds);

    return 0;
}

static void runProgramm(int cmds[]){
    initCmds(cmds);
    initExeFile(cmds);
}

static void initCmds(int cmds[]){
    FILE* inter_cmds = fopen("./inter_cmds.txt", "r");
    if (inter_cmds== NULL){
        assert(NULL);
    }
    char cmd[MAX_CMD_SIZE] = {};
    int ip = 0;

    while (1){
        fscanf(inter_cmds, "%s", cmd);

        if (strcmp(cmd, "push") == 0){
            int arg = 0;

            fscanf(inter_cmds, "%d", &arg);

            cmds[ip++] = PUSH;
            cmds[ip++] = arg;
            continue;
        }
        if (strcmp(cmd, "add") == 0){
            cmds[ip++] = ADD;
            continue;
        }
        if (strcmp(cmd, "sub") == 0){
            cmds[ip++] = SUB;
            continue;
        }
        if (strcmp(cmd, "div") == 0){
            cmds[ip++] = DIV;
            continue;
        }
        if (strcmp(cmd, "mult") == 0){
            cmds[ip++] = MULT;
            continue;
        }
        if (strcmp(cmd, "IN") == 0){
            cmds[ip++] = IN;
            continue;
        }
        if (strcmp(cmd, "out") == 0){
            cmds[ip++] = OUT;
            continue;
        }
        if (strcmp(cmd, "hlt") == 0){
            cmds[ip++] = HLT;
        }
        break;
    }

    fclose(inter_cmds);
}

static void initExeFile(int cmds[]){
    FILE* exe_cmds = NULL;
    if ((exe_cmds = fopen("./exe_cmds.txt", "w")) == NULL){
        assert(NULL);
    }
    int ip = 0;

    while (1){
        if (cmds[ip] == PUSH){
            fprintf(exe_cmds, "%d ", cmds[ip++]);
            fprintf(exe_cmds, "%d\n", cmds[ip++]);
            continue;
        }
        fprintf(exe_cmds, "%d\n", cmds[ip++]);
        if (cmds[ip] == HLT){
            break;
        }
    }

    fclose(exe_cmds);
}
