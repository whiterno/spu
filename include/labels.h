#ifndef LABELS_H
#define LABELS_H

const int MAX_LABEL_NAME_SIZE = 32;
const int MAX_LABELS_AMOUNT = 8;
const int RESIZE_CONSTANT = 2;

struct Label{
    char name[MAX_LABEL_NAME_SIZE];
    int init_adress;
    int* jmp_adresses;
    int jmp_used;
    int jmp_amount;
};

enum LabelErrors{
    EXCEEDED_MAX_LABELS_AMOUNT = -1,
    DOUBLE_INIT,
    NO_ERROR
};

int labelsCtor(Label labels[]);
int findLabelAdress(Label labels[], char name[], int ip);
int labelsDtor(Label labels[]);
int initLabelAdress(Label labels[], char name[], int ip);

#endif
