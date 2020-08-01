cmake_minimum_required(VERSION 3.4.1)

add_library(backend SHARED
			fbrp/fbrp.c
            main.c
			biblec/biblec.c)

# Include libraries needed for hello-jni lib
target_link_libraries(backend
                      android
                      log)
