set(pri_req esp-tls)
idf_component_register(SRCS "user_https_request.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req})