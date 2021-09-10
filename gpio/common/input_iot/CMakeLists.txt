set(app_src input_iot/input_iot.c)

idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    )
