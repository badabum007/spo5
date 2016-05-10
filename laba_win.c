#include "headers.h"

void createReader(struct Data *data)
{	
	data->readerEventName = TEXT("ReadEvent");
	data->writerEventName = TEXT("WriteEvent");
	ZeroMemory(&(data->overRead), sizeof(OVERLAPPED));
	ZeroMemory(&(data->overWrite), sizeof(OVERLAPPED));
	data->overRead->hEvent = createEvent(data->readEventName);
	data->overWrite->hEvent = createEvent(data->writeEventName);

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
	FILE* f;
	f = fopen(data->argv[data->argc - 1],"w");
	int len = 0;

	for(int i = 2; i < data->argc; i++) {
		pthread_mutex_lock(&(data->mutexWrite));
		aiocb_.aio_nbytes = strlen(buf);
		aiocb_.aio_fildes = fileno(f);
		aiocb_.aio_offset = len;
		len += strlen(buf);
		aio_write(&aiocb_);
		while(aio_error(&aiocb_) != 0);
		pthread_mutex_unlock(&(data->mutexRead));
	}
	fclose(f);
}

unsigned  __stdcall readFiles(PVOID data_temp)
{
	struct Data* data;
	data = (struct Data*)data_tmp;

	FILE* f;
	for(int i = 2; i < data->argc; i++) {
		EnterCriticalSection(&(data->readerSection));
		puts(data->argv[i-1]);
		f = fopen(data->argv[i-1],"r");
		memset((void*)buf, 0, sizeof(buf));

		ReadFile(hCom2, bufrd, btr, NULL, &(data->over));
		while(aio_error(&aiocb_) != 0);
		LeaveCriticalSection(&(data->writerSection));
		fclose(f);
	}
}



/*unsigned  __stdcall printThreads(PVOID data_temp)
{
	struct Data data;
	data = *((struct Data*)data_temp);
	free(data_temp);
	while (1)
	{
		Sleep(200);
		EnterCriticalSection(&(data.CriticalSection));
		printf("thread number: %d\n", data.count);
		LeaveCriticalSection(&(data.CriticalSection));
	}
}
