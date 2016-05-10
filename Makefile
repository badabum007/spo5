CC 			= gcc
CFLAGS 		= -c -Wall -pedantic
STAND 		= -std=gnu99
SHARED_LIB_DEPS 	= -pthread -lrt -shared
LIN_PROG_DEPS = -ldl -pthread

ifeq ($(OS), Windows_NT)

all: output

output: laba_win.o main.o 
	$(CC) laba_win.o main.o -o lab
	
laba_win.o: laba_win.c
	$(CC) $(CFLAGS) $(STAND) laba_win.c
	
main.o: main.c
	$(CC) $(CFLAGS) $(STAND) main.c 
else

all: fileconcat.so output

output: main.o laba_unix.o
	$(CC) main.o laba_unix.o -o lab $(LIN_PROG_DEPS)

fileconcat.so: fileconcat.o 
	$(CC) $(SHARED_LIB_DEPS) fileconcat.o -o fileconcat.so
	
fileconcat.o: fileconcat.c
	$(CC) $(CFLAGS) fileconcat.c -fPIC

laba_unix.o: laba_unix.c
	$(CC) $(CFLAGS) laba_unix.c
	
main.o: main.c
	$(CC) $(CFLAGS)  main.c

clean:
	rm -f *.o *.so lab
	
endif
