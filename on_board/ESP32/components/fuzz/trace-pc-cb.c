#include <string.h>
#include <stdint.h>


#include "esp_log.h"
#include "esp_cache.h"
#include "esp_attr.h"  

extern uint8_t g_isIncreasing;
uint32_t g_coverageList[100];
uint32_t g_covListIndex = 0;

void IRAM_ATTR __sanitizer_cov_trace_pc(void){
	volatile uint32_t val;
	volatile uintptr_t word;
	void *ra = __builtin_extract_return_addr(__builtin_return_address(0));
//	ra = (uint8_t *)ra - 0x40000000;
	ra = (uint8_t *)ra - 3;
	g_coverageList[g_covListIndex] = (uint32_t )ra;
	g_covListIndex++;
	g_isIncreasing = 1;

//	ESP_LOGI("DEBUG", "Return address: %p\n", ra);
	asm volatile("nop");
	//memset(ra, 0, 3);

	uint8_t shift = ((uintptr_t)ra % 4);

	if (shift == 3){
		word = (uintptr_t)ra - shift; //At even alignment
					      //
		val = *(volatile uint32_t *)word;
		val = val & 0x00FFFFFF;
		val = val | 0xF0000000;
		*(volatile uint32_t *)word = val;
		esp_cache_msync((uint32_t *)word, 4, ESP_CACHE_MSYNC_FLAG_DIR_M2C);
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");

		word = word + 4; //Now the high byte;
		val = *(volatile uint32_t *)word;
		val = val & 0xFFFF0000;
		val = val | 0x00000020;
		*(volatile uint32_t *)word = val;
		esp_cache_msync((uint32_t *)word, 4, ESP_CACHE_MSYNC_FLAG_DIR_M2C);
	}
	else if (shift == 2) {
		word = (uintptr_t)ra - shift;
		val = *(volatile uint32_t *)word;
		val = val & 0x0000FFFF;
		val = val | 0x20F00000;
		*(uint32_t *)word = val;
		esp_cache_msync((uint32_t *)word, 4, ESP_CACHE_MSYNC_FLAG_DIR_M2C);

		word = word + 4;
		val = *(volatile uint32_t *)word;
		val = val & 0xFFFFFF00;
		*(uint32_t *)word = val;
		esp_cache_msync((uint32_t *)word, 4, ESP_CACHE_MSYNC_FLAG_DIR_M2C);
	}
	else if (shift == 1) {
		word = (uintptr_t)ra - shift;
		val = *(volatile uint32_t *)word;
		val = val & 0x000000FF;
		val = val | 0x0020F000;
		*(uint32_t *)word = val;
		esp_cache_msync((uint32_t *)word, 4, ESP_CACHE_MSYNC_FLAG_DIR_M2C);
	}
	else{ //Perfectly aligned :)
		word = (uintptr_t)ra;
		val = *(volatile uint32_t *)word;
		val = val & 0xFF000000;
		val = val | 0x000020F0;
		*(uint32_t *)word = val;
		esp_cache_msync((uint32_t *)word, 4, ESP_CACHE_MSYNC_FLAG_DIR_M2C);
	}
}
