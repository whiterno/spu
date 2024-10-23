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
static void pasteAdressArg(Label labels[], int* cmds, int* ip, FILE* inter_cmds);
static int ifRegister(const char* str);
static int getArg(const char* input_line, int cmds[], int ip);

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
        if (fscanf(inter_cmds, "%s", cmd) == EOF){
            break;
        }

        if (strcmp(cmd, "push") == 0){
            int cmd_ip = ip - 1;
            char input_line[30] = {};
            fscanf(inter_cmds, "%[^\n]\n", input_line);

            if (strchr(input_line, '[') != NULL){
                ip = getArg(strchr(input_line, '[') + 1, cmds, ip);
                cmds[cmd_ip] |= PUSH;
                continue;
            }

            ip = getArg(input_line, cmds, ip);
            cmds[cmd_ip] |= PUSH;
            continue;
        }
        if (strcmp(cmd, "pop") == 0){
            int cmd_ip = ip;
            char input_line[30] = {};
            fscanf(inter_cmds, "%[^\n]\n", input_line);

            if (strchr(input_line, '[') != NULL){
                ip = getArg(strchr(input_line, '[') + 1, cmds, ip);

                if ((cmds[cmd_ip] & IMMED) && ((cmds[cmd_ip] & MEM) == 0)){
                    printf("Error: constant in pop\n");
                    assert(NULL);
                }
                cmds[cmd_ip] |= POP;
                continue;
            }

            ip = getArg(input_line, cmds, ip);
            cmds[cmd_ip] |= POP;
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
            pasteAdressArg(fixup_labels, cmds, &ip, inter_cmds);
            continue;
        }
        if (strcmp(cmd, "ja") == 0){
            cmds[ip++] = JA;
            pasteAdressArg(fixup_labels, cmds, &ip, inter_cmds);
            continue;
        }
        if (strcmp(cmd, "jae") == 0){
            cmds[ip++] = JAE;
            pasteAdressArg(fixup_labels, cmds, &ip, inter_cmds);
            continue;
        }
        if (strcmp(cmd, "jb") == 0){
            cmds[ip++] = JB;
            pasteAdressArg(fixup_labels, cmds, &ip, inter_cmds);
            continue;
        }
        if (strcmp(cmd, "jbe") == 0){
            cmds[ip++] = JBE;
            pasteAdressArg(fixup_labels, cmds, &ip, inter_cmds);
            continue;
        }
        if (strcmp(cmd, "je") == 0){
            cmds[ip++] = JE;
            pasteAdressArg(fixup_labels, cmds, &ip, inter_cmds);
            continue;
        }
        if (strcmp(cmd, "jne") == 0){
            cmds[ip++] = JNE;
            pasteAdressArg(fixup_labels, cmds, &ip, inter_cmds);
            continue;
        }
        if (strcmp(cmd, "hlt") == 0){
            cmds[ip++] = HLT;
            continue;
        }
        if (strcmp(cmd, "draw") == 0){
            cmds[ip++] = DRAW;
            continue;
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

    cmds[ip] = END_OF_CMDS;

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
        fprintf(exe_cmds, "%d\n", cmds[ip++]);
        if (cmds[ip - 1] == END_OF_CMDS){
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

static void pasteAdressArg(Label labels[], int* cmds, int* ip, FILE* inter_cmds){
    char label_name[MAX_LABEL_NAME_SIZE] = {};

    fscanf(inter_cmds, "%s", label_name);
    int label_adress = findLabelAdress(labels, label_name, *ip);

    cmds[(*ip)++] = label_adress;
}

static int ifRegister(const char* str){
    if ((strcmp(str, "AX") == 0) || (strcmp(str, "BX") == 0) || (strcmp(str, "CX") == 0) || (strcmp(str, "DX") == 0)){
        return 1;
    }

    return 0;
}

static int getArg(const char* input_line, int cmds[], int ip){
    int arg = 0;
    char reg_name[MAX_REGISTER_SIZE] = {};

    int cmd_type = 0;
    if (strchr(input_line, ']') != NULL){
        cmd_type = MEM;
    }

    if (sscanf(input_line, "%s + %d", reg_name, &arg) == 2 || sscanf(input_line, "%d + %s", &arg, reg_name) == 2){
        if (reg_name[2] == ']'){
            reg_name[2] = '\0';
        }
        if (!ifRegister(reg_name)){
            printf("Error: not a register name\n");
            assert(NULL);
        }

        cmds[ip++] = (cmd_type | IMMED) | REG;
        cmds[ip++] = arg;
        cmds[ip++] = reg_name[0] - 'A' + 1;

        return ip;
    }
    if (sscanf(input_line, "%d", &arg) == 1){
        cmds[ip++] = cmd_type | IMMED;
        cmds[ip++] = arg;

        return ip;
    }
    if (sscanf(input_line, "%s", reg_name) == 1){
        if (reg_name[2] == ']'){
            reg_name[2] = '\0';
        }
        if (!ifRegister(reg_name)){
            printf("Error: not a register name\n");
            assert(NULL);
        }

        cmds[ip++] = cmd_type | REG;
        cmds[ip++] = reg_name[0] - 'A' + 1;

        return ip;
    }

    return ip;
}
