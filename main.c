#include "headers.h"

int main(int argc, char** argv)
{
	struct Data data;
	if (argc < 3){
		puts("Not enough args");
		exit(1);
	}
	createSignalObject(&data);

	//last argv - output file
	//1 - prelast argv - input files
	data.argc = argc;
	data.argv = argv;
	data.readerStatus = 0;
	data.writerStatus = 0;
	createReader(&data);
	//createWriter(&data);

	FILE* f;
	f = fopen(data.argv[data.argc - 1],"w");
	int len = 0;

	for(int i = 2; i < data.argc; i++) {
		pthread_mutex_lock(&(data.mutexWrite));
		aiocb_.aio_nbytes = strlen(buf);
		aiocb_.aio_fildes = fileno(f);
		aiocb_.aio_offset = len;
		len += strlen(buf);
		aio_write(&aiocb_);
		while(aio_error(&aiocb_) != 0);
		pthread_mutex_unlock(&(data.mutexRead));
	}
	fclose(f);
	closeSignalObject(&data);
	return 0;
}