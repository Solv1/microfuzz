idf_component_register(
	SRCS
		"fuzz.c"
		"coverage.S"
		"harness.c"
		"time_bare.c"
	INCLUDE_DIRS "./"
	REQUIRES benchmarks
	)
