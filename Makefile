CC 			= gcc
CFLAGS 		= -c -Wall -pedantic
STAND 		= -std=gnu99
ifeq ($(OS), Windows_NT)
SHARED_LIB_LINK_DEPS 	= -shared --enable-runtime-pseudo-reloc
OS_SPECIFIC_PART 		= laba_win
LIB_NAME 				= win_fileconcat
LIB_EXT 				= .dll
OUTPUT_DEPS 			= 
SHARED_LIB_COMPILE_DEPS = 
CLEAN_INSTR				= del /f
else
SHARED_LIB_LINK_DEPS	= -pthread -lrt -shared
OS_SPECIFIC_PART 		= laba_unix
LIB_NAME 				= fileconcat
LIB_EXT 				= .so
OUTPUT_DEPS 			= -ldl -pthread
SHARED_LIB_COMPILE_DEPS = -fPIC 
CLEAN_INSTR				= rm -f
endif

all: $(LIB_NAME)$(LIB_EXT) output

output: $(OS_SPECIFIC_PART).o main.o 
	$(CC) $(OS_SPECIFIC_PART).o main.o -o lab $(OUTPUT_DEPS)
	
$(OS_SPECIFIC_PART).o: $(OS_SPECIFIC_PART).c
	$(CC) $(CFLAGS) $(STAND) $(OS_SPECIFIC_PART).c
	
main.o: main.c
	$(CC) $(CFLAGS) $(STAND) main.c 

$(LIB_NAME)$(LIB_EXT): $(LIB_NAME).o 
	$(CC) $(SHARED_LIB_LINK_DEPS) $(LIB_NAME).o -o $(LIB_NAME)$(LIB_EXT) 
	
$(LIB_NAME).o: $(LIB_NAME).c
	$(CC) $(CFLAGS) $(STAND) $(LIB_NAME).c $(SHARED_LIB_COMPILE_DEPS)

clean:
	$(CLEAN_INSTR) *.o lab *$(LIB_EXT)

