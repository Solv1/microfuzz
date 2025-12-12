set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -mtext-section-literals")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -mtext-section-literals")

idf_component_register(SRCS "ESP32.c"
                    INCLUDE_DIRS "."
		    "../"
		    PRIV_REQUIRES fuzz
		    )
target_compile_options(${COMPONENT_LIB} PRIVATE -mtext-section-literals)
#target_compile_options(${COMPONENT_LIB} PRIVATE -O0)
#target_compile_options(${COMPONENT_LIB} PRIVATE -g)
