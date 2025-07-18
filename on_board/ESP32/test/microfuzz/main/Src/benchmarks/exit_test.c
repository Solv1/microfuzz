#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

void exit_test(void){
	uint16_t var = 1;
	uint16_t var2;

	var2 = var >> 2;

	exit(-1);

}

void _exit_test(void){
	uint16_t var = 1;
	uint16_t var2;

	var2 = var >> 2;

	_exit(-1);
}
