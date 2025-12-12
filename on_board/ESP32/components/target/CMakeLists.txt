idf_component_register(SRCS "target.c"
		    REQUIRES fuzz			
                    INCLUDE_DIRS "include"
		    "../")
target_compile_options(${COMPONENT_LIB} PRIVATE -fsanitize-coverage=trace-pc)
target_compile_options(${COMPONENT_LIB} PRIVATE -mtext-section-literals)
target_compile_options(${COMPONENT_LIB} PRIVATE -fno-omit-frame-pointer)
#target_compile_options(${COMPONENT_LIB} PRIVATE -O0)
