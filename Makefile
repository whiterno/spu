.PHONY: assembly compile run

assembly:
	g++ assembler/assembler.cpp -o assembler/assembler.out
	./assembler/assembler.out

compile:
	g++ processor/processor.cpp -o processor/processor.out

run:
	processor/processor.out
