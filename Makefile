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
	gcc $(FLAGS) src/native.c
	gcc $(FLAGS) src/main.c
	gcc main.o stringbuilder.o tokenizer.o parser.o object.o ast.o operator.o native.o -o swap

debug:
	gcc -g $(FLAGS) src/stringbuilder.c
	gcc -g $(FLAGS) src/tokenizer.c
	gcc -g $(FLAGS) src/parser.c
	gcc -g $(FLAGS) src/object.c
	gcc -g $(FLAGS) src/ast.c
	gcc -g $(FLAGS) src/operator.c
	gcc -g $(FLAGS) src/native.c
	gcc -g $(FLAGS) src/main.c
	gcc main.o stringbuilder.o tokenizer.o parser.o object.o ast.o operator.o native.o -o swap

clean:
	rm *.o

test:
	@$(SWAP) -a ./tests/test1.txt > ./tests/result1.txt
	@$(SWAP) -a ./tests/test2.txt > ./tests/result2.txt
	@$(SWAP) -a ./tests/test3.txt > ./tests/result3.txt
	@$(SWAP) -a ./tests/test4.txt > ./tests/result4.txt
	@$(SWAP) -a ./tests/test5.txt > ./tests/result5.txt
	@$(SWAP) -o ./tests/test6.txt > ./tests/result6.txt
	@$(SWAP) -o ./tests/test7.txt > ./tests/result7.txt
	@$(VERIFY)
