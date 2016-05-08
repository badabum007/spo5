#include "headers.h"

void createSignalObject(struct Data *data)
{
	data->count = 0;
	InitializeCriticalSection(&(data->CriticalSection));
}

unsigned  __stdcall printThreads(PVOID data_temp)
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

void createNewThread(struct Data *data, struct Stack **stack)
{	
	data->count++;	
	struct Data* data_temp = (struct Data*)malloc (sizeof(struct Data));
	*data_temp = *data;
	HANDLE hand;
	hand = (HANDLE)_beginthreadex( NULL, 0, printThreads, (PVOID)data, 0, NULL);
	data->ThreadId = hand;
	push(*&stack, *data);
}

void closeLastThread(struct Stack **stack, struct Data *data)
{
	EnterCriticalSection(&(data->CriticalSection));
	TerminateThread((*stack)->ThreadId, 0);
	pop(*&stack);
	data->count --;
	LeaveCriticalSection(&(data->CriticalSection));
}

void closeAllThreads(struct Stack **stack, struct Data *data)
{
	EnterCriticalSection(&(data->CriticalSection));
	while (size(*stack) != 0)
	{
		TerminateThread((*stack)->ThreadId, 0);
		pop(*&stack);
		data->count --;
	}
	LeaveCriticalSection(&(data->CriticalSection));
}

void closeSignalObject(struct Data *data)
{
	DeleteCriticalSection(&(data->CriticalSection));
}
