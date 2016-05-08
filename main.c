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
	createReader(&data);
	createWriter(&data);
	puts("Success");

	closeSignalObject(&data);
	return 0;
}