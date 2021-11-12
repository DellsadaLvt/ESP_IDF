set(pri_req json)
idf_component_register(SRCS "test.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req})