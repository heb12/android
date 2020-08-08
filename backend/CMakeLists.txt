cmake_minimum_required(VERSION 3.4.1)

add_library(backend SHARED
			fbrp/fbrp.c
            main.c
			biblec/biblec.c)

target_link_libraries(backend
                      android
                      log)
