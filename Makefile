Headers := include/assembler.h include/commands.h include/disassembler.h include/labels.h include/processor.h

.PHONY: assembly compile run all

assembly: obj/assembler.o obj/labels.o
	g++ obj/assembler.o obj/labels.o -o exe/assembler.out
	exe/assembler.out

obj/assembler.o: src/assembler.cpp ${Headers}
	g++ -c src/assembler.cpp -o obj/assembler.o

compile: obj/processor.o obj/stack.o
	g++ obj/processor.o obj/stack.o -o exe/processor.out

obj/processor.o: src/processor.cpp ${Headers}
	g++ -c src/processor.cpp -o obj/processor.o

run:
	exe/processor.out

obj/stack.o: lib/stack/stack_hcpp/stack.cpp lib/stack/stack_hcpp/stack.h
	g++ -c lib/stack/stack_hcpp/stack.cpp -o obj/stack.o

obj/labels.o: src/labels.cpp ${Headers}
	g++ -c src/labels.cpp -o obj/labels.o

all:
	make assembly
	make compile
	make run
