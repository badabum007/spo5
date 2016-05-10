#include "headers.h"

unsigned __stdcall readCall(void* data_tmp)
{
	typedef void (*readFiles) (struct Data*);
	struct Data* data;
	data = (struct Data*)data_tmp;
	readFiles readfiles;
	readfiles = (readFiles)GetProcAddress(data->lib, "readFiles");
	readfiles(data);
	ExitThread(data->readerStatus);
}

void createReader(struct Data *data)
{	
	strcpy(data->readerEventName, "ReadEvent");
	strcpy(data->writerEventName, "WriteEvent");
	ZeroMemory(&(data->overRead), sizeof(OVERLAPPED));
	ZeroMemory(&(data->overWrite), sizeof(OVERLAPPED));

	data->overRead.hEvent = createEvent(data->readerEventName);
	data->overWrite.hEvent = createEvent(data->writerEventName);
	SetEvent(data->overWrite.hEvent);
	//readfiles((PVOID)data);

	HANDLE hand;
	hand = (HANDLE)_beginthreadex( NULL, 0, readCall, (void*)data, 0, NULL);
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
	strcpy(data->libName, "fileconcat.dll");
}

void closeSignalObject(struct Data *data)
{
	DeleteCriticalSection(&(data->readerSection));
	DeleteCriticalSection(&(data->writerSection));
}

void dynLabOpen(struct Data* data)
{
	data->lib = LoadLibrary(data->libName);
}

void dynLabClose(struct Data* data)
{
	FreeLibrary(data->lib);
}

void callWriteFiles(struct Data* data)
{
	typedef void (*writeFiles) (struct Data*);
	writeFiles writefiles;
	writefiles = (writeFiles)GetProcAddress(data->lib, "writeFiles");
	writefiles(data);
}
