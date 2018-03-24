FLAGS=-c -Wall -O2 -Iinclude
SWAP=./swap
VERIFY=./verify.sh
CC=gcc

# *nix
SHARED=libSwapNative.so
# Windows / Cygwin
#SHARED=SwapNative.dll

all: build clean

build:
	$(CC) -Iinclude src/SwapNative.c src/object.c src/ast.c src/stringbuilder.c src/socket.c src/operator.c src/native.c src/thread.c -fpic -shared -o $(SHARED)
	$(CC) $(FLAGS) src/SwapNative.c
	$(CC) $(FLAGS) src/stringbuilder.c
	$(CC) $(FLAGS) src/import.c
	$(CC) $(FLAGS) src/tokenizer.c
	$(CC) $(FLAGS) src/parser.c
	$(CC) $(FLAGS) src/object.c
	$(CC) $(FLAGS) src/ast.c
	$(CC) $(FLAGS) src/operator.c
	$(CC) $(FLAGS) src/native.c
	$(CC) $(FLAGS) src/socket.c
	$(CC) $(FLAGS) src/thread.c
	$(CC) $(FLAGS) src/main.c
#Unix-like systems
	$(CC) main.o stringbuilder.o import.o tokenizer.o parser.o object.o ast.o operator.o native.o socket.o  -o swap -ldl -lpthread
#Windows
#$(CC) main.o stringbuilder.o import.o tokenizer.o parser.o object.o ast.o operator.o native.o socket.o -o swap

debug:
	$(CC) -Iinclude src/SwapNative.c src/object.c src/ast.c src/stringbuilder.c src/socket.c src/operator.c src/native.c src/thread.c -g -fpic -shared -o $(SHARED)
	$(CC) -g $(FLAGS) src/SwapNative.c
	$(CC) -g $(FLAGS) src/stringbuilder.c
	$(CC) -g $(FLAGS) src/import.c
	$(CC) -g $(FLAGS) src/tokenizer.c
	$(CC) -g $(FLAGS) src/parser.c
	$(CC) -g $(FLAGS) src/object.c
	$(CC) -g $(FLAGS) src/ast.c
	$(CC) -g $(FLAGS) src/operator.c
	$(CC) -g $(FLAGS) src/native.c
	$(CC) -g $(FLAGS) src/socket.c
	$(CC) $(FLAGS) src/thread.c
	$(CC) -g $(FLAGS) src/main.c
#Unix-like systems
	$(CC) main.o stringbuilder.o import.o tokenizer.o parser.o object.o ast.o operator.o native.o socket.o  thread.o -o swap -ldl -lpthread
#Windows
#$(CC) main.o stringbuilder.o import.o tokenizer.o parser.o object.o ast.o operator.o native.o socket.o -o swap

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
