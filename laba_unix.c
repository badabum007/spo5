#include "headers.h"

void* readFiles(void* data_tmp)
{
	struct Data* data;
	data = (struct Data*)data_tmp;
	aiocb_.aio_buf = buf;
	FILE* f;

	for(int i = 2; i < data->argc; i++) {
		pthread_mutex_lock(&(data->mutexRead));
		f = fopen(data->argv[i-1],"r");
		memset((void*)buf, 0, sizeof(buf));
		aiocb_.aio_nbytes = maxFileSize;
		aiocb_.aio_fildes = fileno(f);
		aiocb_.aio_offset = 0;
		aio_read(&aiocb_);
		while(aio_error(&aiocb_) != 0);
		pthread_mutex_unlock(&(data->mutexWrite));
		fclose(f);
	}
	pthread_exit(&(data->readerStatus));
}

/*void* writeFiles(void* data_tmp)
{
	struct Data data;
	data = *((struct Data*)data_tmp);

	FILE* f;
	f = fopen(data.argv[data.argc - 1],"w");

	for(int i = 2; i < data.argc; i++) {
		pthread_mutex_lock(&(data.mutex));
		puts(buf);
		aiocb_.aio_nbytes = strlen(buf);
		aiocb_.aio_fildes = fileno(f);
		aiocb_.aio_offset = 0;
		aio_write(&aiocb_);
		while(!aio_error(&aiocb_));
		pthread_mutex_unlock(&(data.mutex));
	}
	fclose(f);
	pthread_exit(&(data.writerStatus));
}*/

void createReader(struct Data *data)
{	
	if (pthread_create(&(data->reader), NULL, &readFiles, (void*)data)){
		perror("reader_create error");
		exit(1);
	}
}

/*void createWriter(struct Data *data)
{	
	if (pthread_create(&(data->writer), NULL, &writeFiles, (void*)data)){
		perror("writer_create error");
		exit(1);
	}
}*/

void createSignalObject(struct Data *data)
{
	pthread_mutex_init(&(data->mutexRead), NULL);
	pthread_mutex_unlock(&(data->mutexRead));
	pthread_mutex_init(&(data->mutexWrite), NULL);
	pthread_mutex_lock(&(data->mutexWrite));
}

void closeSignalObject(struct Data *data)
{
	pthread_mutex_destroy(&(data->mutexRead));
	pthread_mutex_destroy(&(data->mutexWrite));
}
