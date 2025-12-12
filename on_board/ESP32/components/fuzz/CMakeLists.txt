
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mtext-section-literals")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -mtext-section-literals")

idf_component_register(SRCS "fuzz.c"
		"trace-pc-cb.c"
		"time_bare.c"
		"harness.c"
                INCLUDE_DIRS "./include/"
		"./"
		REQUIRES esp_timer esp_mm target
		)
	#set(ALLOWLIST_FILE "${CMAKE_SOURCE_DIR}/fuzz/allowlist.txt")

	#add_compile_options(
	#-fsanitize-coverage=trace-pc
	#-fsanitize-coverage-allowlist=${ALLOWLIST_FILE}
	#)
	#target_compile_options(${COMPONENT_LIB} PRIVATE -fsanitize-coverage-allowlist=allowlist.txt)
#target_compile_options(${COMPONENT_LIB} PRIVATE -fsanitize-coverage=trace-pc)
#target_compile_options(${COMPONENT_LIB} PRIVATE -fsanitize-coverage-ignorelist=blocklist.txt)
target_link_options(${COMPONENT_LIB} PRIVATE "-Wl,--wrap=panic_abort")
target_compile_options(${COMPONENT_LIB} PRIVATE -mtext-section-literals)
target_compile_options(${COMPONENT_LIB} PRIVATE -fno-omit-frame-pointer)
target_compile_options(${COMPONENT_LIB} PRIVATE -O0)
#target_compile_options(${COMPONENT_LIB} PRIVATE -g)
