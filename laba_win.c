#include "headers.h"

void createReader(struct Data *data)
{	
	data->readerEventName = TEXT("ReadEvent");
	data->writerEventName = TEXT("WriteEvent");
	ZeroMemory(&(data->overRead), sizeof(OVERLAPPED));
	ZeroMemory(&(data->overWrite), sizeof(OVERLAPPED));
	data->overRead->hEvent = createEvent(data->readEventName);
	data->overWrite->hEvent = createEvent(data->writeEventName);
	SetEvent(data->overWrite->hEvent);

	HANDLE hand;
	hand = (HANDLE)_beginthreadex( NULL, 0, readFiles, (PVOID)data, 0, NULL);
	data->readerId = hand;
}

HANDLE createEvent(TCHAR* overlappedEventName)
{
	HANDLE hEvent = CreateEvent(NULL, false, true, overlappedEventName);
	if (hEvent == NULL || hEvent == INVALID_HANDLE_VALUE)
	{
		cout << "CreateEvent() error: " << GetLastError() << endl;
		exit(1);
	}

	return hEvent;
}

void createSignalObject(struct Data *data)
{
	InitializeCriticalSection(&(data->readerSection));
	InitializeCriticalSection(&(data->writerSection));
	EnterCriticalSection(&(data->writerSection));
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
	//int len = 0;

	for(int i = 2; i < data->argc; i++) {
		EnterCriticalSection(&(data->writerSection));
		WaitForSingleObject(date->overRead->hEvent, INFINITE);
		//len += strlen(buf);
		WriteFile(hFile, buf, strlen(buf) + 1, NULL,  &(data->overWrite));
		LeaveCriticalSection(&(data->readerSection));
	}
	fclose(f);
}

unsigned  __stdcall readFiles(PVOID data_temp)
{
	struct Data* data;
	data = (struct Data*)data_tmp;
	HANDLE hFile;

	for(int i = 2; i < data->argc; i++) {
		EnterCriticalSection(&(data->readerSection));
		WaitForSingleObject(date->overWrite->hEvent, INFINITE);
		puts(data->argv[i-1]);
		hFile = CreateFile(data->argv[i-1], GENERIC_READ, 
		0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		//memset((void*)buf, 0, sizeof(buf));
		ReadFile(hFile, buf, maxFileSize, NULL, &(data->overRead));
		LeaveCriticalSection(&(data->writerSection));
		CloseHandle(hFile);
	}
}
