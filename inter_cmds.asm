push 0
pop AX

push AX
push AX
mult
out

push AX
push 1
add
pop AX

push AX
push 16
jbe next:

hlt

next:

push 100
out

hlt
