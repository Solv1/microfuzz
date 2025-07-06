#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>



uint32_t g_var1 = 10;
uint32_t g_var2 = 20;


void increment_static(void){
	static uint32_t s_var1 = 0;
	static uint32_t s_var2 = 5;

	s_var1++;
	s_var2++;
	g_var1++;


	if (!(s_var1 % 10)){
		g_var1++;
		g_var2++;
	}

}

int16_t global_test(uint8_t * args, size_t size){
	if(args[0] == 'D'){
		increment_static();
	}
	return 0;
}
