#include<stdio.h>
#include<error.h>
#include "hanoi.h"

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "Input should be: './hanoi input_file > output_file'\n");
		return -1;
	}

	hanoi(argv[1]);

	return 0;
}
