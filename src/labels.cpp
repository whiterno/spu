#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include "../include/labels.h"

static int isNum(char name[]);
static int labelInx(Label labels[], char name[]);
static int resizeLabelsJmp(Label labels[], int label_inx);

int labelsCtor(Label labels[]){
    assert(labels);

    for (int inx = 0; inx < MAX_LABELS_AMOUNT; inx++){
        labels[inx].name[0] = '\0';
        labels[inx].init_adress = -1;
        labels[inx].jmp_adresses = (int*)calloc(1, sizeof(int));
        labels[inx].jmp_adresses[0] = -1;
        labels[inx].jmp_amount = 1;
        labels[inx].jmp_used = 0;
    }

    return NO_ERROR;
}

int findLabelAdress(Label labels[], char name[], int ip){
    assert(labels);
    assert(name);

    char* ptr_to_colon = strchr(name, ':');
    (*ptr_to_colon) = '\0';

    if (isNum(name)){
        int adress = 0;
        sscanf(name, "%d", &adress);

        return adress;
    }
    int label_inx = labelInx(labels, name);
    if (label_inx == EXCEEDED_MAX_LABELS_AMOUNT){
        return EXCEEDED_MAX_LABELS_AMOUNT;
    }

    resizeLabelsJmp(labels, label_inx);
    labels[label_inx].jmp_adresses[labels[label_inx].jmp_used] = ip;
    labels[label_inx].jmp_used++;

    return labels[label_inx].init_adress;
}

int initLabelAdress(Label labels[], char name[], int ip){
    assert(labels);
    assert(name);

    char* ptr_to_colon = strchr(name, ':');
    (*ptr_to_colon) = '\0';

    int label_inx = labelInx(labels, name);
    if (label_inx == EXCEEDED_MAX_LABELS_AMOUNT){
        return EXCEEDED_MAX_LABELS_AMOUNT;
    }
    if (labels[label_inx].init_adress != -1){
        return DOUBLE_INIT;
    }
    labels[label_inx].init_adress = ip;

    return NO_ERROR;
}

int labelsDtor(Label labels[]){
    assert(labels);

    for (int inx = 0; inx < MAX_LABELS_AMOUNT; inx++){
        labels[inx].name[0] = '\0';
        labels[inx].init_adress = -1;
        free(labels[inx].jmp_adresses);
        labels[inx].jmp_adresses = NULL;
    }

    return NO_ERROR;
}

static int isNum(char name[]){
    assert(name);

    char* ptr = name;

    while (*ptr != '\0'){
        if (!isdigit(*ptr)){
            break;
        }
        ptr++;
    }
    if (*ptr == '\0'){
        return 1;
    }
    return 0;
}

static int labelInx(Label labels[], char name[]){
    assert(labels);
    assert(name);

    for (int inx = 0; inx < MAX_LABELS_AMOUNT; inx++){
        if (labels[inx].name[0] == '\0'){
            strcpy(labels[inx].name, name);

            return inx;
        }
        if (strcmp(labels[inx].name, name) == 0){
            return inx;
        }
    }

    return EXCEEDED_MAX_LABELS_AMOUNT;
}

static int resizeLabelsJmp(Label labels[], int label_inx){
    assert(labels);

    if (labels[label_inx].jmp_amount == labels[label_inx].jmp_used){
        labels[label_inx].jmp_amount *= RESIZE_CONSTANT;

        int new_size = labels[label_inx].jmp_amount * sizeof(int);
        labels[label_inx].jmp_adresses = (int*)realloc(labels[label_inx].jmp_adresses, new_size);
    }

    return NO_ERROR;
}
