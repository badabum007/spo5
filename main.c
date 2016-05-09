#include "headers.h"

int main(int argc, char** argv)
{
	struct Data data;
	if (argc < 3){
		puts("Not enough args");
		exit(1);
	}
	//last argv - output file
	//1 - prelast argv - input files
	data.argc = argc;
	data.argv = argv;

	createSignalObject(&data);
	dynLabOpen(&data);
	createReader(&data);
	callWriteFiles(&data);
	dynLabClose(&data);
	closeSignalObject(&data);
	return 0;
}
