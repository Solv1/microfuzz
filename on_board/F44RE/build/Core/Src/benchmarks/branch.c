#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fuzz.h>
//extern void _coverage_trace(void);

int16_t branch_test(uint8_t * args, size_t size){
//_coverage_trace();
	if(args[0] == 'D'){
//		_coverage_trace();
//		printf("Hit the D case\n");
		if(args[1] == 'E'){
//			_coverage_trace();
//			printf("Hit the E case\n");
			if(args[2]== 'A'){
//				_coverage_trace();
//				printf("Hit the A case \n");
				if(args[3] == 'D'){
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
