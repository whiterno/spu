#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/assembler.h"
#include "../include/commands.h"
#include "../include/labels.h"

static void runProgramm(int cmds[]);
static void initCmds(int cmds[]);
static void initExeFile(int cmds[]);
static void goThroughFixupLabels(Label labels[], int cmds[]);

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
    FILE* inter_cmds = fopen("./inter_cmds.asm", "r");
    if (inter_cmds== NULL){
        assert(NULL);
    }
    char cmd[MAX_CMD_SIZE] = {};
    int ip = 0;

    Label fixup_labels[MAX_LABELS_AMOUNT] = {};
    labelsCtor(fixup_labels);

    while (1){
        fscanf(inter_cmds, "%s", cmd);

        if (strcmp(cmd, "push") == 0){
            int arg = 0;

            fscanf(inter_cmds, "%d", &arg);

            cmds[ip++] = PUSH;
            cmds[ip++] = arg;
            continue;
        }
        if (strcmp(cmd, "pushr") == 0){
            char reg_name[3] = {};

            fscanf(inter_cmds, "%s", reg_name);

            cmds[ip++] = PUSHR;
            cmds[ip++] = reg_name[0] - 'A' + 1;
            continue;
        }
        if (strcmp(cmd, "pop") == 0){
            char reg_name[MAX_REGISTER_SIZE] = {};

            fscanf(inter_cmds, "%s", reg_name);

            cmds[ip++] = POP;
            cmds[ip++] = reg_name[0] - 'A' + 1;
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
        if (strcmp(cmd, "in") == 0){
            cmds[ip++] = IN;
            continue;
        }
        if (strcmp(cmd, "out") == 0){
            cmds[ip++] = OUT;
            continue;
        }

        if (strcmp(cmd, "jmp") == 0){
            cmds[ip++] = JMP;
            char label_name[MAX_LABEL_NAME_SIZE] = {};

            fscanf(inter_cmds, "%s", label_name);
            int label_adress = findLabelAdress(fixup_labels, label_name, ip);

            cmds[ip++] = label_adress;
            continue;
        }

        if (strcmp(cmd, "hlt") == 0){
            cmds[ip++] = HLT;
            break;
        }
        if (strchr(cmd, ':') != NULL){
            char label_name[MAX_LABEL_NAME_SIZE] = {};
            strcpy(label_name, cmd);
            int err = initLabelAdress(fixup_labels, label_name, ip);
            if (err == EXCEEDED_MAX_LABELS_AMOUNT){
                printf("Error: exceeded max labels amount\n");
                assert(NULL);
            }
            if (err == DOUBLE_INIT){
                printf("Error: double label initialization\n");
                assert(NULL);
            }
            continue;
        }
        printf("Syntax error: %s\n", cmd);
        assert(NULL);
    }

    fclose(inter_cmds);
    goThroughFixupLabels(fixup_labels, cmds);
    labelsDtor(fixup_labels);
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
        if (cmds[ip] == POP){
            fprintf(exe_cmds, "%d ", cmds[ip++]);
            fprintf(exe_cmds, "%d\n", cmds[ip++]);
        }
        if (cmds[ip] == PUSHR){
            fprintf(exe_cmds, "%d ", cmds[ip++]);
            fprintf(exe_cmds, "%d\n", cmds[ip++]);
            continue;
        }
        if (cmds[ip] == JMP){
            fprintf(exe_cmds, "%d ", cmds[ip++]);
            fprintf(exe_cmds, "%d\n", cmds[ip++]);
            continue;
        }
        fprintf(exe_cmds, "%d\n", cmds[ip++]);
        if (cmds[ip - 1] == HLT){
            break;
        }
    }

    fclose(exe_cmds);
}

static void goThroughFixupLabels(Label labels[], int cmds[]){
    for (int label_inx = 0; label_inx < MAX_LABELS_AMOUNT; label_inx++){
        for (int jmp_inx = 0; jmp_inx < labels[label_inx].jmp_used; jmp_inx++){
            cmds[labels[label_inx].jmp_adresses[jmp_inx]] = labels[label_inx].init_adress;
            if (labels[label_inx].init_adress == -1){
                printf("Error: label not initialized\n");
                assert(NULL);
            }
        }
    }
}
