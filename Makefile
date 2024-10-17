.PHONY: assembly compile run all


assembly: obj/assembler.o
	g++ obj/assembler.o -o assembler.out
	./assembler.out

obj/assembler.o: src/assembler.cpp
	g++ -c src/assembler.cpp -o obj/assembler.o


compile: obj/processor.o obj/stack.o
	g++ obj/processor.o obj/stack.o -o processor.out

obj/processor.o: src/processor.cpp
	g++ -c src/processor.cpp -o obj/processor.o


run:
	./processor.out


obj/stack.o:
	g++ -c lib/stack/stack_hcpp/stack.cpp -o obj/stack.o

all:
	make assembly
	make compile
	make run
