FLAGS=-c -Wall -O2

all:
	gcc $(FLAGS) stringbuilder.c
	gcc $(FLAGS) tokenizer.c
	gcc $(FLAGS) parser.c
	gcc $(FLAGS) object.c
	gcc $(FLAGS) main.c
	gcc main.o stringbuilder.o tokenizer.o parser.o object.o -o swap
