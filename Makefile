FLAGS=-c -Wall -O2 -Iinclude
SWAP=./swap
VERIFY=./verify.sh

all: build clean

build:
	gcc $(FLAGS) src/stringbuilder.c
	gcc $(FLAGS) src/tokenizer.c
	gcc $(FLAGS) src/parser.c
	gcc $(FLAGS) src/object.c
	gcc $(FLAGS) src/ast.c
	gcc $(FLAGS) src/operator.c
	gcc $(FLAGS) src/main.c
	gcc main.o stringbuilder.o tokenizer.o parser.o object.o ast.o operator.o -o swap

clean:
	rm *.o

test:
	@$(SWAP) ./tests/test1.txt > ./tests/result1.txt
	@$(SWAP) ./tests/test2.txt > ./tests/result2.txt
	@$(SWAP) ./tests/test3.txt > ./tests/result3.txt
	@$(SWAP) ./tests/test4.txt > ./tests/result4.txt
	@$(SWAP) ./tests/test5.txt > ./tests/result5.txt
	@$(VERIFY)
