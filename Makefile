FLAGS=-c -Wall -O2 -Iinclude
SWAP=./swap
VERIFY=./verify.sh

# change to SwapNative.dll when using Cygwin on Windows
SHARED=libSwapNative.so

all: build clean

build:
	gcc -fpic -Iinclude src/SwapNative.c src/object.c src/ast.c -shared -o $(SHARED)
	gcc $(FLAGS) src/stringbuilder.c
	gcc $(FLAGS) src/import.c
	gcc $(FLAGS) src/tokenizer.c
	gcc $(FLAGS) src/parser.c
	gcc $(FLAGS) src/object.c
	gcc $(FLAGS) src/ast.c
	gcc $(FLAGS) src/operator.c
	gcc $(FLAGS) src/native.c
	gcc $(FLAGS) src/main.c
#Unix-like systems
	gcc main.o stringbuilder.o import.o tokenizer.o parser.o object.o ast.o operator.o native.o -o swap -ldl
#Windows
#gcc main.o stringbuilder.o import.o tokenizer.o parser.o object.o ast.o operator.o native.o -o swap

debug:
	gcc -g $(FLAGS) src/stringbuilder.c
	gcc -g $(FLAGS) src/import.c
	gcc -g $(FLAGS) src/tokenizer.c
	gcc -g $(FLAGS) src/parser.c
	gcc -g $(FLAGS) src/object.c
	gcc -g $(FLAGS) src/ast.c
	gcc -g $(FLAGS) src/operator.c
	gcc -g $(FLAGS) src/native.c
	gcc -g $(FLAGS) src/main.c
#Unix-like systems
	gcc main.o stringbuilder.o import.o tokenizer.o parser.o object.o ast.o operator.o native.o -o swap -ldl
#Windows
#gcc main.o stringbuilder.o import.o tokenizer.o parser.o object.o ast.o operator.o native.o -o swap

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
	@$(SWAP) ./tests/test8.txt > ./tests/result8.txt
	@$(SWAP) ./tests/test9.txt > ./tests/result9.txt
	@$(VERIFY)
