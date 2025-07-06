#include <stdio.h>
#include <stdint.h>
#include <string.h>

int16_t branch_test(int8_t * args, int16_t size){
	
	if(args[0] == 'D'){
		_coverage_trace();		
		printf("Hit the D case\n");
		if(args[1] == 'E'){
			_coverage_trace();		
			printf("Hit the E case\n");
			if(args[2]== 'A'){
				_coverage_trace();		
				printf("Hit the A case \n");
				if(args[3] == 'D'){
					_coverage_trace();		
					printf("Hit the D case \n");
						if(args[4] == 'B'){
						_coverage_trace();		
						printf("Hit the D case\n");
						if(args[5] == 'E'){
							_coverage_trace();		
							printf("Hit the E case\n");
							if(args[6]== 'E'){
								_coverage_trace();		
								printf("Hit the E case \n");
								if(args[7] == 'F'){
									_coverage_trace();		
									printf("Hit the F case \n");		
									printf("Congrats you got to the end :)\n");
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
