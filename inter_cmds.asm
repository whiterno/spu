push 0
push 0
pop AX
pop BX

first:

    push AX
    push -15
    add
    push AX
    push -15
    add
    mult
    push BX
    push -15
    add
    push BX
    push -15
    add
    mult
    add
    push 25
    ja next:

        push 42
        push BX
        push 30
        mult
        pop CX
        push AX
        push CX
        add
        pop CX
        pop [CX]

    next:

    push AX
    push -15
    add
    push AX
    push -15
    add
    mult
    push BX
    push -15
    add
    push BX
    push -15
    add
    mult
    add
    push 25
    jbe elsenext:

        push 46
        push BX
        push 30
        mult
        pop CX
        push AX
        push CX
        add
        pop CX
        pop [CX]

    elsenext:

    push 1
    push AX
    add
    pop AX

    push AX
    push 30
    jne if1:

        push 1
        push BX
        add
        pop BX

        push 0
        pop AX

    if1:

    push BX
    push 30
    mult
    pop CX
    push AX
    push CX
    add
    push 900
    jb first:

draw

hlt
