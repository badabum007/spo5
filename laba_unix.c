#include "headers.h"

#define maxFileSize 200

char buf[maxFileSize];
struct aiocb aiocbRead;
struct aiocb aiocbWrite;

void* readFiles(void* data_tmp)
{
	struct Data data;
	data = *((struct Data*)data_tmp);
	free(data_tmp);

	FILE* f;
	aiocbRead.aio_buf = buf;
	aiocbRead.aio_nbytes = maxFileSize;

	for(int i = 2; i < data.argc; i++) {
		pthread_mutex_lock(&(data.mutex));
		while(!aio_error(&aiocbWrite));
		f = fopen(data.argv[i-1],"r");
		aiocbRead.aio_fildes = f;
		aiocbRead.aio_offset = 0;
		aio_read(&aiocbRead);
		pthread_mutex_unlock(&(data.mutex));
		fclose(f);
	}
	pthread_exit(NULL);
}

void* writeFiles(void* data_tmp)
{
	struct Data data;
	data = *((struct Data*)data_tmp);
	free(data_tmp);

	struct aiocb aiocbRead;
	FILE* f;
	f = fopen(data.argv[data.argc - 1],"r");
	aiocbWrite.aio_buf = buf;
	aiocbWrite.aio_nbytes = strlen(buf);
	aiocbRead.aio_fildes = f;
	aiocbRead.aio_offset = 0;

	for(int i = 2; i < data.argc; i++) {
		while(!aio_error(&aiocbRead));
		pthread_mutex_lock(&(data.mutex));
		aio_write(&aiocbRead);
		pthread_mutex_unlock(&(data.mutex));
	}
	fclose(f);
	pthread_exit(NULL);
}

void createReader(struct Data *data)
{	
	if (pthread_create(&(data->reader), NULL, &readFiles, (void*)data)){
		perror("reader_create error");
		exit(1);
	}
}

void createWriter(struct Data *data)
{	
	if (pthread_create(&(data->writer), NULL, &writeFiles, (void*)data)){
		perror("writer_create error");
		exit(1);
	}
}

void createSignalObject(struct Data *data)
{
	pthread_mutex_init(&(data->mutex), NULL);
	pthread_mutex_unlock(&(data->mutex));
}

void closeSignalObject(struct Data *data)
{
	pthread_mutex_destroy(&(data->mutex));
}
