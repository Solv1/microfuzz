#include <stdio.h>
#include <stdint.h>
#include <string.h>

int16_t branch_test(uint8_t * args, size_t size){
	if(args[0] == 'D'){
//		printf("Hit the D case\n");
		if(args[1] == 'E'){
//			printf("Hit the E case\n");
			if(args[2]== 'A'){
				if(args[3] == 'D'){
//					printf("Hit the D case \n");
						if(args[4] == 'B'){
//						printf("Hit the D case\n");
						if(args[5] == 'E'){
//							printf("Hit the E case\n");
							if(args[6]== 'E'){
//								printf("Hit the E case \n");
								if(args[7] == 'F'){
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
