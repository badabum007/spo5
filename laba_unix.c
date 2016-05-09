#include "headers.h"

void createReader(struct Data *data)
{	
	void* (*readFiles) (void*);
	*(void**)(&readFiles) = dlsym(data->handle, "readFiles");
	if (pthread_create(&(data->reader), NULL, readFiles, (void*)data)){
		perror("reader_create error");
		exit(1);
	}
}

void createSignalObject(struct Data *data)
{
	pthread_mutex_init(&(data->mutexRead), NULL);
	pthread_mutex_unlock(&(data->mutexRead));
	pthread_mutex_init(&(data->mutexWrite), NULL);
	pthread_mutex_lock(&(data->mutexWrite));
	data->readerStatus = 0;
	strcpy(data->libName, "./fileconcat.so");
}

void closeSignalObject(struct Data *data)
{
	pthread_mutex_destroy(&(data->mutexRead));
	pthread_mutex_destroy(&(data->mutexWrite));
}

void dynLabOpen(struct Data* data)
{
	data->handle = dlopen(data->libName, RTLD_LAZY);
}

void dynLabClose(struct Data* data)
{
	dlclose(data->handle);
}

void callWriteFiles(struct Data* data)
{
	void (*writeFiles) (void*);
	*(void**)(&writeFiles) = dlsym(data->handle, "writeFiles");
	writeFiles(data);
}
