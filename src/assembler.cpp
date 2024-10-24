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

static int setLabel(Label* fixup_labels, char* cmd, int ip);
static void goThroughFixupLabels(Label labels[], int cmds[]);
static void pasteAdressArg(Label labels[], int* cmds, int* ip, FILE* inter_cmds);

static int getArg(const char* input_line, int cmds[], int ip);
static int getArgSD(int* cmds, char* reg_name, int cmd_type, int arg, int ip);
static int getArgD(int* cmds, int cmd_type, int arg, int ip);
static int getArgS(int* cmds, char* reg_name, int cmd_type, int ip);
static int setArg(FILE* inter_cmds, int* cmds, Label* fixup_labels, int ip, int type);
static int setArgPushPop(FILE* inter_cmds, int* cmds, int ip, int type);
static int ifRegister(const char* str);



int main(){
    int cmds[MAX_CMDS_SIZE] = {};

    runProgramm(cmds);

    return 0;
}

static void runProgramm(int cmds[]){
    assert(cmds);

    initCmds(cmds);
    initExeFile(cmds);
}

static void initCmds(int cmds[]){
    assert(cmds);

    FILE* inter_cmds = fopen("./inter_cmds.asm", "r");
    if (inter_cmds== NULL){
        assert(NULL);
    }
    char cmd[MAX_CMD_SIZE] = {};
    int ip = 0;

    Label fixup_labels[MAX_LABELS_AMOUNT] = {};
    labelsCtor(fixup_labels);

    #define DEF_CMD_(name, num, ...)                            \
        if (strcmp(cmd, #name) == 0){                           \
        ip = setArg(inter_cmds, cmds, fixup_labels, ip, name);  \
        continue;                                               \
    }

    while (1){
        if (fscanf(inter_cmds, "%s", cmd) == EOF){
            break;
        }

        #include "../include/cmds_gen.h"

        if (strchr(cmd, ':') != NULL){
            setLabel(fixup_labels, cmd, ip);
            continue;
        }
        printf("Syntax error: %s\n", cmd);
        assert(NULL);
    }

    #undef DEF_CMD_

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
    if ((strcmp(str, "AX") == 0) || (strcmp(str, "BX") == 0) || (strcmp(str, "CX") == 0) || (strcmp(str, "DX") == 0) || (strcmp(str, "EX") == 0) || (strcmp(str, "FX") == 0)){
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
        return getArgSD(cmds, reg_name, cmd_type, arg, ip);
    }
    if (sscanf(input_line, "%d", &arg) == 1){
        return getArgD(cmds, cmd_type, arg, ip);
    }
    if (sscanf(input_line, "%s", reg_name) == 1){
        return getArgS(cmds, reg_name, cmd_type, ip);
    }

    return ip;
}

static int setArg(FILE* inter_cmds, int* cmds, Label* fixup_labels, int ip, int type){
    if (type == PUSH || type == POP){
        return setArgPushPop(inter_cmds, cmds, ip, type);
    }
    else if ((type >= JMP && type <= JNE) || type == CALL){
        cmds[ip++] = type;
        pasteAdressArg(fixup_labels, cmds, &ip, inter_cmds);

        return ip;
    }
    else {
        cmds[ip++] = type;

        return ip;
    }
}

static int setLabel(Label* fixup_labels, char* cmd, int ip){
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

    return 0;
}

static int getArgSD(int* cmds, char* reg_name, int cmd_type, int arg, int ip){
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

static int getArgD(int* cmds, int cmd_type, int arg, int ip){
    cmds[ip++] = cmd_type | IMMED;
    cmds[ip++] = arg;

    return ip;
}

static int getArgS(int* cmds, char* reg_name, int cmd_type, int ip){
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

static int setArgPushPop(FILE* inter_cmds, int* cmds, int ip, int type){
    int cmd_ip = ip;
    char input_line[30] = {};
    fscanf(inter_cmds, "%[^\n]\n", input_line);

    if (strchr(input_line, '[') != NULL){
        ip = getArg(strchr(input_line, '[') + 1, cmds, ip);

        if ((cmds[cmd_ip] & IMMED) && ((cmds[cmd_ip] & MEM) == 0) && type == POP){
            printf("Error: constant in pop\n");
            assert(NULL);
        }
        cmds[cmd_ip] |= type;

        return ip;
    }

    ip = getArg(input_line, cmds, ip);
    cmds[cmd_ip] |= type;

    return ip;
}
