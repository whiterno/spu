push 0
pop AX

next:

pushr AX
pushr AX
mult
out

pushr AX
push 1
add
pop AX

jmp next:


hlt
