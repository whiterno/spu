IN
IN
IN
POP CX
POP BX
POP AX

CALL QUADR:
PUSH CX
PUSH EX
PUSH FX
OUT
OUT
OUT

HLT

QUADR:

    PUSH AX
    PUSH 0
    JNE NEXT1:

        PUSH BX
        PUSH 0
        JNE NEXT3:

            PUSH CX
            PUSH 0
            JNE NEXT4:

                PUSH 8
                POP CX
                PUSH 0
                PUSH 0
                POP EX
                POP FX

                RET

            NEXT4:

            PUSH -1
            POP CX
            PUSH 0
            PUSH 0
            POP FX
            POP EX

            RET

        NEXT3:
        CALL LINEAR:
        RET

    NEXT1:

    PUSH BX
    PUSH BX
    MULT
    PUSH 4
    PUSH AX
    PUSH CX
    MULT
    MULT
    SUB
    POP DX

    PUSH DX
    PUSH 0
    JAE NEXT2:

        PUSH -1
        POP CX

        RET

    NEXT2:

    PUSH DX
    SQRT
    POP DX

    PUSH BX
    PUSH -1
    MULT
    PUSH DX
    ADD
    PUSH AX
    PUSH 2
    MULT
    DIV
    POP EX

    PUSH BX
    PUSH -1
    MULT
    PUSH DX
    SUB
    PUSH AX
    PUSH 2
    MULT
    DIV
    POP FX

    PUSH EX
    PUSH FX
    JE NEXT5:

        PUSH 2
        POP CX

        RET
    NEXT5:

    PUSH 1
    POP CX

RET

LINEAR:

    PUSH -1
    PUSH CX
    MULT
    PUSH BX
    DIV
    POP EX
    PUSH EX
    POP FX
    PUSH 1
    POP CX

RET
