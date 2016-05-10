#include "headers.h"

void createReader(struct Data *data)
{	
	strcpy(data->readerEventName, "ReadEvent");
	strcpy(data->writerEventName, "WriteEvent");
	ZeroMemory(&(data->overRead), sizeof(OVERLAPPED));
	ZeroMemory(&(data->overWrite), sizeof(OVERLAPPED));
	data->overRead.hEvent = createEvent(data->readerEventName);
	data->overWrite.hEvent = createEvent(data->writerEventName);
	SetEvent(data->overWrite.hEvent);

	HANDLE hand;
	hand = (HANDLE)_beginthreadex( NULL, 0, readFiles, (PVOID)data, 0, NULL);
	data->readerId = hand;
}

HANDLE createEvent(TCHAR* overlappedEventName)
{
	HANDLE hEvent = CreateEvent(NULL, FALSE, TRUE, overlappedEventName);
	if (hEvent == NULL || hEvent == INVALID_HANDLE_VALUE)
	{
		puts("CreateEvent() error");
		exit(1);
	}

	return hEvent;
}

void createSignalObject(struct Data *data)
{
	InitializeCriticalSection(&(data->readerSection));
	InitializeCriticalSection(&(data->writerSection));
	EnterCriticalSection(&(data->writerSection));
	data->readerStatus = 0;
	strcpy(data->libName, "./fileconcat.so");
}

void closeSignalObject(struct Data *data)
{
	DeleteCriticalSection(&(data->readerSection));
	DeleteCriticalSection(&(data->writerSection));
}

/*void dynLabOpen(struct Data* data)
{
	data->handle = dlopen(data->libName, RTLD_LAZY);
}

void dynLabClose(struct Data* data)
{
	dlclose(data->handle);
}*/

void callWriteFiles(struct Data* data)
{
	HANDLE hFile;
	hFile = CreateFile(data->argv[data->argc - 1], GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	DWORD len = 0;

	for(int i = 2; i < data->argc; i++) {
		EnterCriticalSection(&(data->writerSection));
		WaitForSingleObject(data->overRead.hEvent, INFINITE);	
		data->overWrite.Offset = len;
		len += strlen(buf);
		puts(buf);
		WriteFile(hFile, buf, strlen(buf), NULL,  &(data->overWrite));
		LeaveCriticalSection(&(data->readerSection));
	}
	CloseHandle(hFile);	
}

unsigned  __stdcall readFiles(PVOID data_tmp)
{
	struct Data* data;
	data = (struct Data*)data_tmp;
	HANDLE hFile;

	for(int i = 2; i < data->argc; i++) {
		EnterCriticalSection(&(data->readerSection));
		WaitForSingleObject(data->overWrite.hEvent, INFINITE);
		hFile = CreateFile(data->argv[i-1], GENERIC_READ, 
		0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		memset((void*)buf, 0, sizeof(buf));
		ReadFile(hFile, buf, maxFileSize, NULL, &(data->overRead));
		LeaveCriticalSection(&(data->writerSection));
		CloseHandle(hFile);
	}
	ExitThread(data->readerStatus);
}
