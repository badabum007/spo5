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

void writeFiles(struct Data* data)
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