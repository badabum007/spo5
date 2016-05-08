#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PLATFORM_WIN 1 
#define PLATFORM_UNIX 2 

#if defined(_WIN32) || defined(_WIN64) 
	#include <windows.h>
	#include <process.h>
	#include <conio.h> 
	#define PLATFORM PLATFORM_WIN 
#else 
	#include <pthread.h>
	#include <unistd.h>
	#include <termios.h>
	#include <aio.h>
	#include <sys/wait.h>
	#include <sys/types.h>
	#define maxFileSize 500
	#define PLATFORM PLATFORM_UNIX
	char buf[maxFileSize];
	struct aiocb aiocb_; 
#endif 

struct Data 
{
	#if PLATFORM == PLATFORM_WIN
	CRITICAL_SECTION CriticalSection;
	HANDLE ThreadId;
	#else
	pthread_t reader, writer;
	int readerStatus, writerStatus;	
	pthread_mutex_t mutexRead, mutexWrite;
	#endif
	int argc;
	char** argv;
};

void createReader(struct Data*);
//void createWriter(struct Data*);
void* readFiles(void* );
//void* writeFiles(void* );

void createSignalObject(struct Data*);
void closeSignalObject(struct Data*);
