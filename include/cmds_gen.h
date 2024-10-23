DEF_CMD_(HLT, -1,
{
    return 0;
})

DEF_CMD_(PUSH, 0,
{
    prc.ip = push(&prc);
    continue;
})

DEF_CMD_(POP, 1,
{
    prc.ip = pop(&prc);
    continue;
})

DEF_CMD_(ADD, 2,
{
    int value1 = 0;
    int value2 = 0;

    stackPop(prc.stk, &value1);
    stackPop(prc.stk, &value2);
    stackPush(prc.stk, value1 + value2);
    continue;
})

DEF_CMD_(SUB, 3,
{
    int value1 = 0;
    int value2 = 0;

    stackPop(prc.stk, &value1);
    stackPop(prc.stk, &value2);
    stackPush(prc.stk, value2 - value1);
    continue;
})

DEF_CMD_(MULT, 4,
{
    int value1 = 0;
    int value2 = 0;

    stackPop(prc.stk, &value1);
    stackPop(prc.stk, &value2);
    stackPush(prc.stk, value1 * value2);
    continue;
})

DEF_CMD_(DIV, 5,
{
    int value1 = 0;
    int value2 = 0;

    stackPop(prc.stk, &value1);
    stackPop(prc.stk, &value2);
    stackPush(prc.stk, value2 / value1);
    continue;
})

DEF_CMD_(IN, 6,
{
    int value = 0;

    scanf("%d", &value);
    stackPush(prc.stk, value);
    continue;
})

DEF_CMD_(OUT, 7,
{
    int value = 0;

    stackPop(prc.stk, &value);
    printf("%d\n", value);
    continue;
})

DEF_CMD_(JMP, 8,
{
    prc.ip = prc.cmds[prc.ip];
    continue;
})

DEF_CMD_(JA, 9,
{
    int value1 = 0;
    int value2 = 0;

    stackPop(prc.stk, &value2);
    stackPop(prc.stk, &value1);

    if (value1 > value2){
        prc.ip = prc.cmds[prc.ip];
    }
    else{
        prc.ip++;
    }
    continue;
})

DEF_CMD_(JAE, 10,
{
    int value1 = 0;
    int value2 = 0;

    stackPop(prc.stk, &value2);
    stackPop(prc.stk, &value1);

    if (value1 >= value2){
        prc.ip = prc.cmds[prc.ip];
    }
    else{
        prc.ip++;
    }
    continue;
})

DEF_CMD_(JB, 11,
{
    int value1 = 0;
    int value2 = 0;

    stackPop(prc.stk, &value2);
    stackPop(prc.stk, &value1);

    if (value1 < value2){
        prc.ip = prc.cmds[prc.ip];
    }
    else{
        prc.ip++;
    }
    continue;
})

DEF_CMD_(JBE, 12,
{
    int value1 = 0;
    int value2 = 0;

    stackPop(prc.stk, &value2);
    stackPop(prc.stk, &value1);

    if (value1 <= value2){
        prc.ip = prc.cmds[prc.ip];
    }
    else{
        prc.ip++;
    }
    continue;
})

DEF_CMD_(JE, 13,
{
    int value1 = 0;
    int value2 = 0;

    stackPop(prc.stk, &value2);
    stackPop(prc.stk, &value1);

    if (value1 == value2){
        prc.ip = prc.cmds[prc.ip];
    }
    else{
        prc.ip++;
    }
    continue;
})

DEF_CMD_(JNE, 14,
{
    int value1 = 0;
    int value2 = 0;

    stackPop(prc.stk, &value2);
    stackPop(prc.stk, &value1);

    if (value1 != value2){
        prc.ip = prc.cmds[prc.ip];
    }
    else{
        prc.ip++;
    }
    continue;
})

DEF_CMD_(DRAW, 15,
{
    for (int row = 0; row < VIDEO_MEM_ROWS; row++){
        for (int col = 0; col < VIDEO_MEM_COLS; col++){
            printf(" %c", prc.RAM[col + row * VIDEO_MEM_ROWS]);
        }
        printf("\n");
    }
})
