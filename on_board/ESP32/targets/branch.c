#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fuzz.h>
#include "esp_attr.h"

//extern void _coverage_trace(void);
volatile uint16_t test_var1 = 0;
volatile uint16_t test_var2 = 0;
volatile uint16_t test_var3 = 0;
volatile uint16_t test_var4 = 0;
volatile uint16_t test_var5 = 0;
volatile uint16_t test_var6 = 0;
volatile uint16_t test_var7 = 0;
volatile uint16_t test_var8 = 42;
int16_t IRAM_ATTR branch_test(uint8_t * args, size_t size){
	
	static uint16_t test_var = 10;
	test_var1++;
	test_var2++;
	test_var3++;
	test_var4++;
	test_var5++;
	test_var6++;
	test_var7++;
//	__sanitizer_cov_trace_pc();
//_coverage_trace();
	if(args[0] == 'D'){
//		__sanitizer_cov_trace_pc();
	//printf("Hit the D case\n")
	//;
		test_var8++;
		test_var++;
		if(args[1] == 'E'){
//			__sanitizer_cov_trace_pc();
//			_coverage_trace();
//			printf("Hit the E case\n");
			test_var++;
			if(args[2]== 'A'){
//				__sanitizer_cov_trace_pc();
//				_coverage_trace();
//				printf("Hit the A case \n");
				if(args[3] == 'D'){
//					__sanitizer_cov_trace_pc();
//					_coverage_trace();
//					printf("Hit the D case \n");
						if(args[4] == 'B'){
//						_coverage_trace();
//						printf("Hit the D case\n");
						if(args[5] == 'E'){
//							_coverage_trace();
//							printf("Hit the E case\n");
							if(args[6]== 'E'){
//								_coverage_trace();
//								printf("Hit the E case \n");
								if(args[7] == 'F'){
									//while(1);
									crash_handler();
//									_coverage_trace();
//									printf("Hit the F case \n");
//									printf("Congrats you got to the end :)\n");
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}
