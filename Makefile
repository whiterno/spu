.PHONY: assembly compile run all

assembly: obj/labels.o obj/assembler.o
	g++ obj/assembler.o obj/labels.o -o exe/assembler.out
	exe/assembler.out

obj/assembler.o: src/assembler.cpp
	g++ -c src/assembler.cpp -o obj/assembler.o

compile: obj/processor.o obj/stack.o
	g++ obj/processor.o obj/stack.o -o exe/processor.out

obj/processor.o: src/processor.cpp
	g++ -c src/processor.cpp -o obj/processor.o

run:
	exe/processor.out

obj/stack.o:
	g++ -c lib/stack/stack_hcpp/stack.cpp -o obj/stack.o

obj/labels.o: src/labels.cpp
	g++ -c src/labels.cpp -o obj/labels.o

all:
	make assembly
	make compile
	make run
