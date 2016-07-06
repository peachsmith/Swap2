FLAGS=-c -Wall -O2 -Iinclude

all:
	gcc $(FLAGS) src/stringbuilder.c
	gcc $(FLAGS) src/tokenizer.c
	gcc $(FLAGS) src/parser.c
	gcc $(FLAGS) src/object.c
	gcc $(FLAGS) src/main.c
	gcc main.o stringbuilder.o tokenizer.o parser.o object.o -o swap

clean:
	rm *.o
